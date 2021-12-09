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

#endif
