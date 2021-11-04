declare <8 x i8> @llvm.x86.mmx.punpcklbw(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %a) {
  %1 = add <8 x i8> %a, poison
  %2 = call <8 x i8> @llvm.x86.mmx.punpcklbw(<8 x i8> %1, <8 x i8> poison)
  ret <8 x i8> %2
}

define <8 x i8> @tgt(<8 x i8> %a) {
  ret <8 x i8> poison
}
