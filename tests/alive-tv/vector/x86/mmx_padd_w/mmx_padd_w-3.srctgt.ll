declare <4 x i16> @llvm.x86.mmx.padd.w(<4 x i16>, <4 x i16>)

define <4 x i16> @src(<4 x i16> %v) {
  %1 = call <4 x i16> @llvm.x86.mmx.padd.w(<4 x i16> %v, <4 x i16> <i16 3, i16 3, i16 3, i16 3>)
  ret <4 x i16> %1
}

define <4 x i16> @tgt(<4 x i16> %v) {
  %tmp = add <4 x i16> %v, <i16 5, i16 5, i16 5, i16 5>
  ret <4 x i16> %tmp
}

; ERROR: Value mismatch
