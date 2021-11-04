declare <2 x i32> @llvm.x86.mmx.punpckldq(<2 x i32>, <2 x i32>)

define <2 x i32> @src(<2 x i32> %a) {
  %1 = call <2 x i32> @llvm.x86.mmx.punpckldq(<2 x i32> %a, <2 x i32> zeroinitializer)
  ret <2 x i32> %1
}

define <2 x i32> @tgt(<2 x i32> %a) {
  %tmp = shufflevector <2 x i32> %a, <2 x i32> zeroinitializer, <2 x i32> <i32 1, i32 2>
  ret <2 x i32> %tmp
}

; ERROR: Target is more poisonous than source
