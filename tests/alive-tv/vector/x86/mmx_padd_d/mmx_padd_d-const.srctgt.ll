declare <2 x i32> @llvm.x86.mmx.padd.d(<2 x i32>, <2 x i32>)

define <2 x i32> @src() {
  %1 = call <2 x i32> @llvm.x86.mmx.padd.d(<2 x i32> <i32 127, i32 -127>, <2 x i32> <i32 0, i32 -4>)
  ret <2 x i32> %1
}

define <2 x i32> @tgt() {
  ret <2 x i32> <i32 127, i32 -131>
}
