#include "llvm/IR/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include <string>
#include <vector>
#include <iostream>

using namespace llvm;

#ifndef IRGENERATOR_H
#define IRGENERATOR_H
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


// Int32ExprAST - Expression class for int32 literals
class Int32ExprAST : public ExprAST {
  int32_t val;

public:
  Int32ExprAST(int32_t input) : val(input) {}

  Value* codegen() override;
};


// PrototypeAST - This class represents the "prototype" for a function,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string& name, std::vector<std::string> args)
	  : Name(name), Args(std::move(args)) {}

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

//Constructs a 32 bit signed integer
Value* Int32ExprAST::codegen() {
  return ConstantInt::get(*TheContext, APInt(32, static_cast<uint64_t>(val), true));
}

Function* PrototypeAST::codegen() {
  // Make the function type: int(int, int, ...) etc.
  std::vector<Type*> Ints(Args.size(), Type::getInt32Ty(*TheContext));
  
  FunctionType* FT = FunctionType::get(Type::getInt32Ty(*TheContext), Ints, false);

  Function* F = Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
	  Arg.setName(Args[Idx++]);

  return F;
}


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
}

#endif
