declare <4 x i16> @llvm.x86.mmx.padd.w(<4 x i16>, <4 x i16>)

define <4 x i16> @src() {
  %1 = call <4 x i16> @llvm.x86.mmx.padd.w(<4 x i16> <i16 127, i16 -127, i16 -127, i16 4>, <4 x i16> <i16 0, i16 -4, i16 6, i16 0>)
  ret <4 x i16> %1
}

define <4 x i16> @tgt() {
  ret <4 x i16> <i16 127, i16 -131, i16 -121, i16 4>
}
