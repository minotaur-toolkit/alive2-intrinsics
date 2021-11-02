declare <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8>, <8 x i8>)

define <8 x i8> @src() {
  %1 = call <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8> <i8 127, i8 -127, i8 -127, i8 4, i8 5, i8 -6, i8 -7, i8 127>, <8 x i8> <i8 0, i8 -4, i8 6, i8 0, i8 0, i8 -12, i8 14, i8 0>)
  ret <8 x i8> %1
}

define <8 x i8> @tgt() {
  ret <8 x i8> <i8 127, i8 125, i8 -121, i8 4, i8 5, i8 -18, i8 7, i8 127>
}
