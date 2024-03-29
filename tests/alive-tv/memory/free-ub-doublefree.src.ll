target datalayout="e-p:64:64"

define i8 @free_ub_doublefree() {
  %ptr = call noalias ptr @malloc(i64 4)
  ; %ptr == null?
  %i = ptrtoint ptr %ptr to i64
  %eq = icmp eq i64 %i, 0
  br i1 %eq, label %EXIT, label %DOUBLE_FREE
EXIT:
  ; If %ptr == null, double-free is okay.
  ret i8 0
DOUBLE_FREE:
  call void @free(ptr %ptr)
  call void @free(ptr %ptr)
  ret i8 1
}

declare noalias ptr @malloc(i64)
declare void @free(ptr)
