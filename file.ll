; ModuleID = 'file.k'
source_filename = "file.k"

define i64 @snumAndVarPara(i8 %var1) {
entry:
  %0 = alloca i64
  %var11 = alloca i8
  store i8 %var1, i8* %var11
  %var2 = alloca i64
  store i64 0, i64* %var2
  %1 = load i8, i8* %var11
  %2 = sext i8 %1 to i16
  %addtmp = add i16 128, %2
  %3 = sext i16 %addtmp to i64
  store i64 %3, i64* %var2
  %4 = load i64, i64* %var2
  %addtmp2 = add i64 -128, %4
  store i64 %addtmp2, i64* %var2
  %5 = load i64, i64* %var2
  store i64 %5, i64* %0
  %6 = load i64, i64* %0
  ret i64 %6
}

define i64 @svarAndNumPara(i8 %var1) {
entry:
  %0 = alloca i64
  %var11 = alloca i8
  store i8 %var1, i8* %var11
  %var2 = alloca i64
  store i64 0, i64* %var2
  %1 = load i8, i8* %var11
  %2 = sext i8 %1 to i16
  %subtmp = sub i16 %2, 128
  %3 = sext i16 %subtmp to i64
  store i64 %3, i64* %var2
  %4 = load i64, i64* %var2
  %subtmp2 = sub i64 %4, -128
  store i64 %subtmp2, i64* %var2
  %5 = load i64, i64* %var2
  store i64 %5, i64* %0
  %6 = load i64, i64* %0
  ret i64 %6
}

define i64 @snumAndNum128() {
entry:
  %0 = alloca i64
  store i64 128, i64* %0
  %1 = load i64, i64* %0
  ret i64 %1
}

define i64 @svarAndVarPara(i64 %var2) {
entry:
  %0 = alloca i64
  %var21 = alloca i64
  store i64 %var2, i64* %var21
  %var1 = alloca i16
  store i16 -32768, i16* %var1
  %1 = load i64, i64* %var21
  %2 = load i16, i16* %var1
  %3 = sext i16 %2 to i64
  %subtmp = sub i64 %1, %3
  store i64 %subtmp, i64* %var21
  %4 = load i16, i16* %var1
  %5 = load i64, i64* %var21
  %6 = sext i16 %4 to i64
  %addtmp = add i64 %6, %5
  store i64 %addtmp, i64* %var21
  %7 = load i64, i64* %var21
  store i64 %7, i64* %0
  %8 = load i64, i64* %0
  ret i64 %8
}

define i64 @snumAndNumAndVarPara(i16 %var1) {
entry:
  %0 = alloca i64
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i64
  %1 = load i16, i16* %var11
  %addtmp = add i16 0, %1
  %2 = sext i16 %addtmp to i64
  store i64 %2, i64* %var2
  %3 = load i64, i64* %var2
  store i64 %3, i64* %0
  %4 = load i64, i64* %0
  ret i64 %4
}

define i16 @sHelperFunc(i16 %var) {
entry:
  %0 = alloca i16
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  store i16 %1, i16* %0
  %2 = load i16, i16* %0
  ret i16 %2
}

define i64 @sCallFunBinPara(i16 %var1) {
entry:
  %0 = alloca i64
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i64
  %1 = load i16, i16* %var11
  %calltmp = call i16 @sHelperFunc(i16 %1)
  %2 = load i16, i16* %var11
  %subtmp = sub i16 %calltmp, %2
  %3 = load i16, i16* %var11
  %calltmp2 = call i16 @sHelperFunc(i16 %3)
  %addtmp = add i16 %subtmp, %calltmp2
  %4 = sext i16 %addtmp to i64
  store i64 %4, i64* %var2
  %5 = load i64, i64* %var2
  store i64 %5, i64* %0
  %6 = load i64, i64* %0
  ret i64 %6
}

