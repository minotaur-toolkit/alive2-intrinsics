declare void @llvm.memset.p0i8.i128(ptr, i8, i128, i1)

define void @f(ptr %p, i128 %n) {
  %n2 = add nuw i128 %n, 1
  call void @llvm.memset.p0i8.i128(ptr %p, i8 0, i128 %n2, i1 0)
  ret void
}
