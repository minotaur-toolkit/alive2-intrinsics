#include "vectorutil.hpp"
#include "x86Intrin.hpp"
#include "randomizer.hpp"
#include "irGenerator.hpp"

#include "compareFunctions.cpp"

__m128i src(__m128i a, __m128i b) {
	return _mm_avg_epu16(a, b);
}


int main() {
	InitializeModule();
	
	std::unique_ptr<Int32ExprAST> expr = std::make_unique<Int32ExprAST>(32);
	std::unique_ptr<PrototypeAST> proto = std::make_unique<PrototypeAST>("src", std::vector<std::string>({"i1"}));
	
	FunctionAST function(std::move(proto), std::move(expr));
	llvm::Function* func1 = function.codegen();

	expr = std::make_unique<Int32ExprAST>(32);
	proto = std::make_unique<PrototypeAST>("tgt", std::vector<std::string>({"i1"}));

	FunctionAST function2(std::move(proto), std::move(expr));
	llvm::Function* func2 = function2.codegen();

	//TheModule->print(errs(), nullptr);
	
	if(report_dir_created);	//Error about unsused variable, TODO: fix
	
	llvm::Triple targetTriple(TheModule.get()->getTargetTriple());
	llvm::TargetLibraryInfoWrapperPass TLI(targetTriple);
	
	out = &std::cout;
	smt_init.emplace();

	compareFunctions(*func1, *func2, TLI);
}

/*
int main()
{		
	//This variable below is the only variable that needs to be manually changed in main
	//(with the exception of the ranges on vectorRandomizer in the for loop)
	constexpr X86IntrinBinOp::Op op = X86IntrinBinOp::sse2_pavg_w;
	
	constexpr unsigned timesToLoop = 1000000;	
	int failedTests = 0;

	//Bitsize is the number of bits in the entire vector
	constexpr unsigned op0BitSize = bitSizeOp0<op>();
	constexpr unsigned op1BitSize = bitSizeOp1<op>();
	constexpr int retBitSize = bitSizeRet<op>();
	
	//Bitwidth is the number of bits in a lane
	constexpr unsigned op0Bitwidth = bitwidthOp0<op>();
	constexpr unsigned op1Bitwidth = bitwidthOp1<op>();
	constexpr unsigned retBitwidth = bitwidthRet<op>();	

	//vals = op0
	//vals2 = op1
	//test = ret (from external src file)
	//test2 = ret (from intrinsic call)
	//Code below initializes the vectors to 0 with the proper types for the operation
	auto vals = std::get<vectorTypeIndex<op0BitSize>()>(returnVectorType<op0BitSize>());
	auto vals2 = std::get<vectorTypeIndex<op1BitSize>()>(returnVectorType<op1BitSize>());
	auto test = std::get<vectorTypeIndex<retBitSize>()>(returnVectorType<retBitSize>());
	auto test2 = std::get<vectorTypeIndex<retBitSize>()>(returnVectorType<retBitSize>());

	//Defines the function pointer type that the function should use,
	//ie auomatically chooses return, op1, and op2 types
	typedef 
		std::conditional<retBitSize != 512, std::conditional<retBitSize == 256, __m256i, __m128i>::type, __m512i>::type
		(*opFunctionType)
		(std::conditional<op0BitSize != 512, std::conditional<op0BitSize == 256, __m256i, __m128i>::type, __m512i>::type,
		std::conditional<op1BitSize != 512, std::conditional<op1BitSize == 256, __m256i, __m128i>::type, __m512i>::type);


	auto function = reinterpret_cast<opFunctionType>(return_function<op>());
	auto function_external = reinterpret_cast<opFunctionType>(src);
	//This jank might truly be real honest to god undefined behavior above, someone help	



	//Loop that tests for the equality of both functions for equal inputs	
	for(int i = 0; i != timesToLoop; ++i) {
		vals = vectorRandomizer<op0Bitwidth>(vals);
		vals2 = vectorRandomizer<op1Bitwidth, 40>(vals2);
		test = function_external(vals, vals2);
		//vals = vectorRandomizer<op0Bitwidth>(vals); //Intentionally changes values so test fails
		test2 = function(vals, vals2);
		if(!areEqual<retBitwidth>(test, test2)) {
			printError<op0Bitwidth, op1Bitwidth, retBitwidth>(vals, vals2, test, test2);
			++failedTests;
		}

	}

	std::cout << "Ran " << timesToLoop << " tests.\nFailed " << failedTests << " tests.\n";	

	return 0;
}
*/
