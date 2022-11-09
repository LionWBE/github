//version 0.19 date 09/11/2022
#include "lib_Mini.h"
//-------------------------------------------------------------------------------------------------------------------------------------
void lib_Mini_setup(MyClass_Config* settings){{
  settings->status.lib_Mini.version_lib = "0.19";
  settings->status.lib_Mini.date_lib = "09.11.2022";
}}
//-------------------------------------------------------------------------------------------------------------------------------------
byte bool_to_byte(bool val){
    byte s = 0;
    if (val) s = 1;
    return s;
}
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
String dec_to_hex(byte dec){
  String s;
  s = String(dec, HEX);
  if (s.length() == 1) s = "0" + s;
  return s;
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
void float_to_2byte(float fval, byte *bval){
  // memcpy(bval, &fval, 4);
  bval[0] = int(fval);
  bval[1] = int((fval - bval[0])*100);
}
// ************************************************************************************************************
float four_byte_to_float(byte *bval){
  float rez;
  memcpy(&rez, bval, 4);
  return rez;
}
// **************************************************************************************************************
byte Compute_CRC8(byte *bytes, int len) {
  unsigned char crc = 0x00;
  while (len--){
      crc = crc8Table[crc ^ *bytes++];
  }
  return crc;
}
// **************************************************************************************************************
byte Compute_CRC8_for_file(byte *bytes, int len, byte crc) {
// вычисление CRC8 суммы для массива байтов имея предыдущую сумму CRC8
  while (len--){
    crc = crc8Table[crc ^ *bytes++];
  }  
  return crc;
}
// **************************************************************************************************************
bool RSTrig_calc(byte col, bool *param){
  byte val = 0;
  byte a;
  for (byte i = 0; i < col; i++){
    if (param[i]){
      a = 1;
    }else{
      a = 0;
    }
    val += a;
  }
  if (val % 2 == 0){
    return false;
  }else{
    return true;
  }
}
// **************************************************************************************************************
bool RSTrig(MyClass_Config *my_config, byte num){
  bool *mas, rez;
  byte n, m;

  n = my_config->config.Trigers.Trigers[num].col;
  mas = new bool[n];
  for (byte i = 0; i < n; i++){
    m = my_config->config.Trigers.Trigers[num].DI[i];  // номер DI
    mas[i] = my_config->config.DIs.DI[m].Val;          // значение DI   
  }
  rez = RSTrig_calc(n, mas); 
  delete mas;
  return rez;  
}
//***************************************************************************************************
void print_free_memory(){
  long fh = ESP.getFreeHeap();
  Serial.println(fh);
}  
//***************************************************************************************************
long get_free_memory(){
  long fh = ESP.getFreeHeap();
  return fh;
}    
//***************************************************************************************************
void get_byte_mac_from_string(byte *byte_mac, String str_mac){
  if (str_mac.length() > 0){
    byte num = StrWithSeparator_GetNumSeparator(str_mac, ':');
    String *str_mac_mas = new String[num];
    StrWithSeparator_GetMasStr(str_mac_mas, str_mac, ':');
    for (byte i = 0; i < num; i++){
      byte_mac[i] = hex_to_dec(str_mac_mas[i]);
    }
    delete str_mac_mas;
  }else{
    for (byte i = 0; i < 6; i++){
      byte_mac[i] = 0;
    }
  }
}  
//***************************************************************************************************
void get_byte_ip_from_string(byte *byte_ip, String str_ip){
  if (str_ip.length() > 0){
    byte num = StrWithSeparator_GetNumSeparator(str_ip, '.');
    String *str_ip_mas = new String[num];
    StrWithSeparator_GetMasStr(str_ip_mas, str_ip, '.');
    for (byte i = 0; i < num; i++){
      byte_ip[i] = byte(str_ip_mas[i].toInt());
    }
    delete str_ip_mas;
  }else{
    for (byte i = 0; i < 4; i++){
      byte_ip[i] = 0;
    }
  }
}
//***************************************************************************************************
byte StrWithSeparator_GetNumSeparator(String str_in, char separator){
  String str = str_in;
  byte NumSeparator = 0;
  byte index;
  while (str.length() > 0){
    NumSeparator++;
    index = str.indexOf(separator);
    if (index == -1){
      break;
    }
    str = str.substring(index+1);
  }  
  return NumSeparator;
}
//***************************************************************************************************
byte StrWithSeparator_GetMasStr(String *str_out, String str_in, char separator){
  String str = str_in;
  byte NumSeparator = StrWithSeparator_GetNumSeparator(str_in, separator);
  byte StringCount = 0;
  while (str.length() > 0){
    byte index = str.indexOf(separator);
    if (index == -1){
      str_out[StringCount] = str;
      break;
    }else{
      str_out[StringCount] = str.substring(0, index);
      str = str.substring(index+1);
    }
    StringCount++;
  }
  return NumSeparator;
}
//***************************************************************************************************
String StrWithSeparator_GetLastStr(String str_in, char separator){
  String str = str_in;
  String str_out;
  byte NumSeparator = StrWithSeparator_GetNumSeparator(str_in, separator);
  str_out = StrWithSeparator_GetNStr(str_in, separator, NumSeparator);
  return str_out;
}
//***************************************************************************************************
String StrWithSeparator_GetNStr(String str_in, char separator, byte num){
  String str = str_in;
  String str_out;
  byte NumSeparator = StrWithSeparator_GetNumSeparator(str_in, separator);
  byte StringCount = 0;
  while (str.length() > 0){
    byte index = str.indexOf(separator);
    if (StringCount == num){
      str_out = str.substring(0, index);
      break;
    }else{
      str = str.substring(index+1);
    }
    StringCount++;
  }  
  return str_out;
}
//***************************************************************************************************
String byte_mas_to_str(byte *byte_mas, uint16_t len_mas){
  String rez = "";
  char s;
  for (uint16_t i = 0; i < len_mas; i++){
    s = byte_mas[i];
    rez += String(s);
  }
  return rez;
}
//***************************************************************************************************
bool is_file_exist(String full_file_name){
  SPIFFS.begin();
  File file = SPIFFS.open(full_file_name, "r");
  bool rez = false;
  if (!file) {
    rez = false;
  }else{
    rez = true;
  }
  file.close();
  SPIFFS.end();
  return rez;
}
//***************************************************************************************************
void is_file_exist_delete(String full_file_name){
  SPIFFS.begin();
  File file = SPIFFS.open(full_file_name, "r");
  bool rez = false;
  if (!file) {
    rez = false;
  }else{
    rez = true;
  }
  file.close();
  if (rez) SPIFFS.remove(full_file_name);
  SPIFFS.end();
}
//***************************************************************************************************
byte Compute_CRC8_for_full_file(String file_name){
  byte crc = 0;
  SPIFFS.begin();
  File file = SPIFFS.open(file_name, "r");
  while (file.available()){
    byte byte_from_file = file.read();
    crc = crc8Table[crc ^ byte_from_file];      
  }  
  file.close();
  SPIFFS.end();
  return crc;
}
//***************************************************************************************************
size_t GetFileSize(String file_name){
  size_t FileSize = 0;
  SPIFFS.begin();
  File file = SPIFFS.open(file_name, "r");
  FileSize = file.size();
  file.close();
  SPIFFS.end();
  return FileSize;
}
//***************************************************************************************************
String GetFileList(){
  SPIFFS.begin();
  String str = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
  }
  SPIFFS.end();
  return str;
}
//***************************************************************************************************
void file_rename(String full_file_name_old, String full_file_name_new){
  is_file_exist_delete(full_file_name_new);
  SPIFFS.begin();
  SPIFFS.rename(full_file_name_old, full_file_name_new);
  SPIFFS.end();
}
//***************************************************************************************************
byte get_val_bit_by_number(byte num_bit, byte value){
  byte bit = (value >> num_bit) & 1;
  return bit;      
}        
//***************************************************************************************************
byte get_val_bit_by_number(byte num_bit, uint16_t value){
  byte bit = (value >> num_bit) & 1;
  return bit;      
}        