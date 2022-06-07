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