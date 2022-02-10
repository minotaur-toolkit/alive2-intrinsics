#include "vectorutil.hpp"
#include "x86Intrin.hpp"
#include "randomizer.hpp"
#include "irGenerator.hpp"
#include "irWrapper.hpp"
#include "JIT.hpp"
#include "compareFunctions.cpp"
#include "testLoop.hpp"

int main()
{		
  //Initialize llvm module and intrinsic module
  InitializeModule();
  
  std::unique_ptr<llvm::orc::JIT> JITCompiler = Tester::generateIntrinsicJIT();
  
  switchToAliveContext();
  
  //Initialize Alive2
  llvm::Triple targetTriple(TheModule.get()->getTargetTriple());
  llvm::TargetLibraryInfoWrapperPass TLI(targetTriple);
  
  //Set up output stream for Alive2 info, then set up smt
  out = &std::cout;
  smt_init.emplace();
  
  // Loop that evaluates every intrinsic
  static_for<IR::X86IntrinBinOp::numOfX86Intrinsics>([&](auto index) 
  {
    if constexpr(index >= 9 && index < 18) // Ignores MMX instructions
      return;
    else // If intrinsic is supported
    {
      constexpr IR::X86IntrinBinOp::Op op = getOp<index.value>();	
      
      constexpr unsigned timesToLoop = 100;	
      
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
      	std::conditional_t<retBitSize != 512, std::conditional_t<retBitSize == 256, __m256i, __m128i>, __m512i>
      	(*opFunctionType)
      	(std::conditional_t<op0BitSize != 512, std::conditional_t<op0BitSize == 256, __m256i, __m128i>, __m512i>,
      	std::conditional_t<op1BitSize != 512, std::conditional_t<op1BitSize == 256, __m256i, std::conditional_t<op1BitSize == 32, int32_t, __m128i>>, __m512i>);
      
      auto* funcPointer = reinterpret_cast<opFunctionType>(JITCompiler->getFuncAddress("func" + std::to_string(index.value)));
      //This jank might truly be real honest to god undefined behavior above, someone help	
      
      //Declare the intrinsic to be used
      llvm::Function* intrinsicFunction = llvm::Intrinsic::getDeclaration(TheModule.get(), TesterX86IntrinBinOp::intrin_id.at((int) op));
      
      //Loop that tests for the equality of both functions for equal inputs	
      for(int i = 0; i != timesToLoop; ++i) {
      	vals = vectorRandomizer<op0Bitwidth>(vals);
	if constexpr(op1BitSize == 32)
	{
          vals2 = integerRandomizer<40>();
	}
	else
	{
          vals2 = vectorRandomizer<op1Bitwidth, 40>(vals2);
	}
      	retVec = funcPointer(vals, vals2);
      
      	llvm::Function* tgtFunc = generateReturnFunction<retBitwidth>(retVec, "tgt");
      	llvm::Function* srcFunc = generateCallFunction<op0Bitwidth, op1Bitwidth>(vals, vals2, intrinsicFunction, "src");
      	compareFunctions(*srcFunc, *tgtFunc, TLI);
      	// Print LLVM IR
      	srcFunc->print(outs());
      	tgtFunc->print(outs());
      
      	tgtFunc->eraseFromParent();
      	srcFunc->eraseFromParent();
      }
      
      std::cout << "Ran " << timesToLoop << " tests." << 
      	"\nNum correct: " << num_correct << 
      	"\nNum unsound: " << num_unsound << 
      	"\nNum failed: " << num_failed << 
      	"\nNum errors: " << num_errors << "\n";	
      
    }
  });	
  
  return 0;
}

