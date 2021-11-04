declare <8 x i8> @llvm.x86.mmx.punpcklbw(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %a) {
  %1 = call <8 x i8> @llvm.x86.mmx.punpcklbw(<8 x i8> %a, <8 x i8> <i8 1, i8 2, i8 poison, i8 poison, i8 5, i8 6, i8 7, i8 8>)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %a) {
  %tmp = shufflevector <8 x i8> %a, <8 x i8> <i8 1, i8 2, i8 poison, i8 poison, i8 5, i8 6, i8 7, i8 8>, <8 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11>
  ret <8 x i8> %tmp
}
