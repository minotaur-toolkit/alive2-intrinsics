declare <4 x i16> @llvm.x86.mmx.punpckhwd(<4 x i16>, <4 x i16>)

define <4 x i16> @src(<4 x i16> %a) {
  %1 = call <4 x i16> @llvm.x86.mmx.punpckhwd(<4 x i16> %a, <4 x i16> zeroinitializer)
  ret <4 x i16> %1
}

define <4 x i16> @tgt(<4 x i16> %a) {
  %tmp = shufflevector <4 x i16> %a, <4 x i16> zeroinitializer, <4 x i32> <i32 2, i32 6, i32 3, i32 7>
  ret <4 x i16> %tmp

}
