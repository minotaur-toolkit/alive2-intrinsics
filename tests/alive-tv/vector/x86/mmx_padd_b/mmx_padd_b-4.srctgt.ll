declare <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %v) {
  %1 = call <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8> %v, <8 x i8> <i8 4, i8 8, i8 12, i8 16, i8 20, i8 24, i8 28, i8 32>)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %v) {
  %tmp = add <8 x i8> %v, <i8 0, i8 4, i8 12, i8 16, i8 20, i8 24, i8 28, i8 32>
  ret <8 x i8> %tmp
}

; ERROR: Value mismatch
