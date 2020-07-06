def uint1 Sinted8_16_0(){
    return 127 + 32767 == 32894
}
def uint1 Sinted8_16_1(){
    return 127 + (-32768) == -32641
}
def uint1 Sinted8_16_2(){
    return (-128) + (-32768) == -32896
}
def uint1 Sinted8_16_3(){
    return (-128) + 32767 == 32639
}

def uint1 Sinted16_32_0(){
    return 2147483647 + 32767 == 2147516414
}
def uint1 Sinted16_32_1(){
    return 2147483647 + (-32768) == -2147450889
}
def uint1 Sinted16_32_2(){
    return (-2147483648) + (-32768) == -2147516416
}
def uint1 Sinted16_32_3(){
    return (-2147483648) + 32767 == -2147450891
}

def uint1 Uinted8_16_1(){
    return 255 + 65535 == 65790
}

def uint1 Uinted16_32_2(){
    return 2147483647 + 32767 == 4295032828
}
