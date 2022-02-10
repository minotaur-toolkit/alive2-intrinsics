//===- KaleidoscopeJIT.h - A simple JIT for Kaleidoscope --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Contains a simple JIT definition for use in the kaleidoscope tutorials.
//
//===----------------------------------------------------------------------===//

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
#include "llvm/Support/TargetSelect.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "x86Intrin.hpp"

#ifndef JIT_H
#define JIT_H

namespace llvm {
namespace orc {

class JIT {
private:
  std::unique_ptr<ExecutionSession> ES;

  DataLayout DL;
  MangleAndInterner Mangle;

  RTDyldObjectLinkingLayer ObjectLayer;
  IRCompileLayer CompileLayer;

  JITDylib &MainJD;

public:
  JIT(std::unique_ptr<ExecutionSession> ES,
                  JITTargetMachineBuilder JTMB, DataLayout DL)
      : ES(std::move(ES)), DL(std::move(DL)), Mangle(*this->ES, this->DL),
        ObjectLayer(*this->ES,
                    []() { return std::make_unique<SectionMemoryManager>(); }),
        CompileLayer(*this->ES, ObjectLayer,
                     std::make_unique<ConcurrentIRCompiler>(std::move(JTMB))),
        MainJD(this->ES->createBareJITDylib("<main>")) {
    MainJD.addGenerator(
        cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(
            DL.getGlobalPrefix())));
  }

  ~JIT() {
    if (auto Err = ES->endSession())
      ES->reportError(std::move(Err));
  }

  static Expected<std::unique_ptr<JIT>> Create() {
    auto EPC = SelfExecutorProcessControl::Create();
    if (!EPC)
      return EPC.takeError();

    auto ES = std::make_unique<ExecutionSession>(std::move(*EPC));
    auto triple = ES->getExecutorProcessControl().getTargetTriple();

    auto expectedMachineBuilder = llvm::orc::JITTargetMachineBuilder::detectHost();
    //Below executes if host couldn't be detected
    if (auto E = expectedMachineBuilder.takeError()) {
      errs() << "Problem with detecting host " << toString(std::move(E)) << "\n";
      exit(-1);
    }

    JITTargetMachineBuilder JTMB = *expectedMachineBuilder;

    auto DL = JTMB.getDefaultDataLayoutForTarget();
    if (!DL)
      return DL.takeError();

    return std::make_unique<JIT>(std::move(ES), std::move(JTMB),
                                             std::move(*DL));
  }

  const DataLayout &getDataLayout() const { return DL; }

  JITDylib &getMainJITDylib() { return MainJD; }

  Error addModule(ThreadSafeModule TSM, ResourceTrackerSP RT = nullptr) {
    if (!RT)
      RT = MainJD.getDefaultResourceTracker();
    return CompileLayer.add(RT, std::move(TSM));
  }

  Expected<JITEvaluatedSymbol> lookup(StringRef Name) {
    return ES->lookup({&MainJD}, Mangle(Name.str()));
  }

  JITTargetAddress getFuncAddress(std::string str)
  {
    auto funcLookup = lookup(str);

    if (auto E = funcLookup.takeError()) {
      errs() << "Problem with JIT lookup " << toString(std::move(E)) << "\n";
      exit(-1);
    }
    
    return funcLookup->getAddress();
  } 
};

} // end namespace orc
} // end namespace llvm

namespace Tester
{
  std::unique_ptr<llvm::orc::JIT> generateIntrinsicJIT()
  {
    //Initialize native target information for Just In Time Compiler        
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    
    //Initialize Just In Time Compiler
    auto JITInit = llvm::orc::JIT::Create();
    
    //Below executes if initializing JIT failed
    if (auto E = JITInit.takeError()) {
      errs() << "Problem with JIT " << toString(std::move(E)) << "\n";
      exit(-1);
    }
    std::unique_ptr<llvm::orc::JIT> JITCompiler = std::move(*JITInit);
    
    //Set data layout of module
    TheModule->setDataLayout(JITCompiler->getDataLayout());
    
    // All intrinsic functions must be added below (probably in some for loop)      
    for(unsigned i = 0; i < IR::X86IntrinBinOp::numOfX86Intrinsics; ++i)
    {
    	llvm::Function* testFunc = llvm::Intrinsic::getDeclaration(TheModule.get(), TesterX86IntrinBinOp::intrin_id.at(i));
    	generateCallFunctionFromFunction(testFunc, "func" + std::to_string(i));
    }
    
    //Debug printing for module
    //TheModule->print(outs(), nullptr);
    
    //Add intrinsic module to JIT
    auto JITadd = JITCompiler->addModule(llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext)));
    
    //Check if adding module to JIT errored
    if(JITadd) {
      errs() << "Problem adding module to JIT " << JITadd << "\n";
      exit(-1);
    }
    return JITCompiler;
  }
} // end namespace Tester

#endif 
