#pragma once

// Copyright (c) 2021-present Stefan Mada
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: January 30, 2023

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>

// For Tester Namespace
#include "commandLineUtil.h"
#include "irGenerator.h"
#include "irWrapper.h"
#include "x86Intrin.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetOptions.h"


namespace Tester {

class JIT {
private:
  std::unique_ptr<llvm::orc::ExecutionSession> ES;

  llvm::DataLayout DL;
  llvm::orc::MangleAndInterner Mangle;

  llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;
  llvm::orc::IRCompileLayer CompileLayer;

  llvm::orc::JITDylib &MainJD;

public:
  JIT(std::unique_ptr<llvm::orc::ExecutionSession> ES, llvm::orc::JITTargetMachineBuilder JTMB,
      llvm::DataLayout DL)
      : ES(std::move(ES)), DL(std::move(DL)), Mangle(*this->ES, this->DL),
        ObjectLayer(*this->ES,
                    []() { return std::make_unique<SectionMemoryManager>(); }),
        CompileLayer(*this->ES, ObjectLayer,
                     std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JTMB))),
        MainJD(this->ES->createBareJITDylib("<main>")) {
    MainJD.addGenerator(
        cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            DL.getGlobalPrefix())));
  }

  ~JIT() {
    if (auto Err = ES->endSession())
      ES->reportError(std::move(Err));
  }

  static Expected<std::unique_ptr<JIT>> Create() {
    auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
    if (!EPC)
      return EPC.takeError();

    auto ES = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));
    auto triple = ES->getExecutorProcessControl().getTargetTriple();

    auto expectedMachineBuilder =
        llvm::orc::JITTargetMachineBuilder::detectHost();
    // Below executes if host couldn't be detected
    if (auto E = expectedMachineBuilder.takeError()) {
      errs() << "Problem with detecting host " << toString(std::move(E))
             << "\n";
      exit(-1);
    }

    llvm::orc::JITTargetMachineBuilder JTMB = *expectedMachineBuilder;

    auto DL = JTMB.getDefaultDataLayoutForTarget();
    if (!DL)
      return DL.takeError();

    return std::make_unique<JIT>(std::move(ES), std::move(JTMB),
                                 std::move(*DL));
  }

  const llvm::DataLayout &getDataLayout() const { return DL; }

  llvm::orc::JITDylib &getMainJITDylib() { return MainJD; }

  llvm::Error addModule(llvm::orc::ThreadSafeModule TSM, llvm::orc::ResourceTrackerSP RT = nullptr) {
    if (!RT)
      RT = MainJD.getDefaultResourceTracker();
    return CompileLayer.add(RT, std::move(TSM));
  }

  llvm::Expected<llvm::JITEvaluatedSymbol> lookup(llvm::StringRef Name) {
    return ES->lookup({&MainJD}, Mangle(Name.str()));
  }

  llvm::JITTargetAddress getFuncAddress(std::string str) {
    auto funcLookup = lookup(str);

    if (auto E = funcLookup.takeError()) {
      errs() << "Problem with JIT lookup " << toString(std::move(E)) << "\n";
      exit(-1);
    }

    return funcLookup->getAddress();
  }
};


std::unique_ptr<Tester::JIT> generateIntrinsicJIT() {
  // Initialize native target information for Just In Time Compiler
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();

  // Initialize Just In Time Compiler
  auto JITInit = Tester::JIT::Create();

  // Below executes if initializing JIT failed
  if (auto E = JITInit.takeError()) {
    llvm::errs() << "Problem with JIT " << toString(std::move(E)) << "\n";
    exit(-1);
  }
  std::unique_ptr<Tester::JIT> JITCompiler = std::move(*JITInit);

  // Set data layout of module
  TheModule->setDataLayout(JITCompiler->getDataLayout());

  // All intrinsic functions must be added below (probably in some for loop)
  for (unsigned i = 0; i < IR::X86IntrinBinOp::numOfX86Intrinsics; ++i) {
    llvm::Function *testFunc = llvm::Intrinsic::getDeclaration(
        TheModule.get(), TesterX86IntrinBinOp::intrin_id.at(i));
    generateCallFunctionFromFunction(testFunc, "func" + std::to_string(i));
  }

  // Debug printing for module
  if (CommandLineUtil::useDebugMode)
    TheModule->print(llvm::outs(), nullptr);

  // Add intrinsic module to JIT
  auto JITadd = JITCompiler->addModule(
      llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext)));

  // Check if adding module to JIT errored
  if (JITadd) {
    llvm::errs() << "Problem adding module to JIT " << JITadd << "\n";
    exit(-1);
  }
  return JITCompiler;
}
} // end namespace Tester
