declare <8 x i8> @llvm.x86.mmx.punpckhbw(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %a) {
  %1 = call <8 x i8> @llvm.x86.mmx.punpckhbw(<8 x i8> %a, <8 x i8> <i8 poison, i8 poison, i8 poison, i8 poison, i8 5, i8 6, i8 7, i8 8>)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %a) {
  %tmp = shufflevector <8 x i8> %a, <8 x i8> <i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8>, <8 x i32> <i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  ret <8 x i8> %tmp
}