define i32 @unumAndVarPara(i8 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i8
  store i8 %var1, i8* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  %1 = load i8, i8* %var11
  %2 = zext i8 %1 to i16
  %addtmp = add i16 256, %2
  %3 = zext i16 %addtmp to i32
  store i32 %3, i32* %var2
  %4 = load i32, i32* %var2
  %subtmp = sub i32 %4, 256
  store i32 %subtmp, i32* %var2
  %5 = load i32, i32* %var2
  store i32 %5, i32* %0
  %6 = load i32, i32* %0
  ret i32 %6
}

define i32 @uvarAndNumPara(i8 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i8
  store i8 %var1, i8* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  %1 = load i8, i8* %var11
  %2 = zext i8 %1 to i16
  %addtmp = add i16 %2, 256
  %3 = zext i16 %addtmp to i32
  store i32 %3, i32* %var2
  %4 = load i32, i32* %var2
  %subtmp = sub i32 %4, 256
  store i32 %subtmp, i32* %var2
  %5 = load i32, i32* %var2
  store i32 %5, i32* %0
  %6 = load i32, i32* %0
  ret i32 %6
}

define i16 @unumAndNum65535() {
entry:
  %0 = alloca i16
  store i16 -1, i16* %0
  %1 = load i16, i16* %0
  ret i16 %1
}

define i32 @uvarAndVarPara(i32 %var2) {
entry:
  %0 = alloca i32
  %var21 = alloca i32
  store i32 %var2, i32* %var21
  %var1 = alloca i16
  store i16 255, i16* %var1
  %1 = load i16, i16* %var1
  %2 = load i32, i32* %var21
  %3 = zext i16 %1 to i32
  %addtmp = add i32 %3, %2
  store i32 %addtmp, i32* %var21
  %4 = load i32, i32* %var21
  %5 = load i16, i16* %var1
  %6 = zext i16 %5 to i32
  %subtmp = sub i32 %4, %6
  store i32 %subtmp, i32* %var21
  %7 = load i32, i32* %var21
  store i32 %7, i32* %0
  %8 = load i32, i32* %0
  ret i32 %8
}

define i32 @unumAndNumAndVarPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  %1 = load i16, i16* %var11
  %addtmp = add i16 0, %1
  %2 = zext i16 %addtmp to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i16 @uHelperFunc(i16 %var) {
entry:
  %0 = alloca i16
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  store i16 %1, i16* %0
  %2 = load i16, i16* %0
  ret i16 %2
}

define i32 @uCallFunBinPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  %1 = load i16, i16* %var11
  %calltmp = call i16 @uHelperFunc(i16 %1)
  %2 = load i16, i16* %var11
  %subtmp = sub i16 %calltmp, %2
  %3 = load i16, i16* %var11
  %calltmp2 = call i16 @uHelperFunc(i16 %3)
  %addtmp = add i16 %subtmp, %calltmp2
  %4 = zext i16 %addtmp to i32
  store i32 %4, i32* %var2
  %5 = load i32, i32* %var2
  store i32 %5, i32* %0
  %6 = load i32, i32* %0
  ret i32 %6
}

define i64 @sintVarDef127() {
entry:
  %0 = alloca i64
  %var1 = alloca i16
  store i16 127, i16* %var1
  %var2 = alloca i64
  %1 = load i16, i16* %var1
  %2 = sext i16 %1 to i64
  store i64 %2, i64* %var2
  %3 = load i64, i64* %var2
  store i64 %3, i64* %0
  %4 = load i64, i64* %0
  ret i64 %4
}

define i32 @sintVarDefBinaryPara(i16 %var2) {
entry:
  %0 = alloca i32
  %var21 = alloca i16
  store i16 %var2, i16* %var21
  %var1 = alloca i16
  store i16 32767, i16* %var1
  %var3 = alloca i32
  %1 = load i16, i16* %var21
  %2 = load i16, i16* %var1
  %subtmp = sub i16 %1, %2
  %3 = sext i16 %subtmp to i32
  store i32 %3, i32* %var3
  %var4 = alloca i32
  %4 = load i32, i32* %var3
  %addtmp = add i32 %4, 32767
  store i32 %addtmp, i32* %var4
  %5 = load i32, i32* %var4
  store i32 %5, i32* %0
  %6 = load i32, i32* %0
  ret i32 %6
}

