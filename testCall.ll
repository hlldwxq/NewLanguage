; what are in map:
; 0xece880
; what we are finding:
; 0xecf1d8
; ModuleID = 'testCall.q'
source_filename = "testCall.q"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"

@0 = private unnamed_addr constant [22 x i8] c"overflow at line: %d\0A\00", align 1

define i8* @helper() {
entry:
  %overflowtmp = call { i64, i1 } @llvm.umul.with.overflow.i64.i64(i64 3, i64 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i64))
  %0 = extractvalue { i64, i1 } %overflowtmp, 1
  br i1 %0, label %overflow, label %normal

overflow:                                         ; preds = %entry
  %1 = call i64 @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), i64 2)
  call void @exit(i64 1)
  unreachable

normal:                                           ; preds = %entry
  %new = tail call i8* @malloc(i64 mul (i64 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i64), i64 3))
  ret i8* %new
}

; Function Attrs: nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.umul.with.overflow.i64.i64(i64, i64) #0

declare i64 @printf(i8*, i64)

declare void @exit(i64)

declare noalias i8* @malloc(i64)

define void @test() {
entry:
  %a = alloca i8*
  %calltmp = call i8* @helper()
  %0 = ptrtoint i8* %calltmp to i64
  %1 = ptrtoint i8* %new to i64
  %2 = sub i64 %0, %1
  %cmpValue = sdiv exact i64 %2, ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i64)
  store i8* %calltmp, i8** %a
  %3 = load i8*, i8** %a
  %4 = getelementptr i8, i8* %3, i8 0
  store i8 1, i8* %4
  ret void
}

attributes #0 = { nounwind readnone speculatable willreturn }
