// Copyright (c) 2021-present The Alive2 Authors.
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: February 23, 2022

#include "x86Intrin.h"

std::array<llvm::Intrinsic::ID, IR::X86IntrinBinOp::numOfX86Intrinsics>
    TesterX86IntrinBinOp::intrin_id = {
#define PROCESS(NAME,A,B,C,D,E,F) llvm::Intrinsic::NAME,
#include "ir/intrinsics.h"
#undef PROCESS
};
