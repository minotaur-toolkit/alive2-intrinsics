#include "vectorutil.hpp"
#include "x86Intrin.hpp"
#include "randomizer.hpp"
#include "irGenerator.hpp"
#include "irWrapper.hpp"

#include "compareFunctions.cpp"

__m128i src(__m128i a, __m128i b) {
	return _mm_avg_epu16(a, b);
}


int main() {
	InitializeModule();
	
	/*
	 * Code tests four things. Creates four functions.
	 * First function @src takes one i32 input and always returns i32 32
	 * Second function @tgt is defined the same as the first function
	 * Third function @vec takes no arguments and returns <4 x i32> with 32 i32 values
	 * Then, code declares an x86 intrinsic
	 * The final function @call takes no arguments, and returns a call to the intrinsic with constant values
	 */


	//Start function 1
	std::unique_ptr<IntExprAST> expr = std::make_unique<IntExprAST>(32, IRint32_t);
	std::unique_ptr<PrototypeAST> proto = std::make_unique<PrototypeAST>("src", IRint32_t, std::vector<Type*>({IRint32_t}));
	
	FunctionAST function(std::move(proto), std::move(expr));
	llvm::Function* func1 = function.codegen();
	//End function 1


	//Start function 2
	expr = std::make_unique<IntExprAST>(32, IRint32_t);
	proto = std::make_unique<PrototypeAST>("tgt", IRint32_t, std::vector<Type*>({IRint32_t}));

	FunctionAST function2(std::move(proto), std::move(expr));
	llvm::Function* func2 = function2.codegen();
	//End function 2


	//Start function 3
	__m128i vals = vectorRandomizer<32>(vals);
	
	llvm::Function* func3 = generateReturnFunction<32>(vals, "vec");

	//End function 3

	//Declare the intrinsic to be used
	llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::x86_sse2_psrl_d);

	//Start function 4
	//Create one of the operands for the call, store in vecExpr
	std::vector<std::unique_ptr<IntExprAST>> valVec;
	valVec.clear();
	for(int i = 0; i < 4; ++i) {
	  expr = std::make_unique<IntExprAST>(32, IRint32_t);
	  valVec.push_back(std::move(expr));
	}
	std::unique_ptr<IntVectorExprAST> vecExpr = 
	std::make_unique<IntVectorExprAST>(IRint32_t, std::move(valVec));	
	
	//Create the second set of operands for the call, store in vecExpr2
	valVec.clear();
	for(int i = 0; i < 4; ++i) {
	  expr = std::make_unique<IntExprAST>(32, IRint32_t);
	  valVec.push_back(std::move(expr));
	}	
	std::unique_ptr<IntVectorExprAST> vecExpr2 = 
	std::make_unique<IntVectorExprAST>(IRint32_t, std::move(valVec)); 
	
	//Create the vector callVec that contains the operands for the call
	std::vector<std::unique_ptr<ExprAST>> callVec;
	callVec.push_back(std::move(vecExpr));
	callVec.push_back(std::move(vecExpr2));	
	
	//Create the actual call instruction using the operands created above
	std::unique_ptr<CallExprAST> callExpr = 
	std::make_unique<CallExprAST>("llvm.x86.sse2.psrl.d", std::move(callVec));
	
	//Create the prototype
	proto = std::make_unique<PrototypeAST>("call", VectorType::get(IRint32_t, 4, false), std::vector<Type*>({}));

	//Combine the prototype and the call
	FunctionAST function4(std::move(proto), std::move(callExpr));
	llvm::Function* func4 = function4.codegen();
	//End function 4
	


	//Print the module to see the LLVM IR currently created
	TheModule->print(errs(), nullptr);
	
	if(report_dir_created);	//Error about unsused variable, TODO: fix
	
	//Set up TLI for Alive2
	llvm::Triple targetTriple(TheModule.get()->getTargetTriple());
	llvm::TargetLibraryInfoWrapperPass TLI(targetTriple);
	
	//Set up output stream for Alive2 info, then set up smt
	out = &std::cout;
	smt_init.emplace();

	//Compare functions
	compareFunctions(*func1, *func2, TLI);
	compareFunctions(*func3, *func4, TLI);
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
