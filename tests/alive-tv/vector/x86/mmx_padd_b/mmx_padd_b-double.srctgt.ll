; TEST-ARGS: -disable-undef-input
; disabled undef inputs to accelerate the verification

declare <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %v) {
  %1 = call <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8> %v, <8 x i8> %v)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %v) {
  %tmp = shl <8 x i8> %v, <i8 1, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1>
  ret <8 x i8> %tmp
}
