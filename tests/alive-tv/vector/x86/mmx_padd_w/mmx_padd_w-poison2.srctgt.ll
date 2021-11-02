declare <4 x i16> @llvm.x86.mmx.padd.w(<4 x i16>, <4 x i16>)

define <4 x i16> @src(<4 x i16> %v) {
  %1 = call <4 x i16> @llvm.x86.mmx.padd.w(<4 x i16> %v, <4 x i16> poison)
  ret <4 x i16> %1
}

define <4 x i16> @tgt(<4 x i16> %v) {
  ret <4 x i16> poison
}
