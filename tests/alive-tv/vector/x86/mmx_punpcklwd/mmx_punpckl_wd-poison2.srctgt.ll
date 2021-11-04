declare <4 x i16> @llvm.x86.mmx.punpcklwd(<4 x i16>, <4 x i16>)

define <4 x i16> @src(<4 x i16> %a) {
  %1 = call <4 x i16> @llvm.x86.mmx.punpcklwd(<4 x i16> %a, <4 x i16> <i16 1, i16 2, i16 poison, i16 poison>)
  ret <4 x i16> %1
}

define <4 x i16> @tgt(<4 x i16> %a) {
  %tmp = shufflevector <4 x i16> %a, <4 x i16> <i16 1, i16 2, i16 3, i16 4>, <4 x i32> <i32 0, i32 4, i32 1, i32 5>
  ret <4 x i16> %tmp
}
