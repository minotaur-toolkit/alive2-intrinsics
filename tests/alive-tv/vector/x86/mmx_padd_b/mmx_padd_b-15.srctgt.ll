declare <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %v) {
  %1 = call <8 x i8> @llvm.x86.mmx.padd.b(<8 x i8> %v, <8 x i8> <i8 15, i8 15, i8 15, i8 15, i8 15, i8 15, i8 15, i8 15>)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %v) {
  %tmp = add <8 x i8> %v, <i8 15, i8 15, i8 15, i8 15, i8 15, i8 15, i8 15, i8 15>
  ret <8 x i8> %tmp
}
