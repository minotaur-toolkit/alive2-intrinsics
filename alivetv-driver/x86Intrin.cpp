#include "x86Intrin.hpp"


std::array<llvm::Intrinsic::ID, IR::X86IntrinBinOp::numOfX86Intrinsics> TesterX86IntrinBinOp::intrin_id = {
  llvm::Intrinsic::x86_sse2_pavg_w,
  llvm::Intrinsic::x86_sse2_pavg_b,
  llvm::Intrinsic::x86_avx2_pavg_w,
  llvm::Intrinsic::x86_avx2_pavg_b,
  llvm::Intrinsic::x86_avx512_pavg_w_512,
  llvm::Intrinsic::x86_avx512_pavg_b_512,
  llvm::Intrinsic::x86_avx2_pshuf_b,
  llvm::Intrinsic::x86_ssse3_pshuf_b_128,
  llvm::Intrinsic::x86_mmx_padd_b,
  llvm::Intrinsic::x86_mmx_padd_w,
  llvm::Intrinsic::x86_mmx_padd_d,
  llvm::Intrinsic::x86_mmx_punpckhbw,
  llvm::Intrinsic::x86_mmx_punpckhwd,
  llvm::Intrinsic::x86_mmx_punpckhdq,
  llvm::Intrinsic::x86_mmx_punpcklbw,
  llvm::Intrinsic::x86_mmx_punpcklwd,
  llvm::Intrinsic::x86_mmx_punpckldq,
  llvm::Intrinsic::x86_sse2_psrl_w,

};

