#include "llvm/IR/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include <llvm/IR/IntrinsicsX86.h>

#include <string>
#include <vector>
#include <iostream>

using namespace llvm;

#ifndef IRGENERATOR_H
#define IRGENERATOR_H
//===----------------------------------------------------------------------===//
// Define easier to type IR types
//===----------------------------------------------------------------------===//

IntegerType* IRint8_t;
IntegerType* IRint16_t;
IntegerType* IRint32_t;
IntegerType* IRint64_t;

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

//namespace {

// ExprAST - Base class for all expression nodes
class ExprAST {

public:
  virtual ~ExprAST() {}
  virtual Value *codegen() = 0;
};


// IntExprAST - Expression class for integer literals
class IntExprAST : public ExprAST {
  uint64_t val;
  IntegerType* type;

public:
  IntExprAST(uint64_t input, IntegerType* iType) : val(input), type(iType) {}

  Value* codegen() override;
};

// VectorExprAST - Expression class for vectors of integer type
// Value and size of vector depends on size of input vector
class IntVectorExprAST : public ExprAST {
  IntegerType* ElementType;
  std::vector<std::unique_ptr<IntExprAST>> Vals;
  //Vals vector could have different width integers, could lead to issues

public:
  IntVectorExprAST(IntegerType* elementType, 
		   std::vector<std::unique_ptr<IntExprAST>> iVals)
	  : ElementType(elementType), 
	  Vals(std::move(iVals)) {}

  Value* codegen() override;
};

// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee = "";
  Function* CalleeF;
  std::vector<std::unique_ptr<ExprAST>> Args;
public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

  CallExprAST(Function* func, 
	      std::vector<std::unique_ptr<ExprAST>> Args)
      : CalleeF(func), Args(std::move(Args)) {}

  Value* codegen() override;
};


// PrototypeAST - This class represents the "prototype" for a function,
// which captures its name, and its argument types (thus implicitly the number
// of arguments the function takes).
class PrototypeAST {
  std::string Name;
  Type* RetType;
  std::vector<Type*> ArgTypes;

public:
  PrototypeAST(const std::string& name, Type* retType, std::vector<Type*> args)
	  : Name(name), RetType(retType), ArgTypes(std::move(args)) {}

  Function* codegen();
  const std::string& getName() const { return Name; }
};

// FunctionAST - This class represents a function definition itself
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:	
  FunctionAST(std::unique_ptr<PrototypeAST> inputProto, std::unique_ptr<ExprAST> inputBody)
	  : Proto(std::move(inputProto)), Body(std::move(inputBody)) {}

  Function* codegen();
};

//} // end anonymous namespace

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string, Value *> NamedValues;

// Constructs a 32 bit signed integer
Value* IntExprAST::codegen() {
  return ConstantInt::get(type, val);
}

// Constructs a constant vector out of input arguments
Value* IntVectorExprAST::codegen() {
  std::vector<Constant*> v;
  for(unsigned i = 0, e = Vals.size(); i != e; ++i)
  {
    v.push_back(static_cast<ConstantInt*>(Vals[i]->codegen()));
  }
  return ConstantVector::get(v);
}

// Constructs a function call
Value* CallExprAST::codegen() {
  //TODO: Add a check that compares if arguments have the correct types
	
  // Look up the name in the global module table.
  // If the function is not directly provided
  if(Callee != "") {
    CalleeF = TheModule->getFunction(Callee);
    if (!CalleeF) {
      std::cerr << "Unknown function referenced" << std::endl;
      throw std::invalid_argument("Unknown function referenced");
    }
  }
  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size()) {
    std::cerr << "Incorrect # arguments passed" << std::endl;
    throw std::invalid_argument("Incorrect # arguments passed");
  }

  std::vector<Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }
  return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

// Constructs the prototype for a function
Function* PrototypeAST::codegen() {
  
  FunctionType* FT = FunctionType::get(RetType, ArgTypes, false);

  Function* F = Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

  // Set names for all arguments.
  std::string inputChar = "i";
  unsigned Idx = 0;
  for (auto &Arg : F->args())
	  Arg.setName(inputChar + std::to_string(Idx++));

  return F;
}

// Constructs the body of a function
Function* FunctionAST::codegen() {
  // Check for existing function from a previous "extern" declaration
  Function* TheFunction = TheModule->getFunction(Proto->getName());  
  
  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;
  
  BasicBlock* BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[std::string(Arg.getName())] = &Arg;

  if (Value* RetVal = Body->codegen()) {
    // Finish off function by returning generated value
    Builder->CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}



//===----------------------------------------------------------------------===//
// Initialize LLVM objects
//===----------------------------------------------------------------------===//

static void InitializeModule() {
  // Open a new context and module.
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("Driver Module", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);


  IRint8_t = Type::getInt8Ty(*TheContext);
  IRint16_t = Type::getInt16Ty(*TheContext);
  IRint32_t = Type::getInt32Ty(*TheContext);
  IRint64_t = Type::getInt64Ty(*TheContext);
}



#endif
