//version 0.12
#ifndef lib_Mini_h
#define lib_Mini_h
#include <lib_Config.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
    byte hex_to_dec(String hex);
    char* cstr_and_strcpy(char *ptr_from);
    void cancat_two_char_array(char *dest, char *s1, char *s2);
    char* create_char_array_from_string(String str);
    void addr_string_to_byte(String str_addr, byte *byte_addr);
    String addr_char_to_string(byte *addr);
    void float_to_2byte(float fval, byte *bval);
    float four_byte_to_float(byte *bval);
    byte Compute_CRC8(byte *bytes, int len);
    bool RSTrig_calc(byte col, bool *param);
    bool RSTrig(MyClass_Config *my_config, byte num);
#endif