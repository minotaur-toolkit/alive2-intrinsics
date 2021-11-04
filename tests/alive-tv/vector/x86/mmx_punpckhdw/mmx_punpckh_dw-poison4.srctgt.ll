declare <2 x i32> @llvm.x86.mmx.punpckhdq(<2 x i32>, <2 x i32>)

define <2 x i32> @src(<2 x i32> %a) {
  %1 = add <2 x i32> %a, poison
  %2 = call <2 x i32> @llvm.x86.mmx.punpckhdq(<2 x i32> %1, <2 x i32> poison)
  ret <2 x i32> %2
}

define <2 x i32> @tgt(<2 x i32> %a) {
  ret <2 x i32> poison
}
