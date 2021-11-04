declare <8 x i8> @llvm.x86.mmx.punpckhbw(<8 x i8>, <8 x i8>)

define <8 x i8> @src(<8 x i8> %a, <8 x i8> %b) {
  %1 = call <8 x i8> @llvm.x86.mmx.punpckhbw(<8 x i8> %a, <8 x i8> %b)
  ret <8 x i8> %1
}

define <8 x i8> @tgt(<8 x i8> %a, <8 x i8> %b) {
  %tmp = shufflevector <8 x i8> %a, <8 x i8> %b, <8 x i32> <i32 4, i32 12, i32 1, i32 13, i32 6, i32 14, i32 7, i32 15>
  ret <8 x i8> %tmp
}

; ERROR: Target is more poisonous than source
