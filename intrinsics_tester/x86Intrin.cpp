// Copyright (c) 2021-present Stefan Mada
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: January 30, 2023

#include "x86Intrin.h"

std::array<llvm::Intrinsic::ID, IR::X86IntrinBinOp::numOfX86Intrinsics>
    TesterX86IntrinBinOp::intrin_id = {
#define PROCESS(NAME,A,B,C,D,E,F) llvm::Intrinsic::NAME,
#include "ir/intrinsics.h"
#undef PROCESS
};
