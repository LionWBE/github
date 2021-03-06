// 08/06/2022
#include "lib_Mini.h"
//-------------------------------------------------------------------------------------------------------------------------------------
char* create_char_array_from_string(String str){
    int n = str.length()+1;
    char *chr = new char[n];
    str.toCharArray(chr, n); 
    return chr;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void cancat_two_char_array(char *dest, char *s1, char *s2){
    dest[0] = '\0';
    strcat(dest, s1);
    strcat(dest, s2);
}
//-------------------------------------------------------------------------------------------------------------------------------------
char* cstr_and_strcpy(char *ptr_from){
    char* ptr = new char(strlen(ptr_from) + 1);
    strcpy(ptr, ptr_from);  
    return ptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------
byte hex_to_dec(String hex){
    byte n = hex.length();
    hex.toLowerCase();
    byte rez = 0;
    for (unsigned int i = 0; i < n; i++){
        char a = hex.charAt(i);
        byte b = a - '0';
        if (a == 'a') b = 10;
        if (a == 'b') b = 11;
        if (a == 'c') b = 12;
        if (a == 'd') b = 13;
        if (a == 'e') b = 14;
        if (a == 'f') b = 15;
        rez = rez + pow(16,(n-1)-i)*b;
    }
    return rez;
}
// ************************************************************************************************************
void addr_string_to_byte(String str_addr, byte *byte_addr){
  String b;
  byte s;
  for (byte i = 0; i < 8; i++){
    b = str_addr.substring(i*2 + i, i*2+2 + i);
    s = hex_to_dec(b);
    byte_addr[i] = char(s);
  }
}
// ************************************************************************************************************
String addr_char_to_string(byte *addr){
  String rez = "";
  String b;
  for (byte i = 0; i < 8; i++){
    b = String(addr[i], HEX);
    if (addr[i] < 16)	b = "0" + b;
    rez += b;
    if (i<7) rez += ":";
  }
  return rez;
}
// ************************************************************************************************************
void float_to_4_byte(float fval, byte *bval){
  // fval
  // byte a;
  memcpy(bval, &fval, 4);
  // String rez = "";
  // String b;
  // for (byte i = 0; i < 8; i++){
  //   b = String(addr[i], HEX);
  //   if (addr[i] < 16)	b = "0" + b;
  //   rez += b;
  //   if (i<7) rez += ":";
  // }
  // return rez;

//   float var;
// char buffer[4];
// ...
// CopyMemory(&var, buffer, 4);
}