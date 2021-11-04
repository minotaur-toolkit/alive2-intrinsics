declare <4 x i16> @llvm.x86.mmx.punpcklwd(<4 x i16>, <4 x i16>)

define <4 x i16> @src(<4 x i16> %a) {
  %1 = add <4 x i16> %a, poison
  %2 = call <4 x i16> @llvm.x86.mmx.punpcklwd(<4 x i16> %1, <4 x i16> poison)
  ret <4 x i16> %2
}

define <4 x i16> @tgt(<4 x i16> %a) {
  ret <4 x i16> poison
}
