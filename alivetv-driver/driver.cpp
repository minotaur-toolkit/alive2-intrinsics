#include "vectorutil.hpp"
#include "x86Intrin.hpp"
#include "randomizer.hpp"
#include "irGenerator.hpp"
#include "irWrapper.hpp"
#include "JIT.hpp"

#include "compareFunctions.cpp"

#include "llvm/Support/TargetSelect.h"


int main()
{		
	//Initialize llvm module and intrinsic module
	InitializeModule();

	//Initialize native target information for Just In Time Compiler	
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();

	//Initialize Just In Time Compiler
	auto JITInit = llvm::orc::JIT::Create();

	//Below executes if initializing JIT failed
	if (auto E = JITInit.takeError()) {
	  errs() << "Problem with JIT " << toString(std::move(E)) << "\n";
	  return -1;
	}
	std::unique_ptr<llvm::orc::JIT> JITCompiler = std::move(*JITInit);
	
	//Set data layout of module
	TheModule->setDataLayout(JITCompiler->getDataLayout());

	// All intrinsic functions must be added below (probably in some for loop)	
	llvm::Function* testFunc = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::x86_sse2_pavg_w);
	generateCallFunctionFromFunction(testFunc, "test2");
	
	//Debug printing for module
	TheModule->print(errs(), nullptr);
	
	//Add intrinsic module to JIT
	auto JITadd = JITCompiler->addModule(llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext)));
	
	//Check if adding module to JIT errored
	if(JITadd) {
 	  errs() << "Problem adding module to JIT " << JITadd << "\n";
	  exit(-1);
	}
	auto funcLookup = JITCompiler->lookup("test2");

	//Below executes if looking up JIT function failed
	if (auto E = funcLookup.takeError()) {
	  errs() << "Problem with JIT lookup " << toString(std::move(E)) << "\n";
	  return -1;
	}

	auto* funcPointer = (__m128i(*)(__m128i, __m128i))funcLookup->getAddress();
	
	
	switchToAliveContext();

	//Initialize Alive2
	llvm::Triple targetTriple(TheModule.get()->getTargetTriple());
	llvm::TargetLibraryInfoWrapperPass TLI(targetTriple);
	
	//Set up output stream for Alive2 info, then set up smt
	out = &std::cout;
	smt_init.emplace();
	
	
	//This variable below is the only variable that needs to be manually changed in main
	//(with the exception of the ranges on vectorRandomizer in the for loop)
	constexpr X86IntrinBinOp::Op op = X86IntrinBinOp::sse2_pavg_w;
	
	constexpr unsigned timesToLoop = 10000;	

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
	//retVec = ret
	//Code below initializes the vectors to 0 with the proper types for the operation
	auto vals = std::get<vectorTypeIndex<op0BitSize>()>(returnVectorType<op0BitSize>());
	auto vals2 = std::get<vectorTypeIndex<op1BitSize>()>(returnVectorType<op1BitSize>());
	auto retVec = std::get<vectorTypeIndex<retBitSize>()>(returnVectorType<retBitSize>());

	//Defines the function pointer type that the function should use,
	//ie auomatically chooses return, op1, and op2 types
	/*
	typedef 
		std::conditional<retBitSize != 512, std::conditional<retBitSize == 256, __m256i, __m128i>::type, __m512i>::type
		(*opFunctionType)
		(std::conditional<op0BitSize != 512, std::conditional<op0BitSize == 256, __m256i, __m128i>::type, __m512i>::type,
		std::conditional<op1BitSize != 512, std::conditional<op1BitSize == 256, __m256i, __m128i>::type, __m512i>::type);

	*/
	//auto function = reinterpret_cast<opFunctionType>(return_function<op>());
	//This jank might truly be real honest to god undefined behavior above, someone help	
	

	//Declare the intrinsic to be used
	llvm::Function* intrinsicFunction = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::x86_sse2_pavg_w);

	//Loop that tests for the equality of both functions for equal inputs	
	for(int i = 0; i != timesToLoop; ++i) {
		vals = vectorRandomizer<op0Bitwidth>(vals);
		vals2 = vectorRandomizer<op1Bitwidth, 40>(vals2);
		
		retVec = funcPointer(vals, vals2);
		
		llvm::Function* tgtFunc = generateReturnFunction<retBitwidth>(retVec, "tgt");
		llvm::Function* srcFunc = generateCallFunction<op0Bitwidth, op1Bitwidth>(vals, vals2, intrinsicFunction, "src");

		compareFunctions(*srcFunc, *tgtFunc, TLI);

		tgtFunc->eraseFromParent();
		srcFunc->eraseFromParent();
	}

	std::cout << "Ran " << timesToLoop << " tests." << 
		"\nNum correct: " << num_correct << 
		"\nNum unsound: " << num_unsound << 
		"\nNum failed: " << num_failed << 
		"\nNum errors: " << num_errors << "\n";	

	return 0;
}

