//version 0.12
#ifndef lib_Mini_h
#define lib_Mini_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
    byte hex_to_dec(String hex);
    char* cstr_and_strcpy(char *ptr_from);
    void cancat_two_char_array(char *dest, char *s1, char *s2);
    char* create_char_array_from_string(String str);
#endif