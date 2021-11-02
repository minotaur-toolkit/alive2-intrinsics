declare <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %v) {
  %1 = call <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8> %v, <8 x i8> <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %v) {
  %tmp = add <8 x i8> %v, <i8 5, i8 5, i8 5, i8 5, i8 5, i8 5, i8 5, i8 5>
  ret <8 x i8> %tmp
}

; ERROR: Value mismatch
