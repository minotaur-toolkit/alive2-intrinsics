target datalayout = "e-m:m-p:40:64:64:32-i32:32-i16:16-i8:8-n32"

define <2 x ptr> @f(<2 x ptr> %x) {
  %y = getelementptr i8, <2 x ptr> %x, <2 x i64> <i64 0, i64 1>
  %z = getelementptr i8, <2 x ptr> %y, <2 x i64> <i64 1, i64 0>
  ret <2 x ptr> %z
}

; from Transforms/InstCombine/gep-custom-dl.ll
%S = type { i32, [ 100 x i32] }

define <2 x i1> @test6b(<2 x i32> %X, <2 x ptr> %P) {
  %A = getelementptr inbounds %S, <2 x ptr> %P, i32 0, i32 1, <2 x i32> %X
  %B = getelementptr inbounds %S, <2 x ptr> %P, i32 0, i32 0
  %C = icmp eq <2 x ptr> %A, %B
  ret <2 x i1> %C
}

; from Transforms/InstCombine/gep-vector.ll
@block = global [64 x [8192 x i8]] zeroinitializer, align 1

define <2 x ptr> @vectorindex1() {
  %1 = getelementptr inbounds [64 x [8192 x i8]], ptr @block, i64 0, <2 x i64> <i64 0, i64 1>, i64 8192
  ret <2 x ptr> %1
}