define i32 @sCallFunVarDefPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  %1 = load i16, i16* %var11
  %calltmp = call i16 @sHelperFunc(i16 %1)
  %2 = sext i16 %calltmp to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @uintVarDef127() {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 127, i16* %var1
  %var2 = alloca i32
  %1 = load i16, i16* %var1
  %2 = zext i16 %1 to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @uintVarDefBinaryPara(i16 %var2) {
entry:
  %0 = alloca i32
  %var21 = alloca i16
  store i16 %var2, i16* %var21
  %var1 = alloca i16
  store i16 -1, i16* %var1
  %var3 = alloca i32
  %1 = load i16, i16* %var21
  %2 = load i16, i16* %var1
  %addtmp = add i16 %1, %2
  %subtmp = sub i16 %addtmp, -1
  %3 = zext i16 %subtmp to i32
  store i32 %3, i32* %var3
  %4 = load i32, i32* %var3
  store i32 %4, i32* %0
  %5 = load i32, i32* %0
  ret i32 %5
}

define i32 @uCallFunVarDefPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  %1 = load i16, i16* %var11
  %calltmp = call i16 @uHelperFunc(i16 %1)
  %2 = zext i16 %calltmp to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @sAssignNum32767(i32 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i32
  store i32 %var1, i32* %var11
  store i32 32767, i32* %var11
  %1 = load i32, i32* %var11
  store i32 %1, i32* %0
  %2 = load i32, i32* %0
  ret i32 %2
}

define i32 @sAssignVarPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  %1 = load i16, i16* %var11
  %2 = sext i16 %1 to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @sAssignBinaryPara(i32 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i32
  store i32 %var1, i32* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  store i32 32767, i32* %var2
  %1 = load i32, i32* %var2
  %2 = load i32, i32* %var2
  %subtmp = sub i32 %1, %2
  %3 = load i32, i32* %var11
  %addtmp = add i32 %subtmp, %3
  store i32 %addtmp, i32* %var11
  %4 = load i32, i32* %var11
  %addtmp2 = add i32 %4, 0
  store i32 %addtmp2, i32* %var11
  %5 = load i32, i32* %var11
  store i32 %5, i32* %0
  %6 = load i32, i32* %0
  ret i32 %6
}

define i32 @sCallFunAssignPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  %1 = load i16, i16* %var11
  %calltmp = call i16 @sHelperFunc(i16 %1)
  %2 = sext i16 %calltmp to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @uAssignNum65537(i32 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i32
  store i32 %var1, i32* %var11
  store i32 65537, i32* %var11
  %1 = load i32, i32* %var11
  store i32 %1, i32* %0
  %2 = load i32, i32* %0
  ret i32 %2
}

define i32 @uAssignVarPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  %1 = load i16, i16* %var11
  %2 = zext i16 %1 to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @uAssignBinaryPara(i32 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i32
  store i32 %var1, i32* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  store i32 65535, i32* %var2
  %1 = load i32, i32* %var11
  %2 = load i32, i32* %var2
  %subtmp = sub i32 %1, %2
  %3 = load i32, i32* %var2
  %addtmp = add i32 %subtmp, %3
  store i32 %addtmp, i32* %var11
  %4 = load i32, i32* %var11
  %addtmp2 = add i32 %4, 0
  store i32 %addtmp2, i32* %var11
  %5 = load i32, i32* %var11
  store i32 %5, i32* %0
  %6 = load i32, i32* %0
  ret i32 %6
}

define i32 @uCallFunAssignPara(i16 %var1) {
entry:
  %0 = alloca i32
  %var11 = alloca i16
  store i16 %var1, i16* %var11
  %var2 = alloca i32
  store i32 0, i32* %var2
  %1 = load i16, i16* %var11
  %calltmp = call i16 @uHelperFunc(i16 %1)
  %2 = zext i16 %calltmp to i32
  store i32 %2, i32* %var2
  %3 = load i32, i32* %var2
  store i32 %3, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @sHelperFuncArg(i32 %arg1, i16 %arg2) {
entry:
  %0 = alloca i32
  %arg11 = alloca i32
  store i32 %arg1, i32* %arg11
  %arg22 = alloca i16
  store i16 %arg2, i16* %arg22
  %1 = load i32, i32* %arg11
  %2 = load i16, i16* %arg22
  %3 = sext i16 %2 to i32
  %subtmp = sub i32 %1, %3
  store i32 %subtmp, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @sFunctionArg(i16 %arg1, i8 %arg2) {
entry:
  %0 = alloca i32
  %arg11 = alloca i16
  store i16 %arg1, i16* %arg11
  %arg22 = alloca i8
  store i8 %arg2, i8* %arg22
  %1 = load i16, i16* %arg11
  %2 = sext i16 %1 to i32
  %3 = load i8, i8* %arg22
  %4 = sext i8 %3 to i16
  %calltmp = call i32 @sHelperFuncArg(i32 %2, i16 %4)
  store i32 %calltmp, i32* %0
  %5 = load i32, i32* %0
  ret i32 %5
}

define i32 @uHelperFuncArg(i32 %arg1, i16 %arg2) {
entry:
  %0 = alloca i32
  %arg11 = alloca i32
  store i32 %arg1, i32* %arg11
  %arg22 = alloca i16
  store i16 %arg2, i16* %arg22
  %1 = load i32, i32* %arg11
  %2 = load i16, i16* %arg22
  %3 = zext i16 %2 to i32
  %subtmp = sub i32 %1, %3
  store i32 %subtmp, i32* %0
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @uFunctionArg(i16 %arg1, i8 %arg2) {
entry:
  %0 = alloca i32
  %arg11 = alloca i16
  store i16 %arg1, i16* %arg11
  %arg22 = alloca i8
  store i8 %arg2, i8* %arg22
  %1 = load i16, i16* %arg11
  %2 = zext i16 %1 to i32
  %3 = load i8, i8* %arg22
  %4 = zext i8 %3 to i16
  %calltmp = call i32 @uHelperFuncArg(i32 %2, i16 %4)
  store i32 %calltmp, i32* %0
  %5 = load i32, i32* %0
  ret i32 %5
}

define i32 @sReturnNum127() {
entry:
  %0 = alloca i32
  store i32 127, i32* %0
  %1 = load i32, i32* %0
  ret i32 %1
}

define i32 @sReturnVarPara(i16 %var) {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  %2 = sext i16 %1 to i32
  store i32 %2, i32* %0
  %3 = load i32, i32* %0
  ret i32 %3
}

define i32 @sReturnBinPara(i16 %var) {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  %addtmp = add i16 0, %1
  %2 = sext i16 %addtmp to i32
  store i32 %2, i32* %0
  %3 = load i32, i32* %0
  ret i32 %3
}

define i32 @sReturnCallPara(i16 %var) {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  %calltmp = call i16 @sHelperFunc(i16 %1)
  %2 = sext i16 %calltmp to i32
  store i32 %2, i32* %0
  %3 = load i32, i32* %0
  ret i32 %3
}

define i32 @uReturnNum127() {
entry:
  %0 = alloca i32
  store i32 127, i32* %0
  %1 = load i32, i32* %0
  ret i32 %1
}

define i32 @uReturnVarPara(i16 %var) {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  %2 = zext i16 %1 to i32
  store i32 %2, i32* %0
  %3 = load i32, i32* %0
  ret i32 %3
}

define i32 @uReturnBinPara(i16 %var) {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  %addtmp = add i16 0, %1
  %2 = zext i16 %addtmp to i32
  store i32 %2, i32* %0
  %3 = load i32, i32* %0
  ret i32 %3
}

define i32 @uReturnCallPara(i16 %var) {
entry:
  %0 = alloca i32
  %var1 = alloca i16
  store i16 %var, i16* %var1
  %1 = load i16, i16* %var1
  %calltmp = call i16 @uHelperFunc(i16 %1)
  %2 = zext i16 %calltmp to i32
  store i32 %2, i32* %0
  %3 = load i32, i32* %0
  ret i32 %3
}
