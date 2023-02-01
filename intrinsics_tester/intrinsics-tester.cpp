// Copyright (c) 2021-present Stefan Mada.
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: January 30, 2023

#include "JIT.h"
#include "commandLineUtil.h"
#include "compareFunctions.cpp"
#include "irGenerator.h"
#include "irWrapper.h"
#include "progressBar.h"
#include "randomizer.h"
#include "testLoop.h"
#include "vectorWrapper.h"
#include "vectorutil.h"
#include "x86Intrin.h"
#include <chrono>

using namespace std;
using namespace Tester;
using namespace llvm;

int main(int argc, char **argv) {
  CommandLineUtil commandLineUtil(argc, argv);

  bool enableDebug = CommandLineUtil::useDebugMode;
  unsigned lowerBound = CommandLineUtil::lowerInclusiveBound;
  unsigned upperBound = CommandLineUtil::upperExclusiveBound;
  // Initialize llvm module and intrinsic module
  InitializeModule();

  unique_ptr<JIT> JITCompiler = generateIntrinsicJIT();

  switchToAliveContext();

  // Initialize Alive2
  Triple targetTriple(TheModule.get()->getTargetTriple());
  TargetLibraryInfoWrapperPass TLI(targetTriple);

  // Set up output stream for Alive2 info, then set up smt
  out = &cout;
  smt_init.emplace();

  // Loop that evaluates every intrinsic
  constexpr unsigned loopCount = IR::X86IntrinBinOp::numOfX86Intrinsics;
  uint64_t numberOfTestsPerformed = 0;
  uint64_t numberOfIntrinsicsTested = 0;
  ProgressBar progressBar(lowerBound, upperBound); // Subtract MMX instructions

  auto start = chrono::steady_clock::now();

  static_for<loopCount>([&](auto index) {
    if (index < lowerBound || index >= upperBound)
      return;
    else // If intrinsic is in range
    {
      constexpr IR::X86IntrinBinOp::Op op = getOp<index.value>();

      const unsigned timesToLoop = CommandLineUtil::numberOfRepetitions;

      // Bitsize is the number of bits in the entire vector
      constexpr unsigned op0BitSize = bitSizeOp0<op>();
      constexpr unsigned op1BitSize = bitSizeOp1<op>();
      constexpr unsigned retBitSize = bitSizeRet<op>();

      // Bitwidth is the number of bits in a lane
      constexpr unsigned op0Bitwidth = bitwidthOp0<op>();
      constexpr unsigned op1Bitwidth = bitwidthOp1<op>();
      constexpr unsigned retBitwidth = bitwidthRet<op>();

      // Code below initializes the vectors to 0 with the proper types for the
      // operation
      auto vals = get<vectorTypeIndex<op0BitSize>()>(
          returnVectorType<op0BitSize>());
      auto vals2 = get<vectorTypeIndex<op1BitSize>()>(
          returnVectorType<op1BitSize>());
      auto retVec = get<vectorTypeIndex<retBitSize>()>(
          returnVectorType<retBitSize>());

      // Defines the function pointer type that the function should use,
      // ie auomatically chooses return, op1, and op2 types
      using returnType = conditional_t<
          retBitSize != 512,
          conditional_t<retBitSize == 256, __m256i, __m128i>, __m512i>;
      using op0Type = conditional_t<
          op0BitSize != 512,
          conditional_t<op0BitSize == 256, __m256i, __m128i>, __m512i>;
      using op1Type = conditional_t<
          op1BitSize != 512,
          conditional_t<
              op1BitSize == 256, __m256i,
              conditional_t<op1BitSize == 32, int32_t, __m128i>>,
          __m512i>;

      typedef returnType (*opFunctionType)(op0Type, op1Type);

      auto *funcPointer = reinterpret_cast<opFunctionType>(
          JITCompiler->getFuncAddress("func" + to_string(index.value)));

      // Declare the intrinsic to be used
      Function *intrinsicFunction = Intrinsic::getDeclaration(
          TheModule.get(), TesterX86IntrinBinOp::intrin_id.at((int)op));

      // Map that counts how often certain end results are reached
      unordered_map<VectorWrapper<retBitwidth, retBitSize, returnType>,
                    int, VectorWrapperHashFn>
          resultMap;

      // Loop that tests for the equality of both functions for equal inputs
      for (unsigned i = 0; i != timesToLoop; ++i) {
        vals = vectorRandomizer<op0Bitwidth>(vals);
        if constexpr (op1BitSize == 32) {
          vals2 = integerRandomizer();
        } else {
          vals2 = vectorRandomizer<op1Bitwidth>(vals2);
        }

        retVec = funcPointer(vals, vals2);

        Function *tgtFunc =
            generateReturnFunction<retBitwidth>(retVec, "tgt");
        Function *srcFunc =
            generateCallFunction<op0Bitwidth, op1Bitwidth>(
                vals, vals2, intrinsicFunction, "src");

        const unsigned currentNumCorrect = num_correct;
        compareFunctions(*srcFunc, *tgtFunc, TLI);

        // Print LLVM IR if failed test
        if (currentNumCorrect + 1 != num_correct) {
          srcFunc->print(outs());
          tgtFunc->print(outs());
        }

        if (enableDebug) {
          VectorWrapper<retBitwidth, retBitSize, returnType> wrappedVec(retVec);
          if (resultMap.contains(wrappedVec))
            resultMap[wrappedVec] = resultMap[wrappedVec] + 1;
          else
            resultMap[wrappedVec] = 0;
        }

        tgtFunc->eraseFromParent();
        srcFunc->eraseFromParent();
        ++numberOfTestsPerformed;
      }
      if (enableDebug) {
        cout << "Number of repetitions for func" << index.value << ": ";
        int numberOfRepetitions = 0;
        VectorWrapper<retBitwidth, retBitSize, returnType> mostCommonVec;
        int mostNumberOfAppearances = 0;
        for (auto it : resultMap) {
          numberOfRepetitions += it.second;
          if (it.second > mostNumberOfAppearances) {
            mostNumberOfAppearances = it.second;
            mostCommonVec = it.first;
          }
        }
        cout << numberOfRepetitions;
        if (numberOfRepetitions > 0) {
          cout << " | Vector: ";
          mostCommonVec.printVec();
        }
        cout << "\n";
      }

      ++numberOfIntrinsicsTested;
      if (!enableDebug)
        progressBar.update(numberOfIntrinsicsTested);
    }
  });

  auto end = chrono::steady_clock::now();
  auto minutesTaken =
       chrono::duration_cast<chrono::minutes>(end - start);

  cout << "\nRan " << numberOfTestsPerformed << " tests on "
       << numberOfIntrinsicsTested << " intrinsics."
       << "\nNumber of minutes taken: " << minutesTaken.count()
       << "\nNum correct: " << num_correct
       << "\nNum unsound: " << num_unsound
       << "\nNum failed: " << num_failed << "\nNum errors: " << num_errors
       << "\n";

  return 0;
}
