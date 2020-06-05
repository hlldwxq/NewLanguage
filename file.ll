; ModuleID = 'file.k'
source_filename = "file.k"

@t = common global i64* null
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 1, void ()* @init_all_global_array, i8* null }]

define internal void @init_global_array_t() {
entry:
  %0 = zext i8 3 to i64
  %mallocsize = mul i64 %0, ptrtoint (i64* getelementptr (i64, i64* null, i32 1) to i64)
  %malloccall = tail call i8* @malloc(i64 %mallocsize)
  %1 = bitcast i8* %malloccall to i64*
  store i64* %1, i64** @t
  ret void
}

declare noalias i8* @malloc(i64)

define i64 @ValidTest1(i8 %var1, i64 %var2) {
entry:
  %0 = alloca i64
  %var11 = alloca i8
  store i8 %var1, i8* %var11
  %var22 = alloca i64
  store i64 %var2, i64* %var22
  %1 = load i8, i8* %var11
  %2 = sext i8 %1 to i16
  %addtmp = add i16 129, %2
  %3 = sext i16 %addtmp to i64
  store i64 %3, i64* %var22
  %4 = load i8, i8* %var11
  %addtmp3 = add i8 2, %4
  %5 = sext i8 %addtmp3 to i64
  store i64 %5, i64* %var22
  %6 = load i8, i8* %var11
  %7 = sext i8 %6 to i16
  %subtmp = sub i16 %7, 128
  %8 = sext i16 %subtmp to i64
  store i64 %8, i64* %var22
  %9 = load i8, i8* %var11
  %subtmp4 = sub i8 %9, 2
  %10 = sext i8 %subtmp4 to i64
  store i64 %10, i64* %var22
  store i64 1152, i64* %var22
  %11 = load i64, i64* %var22
  %12 = load i8, i8* %var11
  %13 = sext i8 %12 to i64
  %addtmp5 = add i64 %11, %13
  store i64 %addtmp5, i64* %var22
  %var3 = alloca i32
  store i32 -45, i32* %var3
  %var4 = alloca i32
  %14 = load i8, i8* %var11
  %15 = sext i8 %14 to i32
  store i32 %15, i32* %var4
  %16 = load i32, i32* %var3
  %17 = sext i32 %16 to i64
  store i64 %17, i64* %0
  %18 = load i64, i64* %0
  ret i64 %18
}

define i64 @ValidTest2() {
entry:
  %0 = alloca i64
  %var1 = alloca i32
  store i32 78, i32* %var1
  %var2 = alloca i64
  %1 = load i32, i32* %var1
  %2 = sext i32 %1 to i64
  store i64 %2, i64* %var2
  %arg = alloca i32
  store i32 20, i32* %arg
  %3 = load i32, i32* %arg
  %4 = sext i32 %3 to i64
  %calltmp = call i64 @ValidTest1(i8 6, i64 %4)
  store i64 6, i64* %0
  %5 = load i64, i64* %0
  ret i64 %5
}

define i16 @InValidTest() {
entry:
  %0 = alloca i16
  store i16 0, i16* %0
  %1 = load i16, i16* %0
  ret i16 %1
}

define internal void @init_all_global_array() {
entry:
  call void @init_global_array_t()
  ret void
}
