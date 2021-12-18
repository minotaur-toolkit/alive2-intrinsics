#include "irGenerator.hpp"
#include "vectorutil.hpp"

#ifndef IRWRAPPER_H
#define IRWRAPPER_H

using namespace llvm;

template<unsigned bitwidth, typename type>
std::unique_ptr<IntVectorExprAST> generateVector(type input) {
	std::variant<int64_t*, int32_t*, int16_t*, int8_t*> vals;
	IntegerType* intType;
        if constexpr (bitwidth == 64) {
                vals = (int64_t*)&input;
		intType = IRint64_t;
	} else if constexpr (bitwidth == 32) {
                vals = (int32_t*)&input;
		intType = IRint32_t;
	} else if constexpr (bitwidth == 16) {
                vals = (int16_t*)&input;
		intType = IRint16_t;
	} else if constexpr (bitwidth == 8) {
                vals = (int8_t*)&input;
		intType = IRint8_t;
	} else
                throw std::invalid_argument("Bitwidth must be powers of 2 between 2 and 64");

        constexpr unsigned vectorBitSize = bitSize<type>();

	std::unique_ptr<IntExprAST> expr;
	std::vector<std::unique_ptr<IntExprAST>> valVec;

        for(unsigned i = 0; i < vectorBitSize / bitwidth; ++i)
                std::visit([&](auto&& arg)
				{
				  expr = std::make_unique<IntExprAST>(arg[i], intType);
				  valVec.push_back(std::move(expr));
				}, vals);

	return std::make_unique<IntVectorExprAST>(intType, std::move(valVec));
  
}

template<unsigned bitwidth, typename type>
Function* generateReturnFunction(type input, std::string name) {
	std::unique_ptr<IntVectorExprAST> vecExpr = generateVector<bitwidth>(input);

	IntegerType* intType;
	if constexpr (bitwidth == 64) 
		intType = IRint64_t;
	else if constexpr (bitwidth == 32) 
		intType = IRint32_t;
	else if constexpr (bitwidth == 16)
		intType = IRint16_t;
	else if constexpr (bitwidth == 8)
		intType = IRint8_t;
	else
		throw std::invalid_argument("Bitwidth must be powers of 2 between 2 and 64");

        constexpr unsigned vectorBitSize = bitSize<type>();
	constexpr unsigned vectorLength = vectorBitSize / bitwidth;

	std::unique_ptr<PrototypeAST> proto =
	std::make_unique<PrototypeAST>(name, VectorType::get(intType, vectorLength, false), std::vector<Type*>({}));

	std::unique_ptr<FunctionAST> func = std::make_unique<FunctionAST>(std::move(proto), std::move(vecExpr));
	return func->codegen();
}

template<unsigned bitwidthOp1, unsigned bitwidthOp2, typename typeOp1, typename typeOp2>
Function* generateCallFunction(typeOp1 input1, typeOp2 input2, Function* func, std::string name) {
	std::unique_ptr<IntVectorExprAST> vecExprOp1 = generateVector<bitwidthOp1>(input1);
	std::unique_ptr<IntVectorExprAST> vecExprOp2 = generateVector<bitwidthOp2>(input2);
	
	std::vector<std::unique_ptr<ExprAST>> callVec;
	callVec.push_back(std::move(vecExprOp1));
	callVec.push_back(std::move(vecExprOp2));

	std::unique_ptr<CallExprAST> callExpr =
	std::make_unique<CallExprAST>(func, std::move(callVec));

	std::unique_ptr<PrototypeAST> proto = 
	std::make_unique<PrototypeAST>(name, func->getReturnType(), std::vector<Type*>({}));

	std::unique_ptr<FunctionAST> returnFunction =
	std::make_unique<FunctionAST>(std::move(proto), std::move(callExpr));

	return returnFunction->codegen();
}

template<unsigned bitwidth, typename type>
VectorType* getVectorType(type input) {
	IntegerType* intType;
        if constexpr (bitwidth == 64)
                intType = IRint64_t;
        else if constexpr (bitwidth == 32)
                intType = IRint32_t;
        else if constexpr (bitwidth == 16)
                intType = IRint16_t;
        else if constexpr (bitwidth == 8)
                intType = IRint8_t;
        else
                throw std::invalid_argument("Bitwidth must be powers of 2 between 2 and 64");

        constexpr unsigned vectorBitSize = bitSize<type>();
        constexpr unsigned vectorLength = vectorBitSize / bitwidth;

	return VectorType::get(intType, vectorLength, false);
}

template<unsigned bitwidthOp1, unsigned bitwidthOp2, typename typeOp1, typename typeOp2>
Function* generateCallFunctionWithVarInput(typeOp1 input1, typeOp2 input2, Function* func, std::string name) {
	std::unique_ptr<VariableExprAST> vecVarOp1 = std::make_unique<VariableExprAST>("i0");
	std::unique_ptr<VariableExprAST> vecVarOp2 = std::make_unique<VariableExprAST>("i1");
	
	std::vector<std::unique_ptr<ExprAST>> callVec;
	callVec.push_back(std::move(vecVarOp1));
	callVec.push_back(std::move(vecVarOp2));

	std::unique_ptr<CallExprAST> callExpr =
	std::make_unique<CallExprAST>(func, std::move(callVec));

	// Get input variable types
	VectorType* vecTypeOp1 = getVectorType<bitwidthOp1>(input1);
	VectorType* vecTypeOp2 = getVectorType<bitwidthOp2>(input2);

	std::unique_ptr<PrototypeAST> proto = 
	std::make_unique<PrototypeAST>(name, func->getReturnType(), std::vector<Type*>({vecTypeOp1, vecTypeOp2}));

	std::unique_ptr<FunctionAST> returnFunction =
	std::make_unique<FunctionAST>(std::move(proto), std::move(callExpr));

	return returnFunction->codegen();
}
#endif
