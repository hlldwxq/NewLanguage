; what are in map:
; 0x249da70
; what we are finding:
; 0x249dc38
; ModuleID = 'test.q'
source_filename = "test.q"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"

@0 = private unnamed_addr constant [22 x i8] c"overflow at line: %d\0A\00", align 1

define void @test() {
entry:
  %a = alloca i8*
  %overflowtmp = call { i64, i1 } @llvm.umul.with.overflow.i64.i64(i64 3, i64 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i64))
  %0 = extractvalue { i64, i1 } %overflowtmp, 1
  br i1 %0, label %overflow, label %normal

overflow:                                         ; preds = %entry
  %1 = call i64 @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), i64 2)
  call void @exit(i64 1)
  unreachable

normal:                                           ; preds = %entry
  %2 = tail call i8* @malloc(i64 mul (i64 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i64), i64 3))
  store i8* %2, i8** %a
  %3 = load i8*, i8** %a
  %4 = getelementptr i8, i8* %3, i8 0
  store i8 1, i8* %4
  ret void
}

; Function Attrs: nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.umul.with.overflow.i64.i64(i64, i64) #0

declare i64 @printf(i8*, i64)

declare void @exit(i64)

declare noalias i8* @malloc(i64)

attributes #0 = { nounwind readnone speculatable willreturn }
