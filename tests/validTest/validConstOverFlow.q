def uint1 Sinted8_0(){
    return 127 + 127 == 254
}
def uint1 Sinted8_1(){
    return -128 + -128 == -256
}
def uint1 Sinted16_1(){
    return 32767 + 32767 == 65534
}
def uint1 Sinted16_2(){
    return (-32768) + (-32768) == -65536
}

def uint1 Sinted32_0(){
    return 2147483647 + 2147483647 == 4294967294
}
def uint1 Sinted32_1(){
    return -2147483648 + -2147483648 == -4294967296
}
def uint1 Sinted64_0(){
    return 9223372036854775807 + 9223372036854775807 == 18446744073709551614
}
def uint1 Sinted64_1(){
    return -9223372036854775807 + -9223372036854775807 == -18446744073709551614
}
def uint1 Sinted128(){
    return 170141183460469231731687303715884105727 + 170141183460469231731687303715884105727 == 340282366920938463463374607431768211454
}
