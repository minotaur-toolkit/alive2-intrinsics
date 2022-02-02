#include <utility>

template<typename F, std::size_t... S>
constexpr void static_for(F&& function, std::index_sequence<S...>) {
    int unpack[] = {0,
        (void(function(std::integral_constant<std::size_t, S>{})), 0)...
    };

    (void) unpack;
}

template<std::size_t iterations, typename F>
constexpr void static_for(F&& function) {
    static_for(std::forward<F>(function), std::make_index_sequence<iterations>());
}

/*
template<size_t i>
constexpr bool singleTestLoop(std::index_sequence<i...>, llvm::orc::JIT* JITCompiler, llvm::TargetLibraryInfoWrapperPass* TLI)
{
	constexpr X86IntrinBinOp::Op op = static_cast<X86IntrinBinOp::Op>(n);
	
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
	typedef 
		std::conditional<retBitSize != 512, std::conditional<retBitSize == 256, __m256i, __m128i>::type, __m512i>::type
		(*opFunctionType)
		(std::conditional<op0BitSize != 512, std::conditional<op0BitSize == 256, __m256i, __m128i>::type, __m512i>::type,
		std::conditional<op1BitSize != 512, std::conditional<op1BitSize == 256, __m256i, __m128i>::type, __m512i>::type);
	

	auto* funcPointer = reinterpret_cast<opFunctionType>(JITCompiler->getFuncAddress("func6"));
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

		compareFunctions(*srcFunc, *tgtFunc, *TLI);
	}

	std::cout << "Ran " << timesToLoop << " tests." << 
		"\nNum correct: " << num_correct << 
		"\nNum unsound: " << num_unsound << 
		"\nNum failed: " << num_failed << 
		"\nNum errors: " << num_errors << "\n";	

	return true;
}

template< size_t...j>
constexpr bool loopHelper(std::index_sequence<j...>, llvm::orc::JIT* JITCompiler, llvm::TargetLibraryInfoWrapperPass* TLI)
{
    return ((singleTestLoop( std::make_index_sequence<j>{}, JITCompiler, TLI)),...);
}

template< size_t n>
constexpr bool testLoop(llvm::orc::JIT* JITCompiler)
{
    
	//Initialize Alive2
	llvm::Triple targetTriple(TheModule.get()->getTargetTriple());
	llvm::TargetLibraryInfoWrapperPass* TLI(targetTriple);
	
	
	//Set up output stream for Alive2 info, then set up smt
	out = &std::cout;
	smt_init.emplace();
    	
	return loopHelper(std::make_index_sequence<n>{}, JITCompiler, TLI);
}
*/
