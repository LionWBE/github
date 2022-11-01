//version 0.18 date 28/10/2022
#include "lib_PCF8575_NEW.h"
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::setup(MyClass_Config *my_config, byte num_board){
  String str_adr;
  settings = my_config;
  my_Tags = (MyClass_MQTT_Tag*)settings->data.Tags;
  my_ethernet = (MyClass_Ethernet*)settings->data.Ethernet;
  I2C_adr = settings->config.PCF8575.board[num_board].I2C_adr;
  MQTT_link_val = new uint16_t[16];

  if (settings->config.PCF8575.board[num_board].Type == 1){                    //16 DI
    board_type = 1;
    Wire.beginTransmission(I2C_adr);
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.endTransmission();  
    for (byte i = 0; i < 16; i++) {
      setup_DI_one(i, num_board);
    }
  }
  if (settings->config.PCF8575.board[num_board].Type == 2){                    //16 DO
    board_type = 2;
    Wire.beginTransmission(I2C_adr);
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.endTransmission();  
    for (byte i = 0; i < 16; i++) {
      setup_DO_one(i, num_board);   
    }  
  }

  str_adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/online"; 
  MQTT_link_adr[0] = my_Tags->CreateNewTag(&str_adr);      //error here
  str_adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/I2C_addres"; 
  MQTT_link_adr[1] = my_Tags->CreateNewTag(&str_adr);      //error here

  All_DI_is_set = false;
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::setup_DI_one(byte i, byte num_board){
  String str_adr;
  DI[i].is_enable = true;
  DO[i].is_enable = false;
  DI[i].setup(settings);
  link_to_DI_in_config[i] = -1; 
  for (byte j = 0; j < settings->config.DIs.col; j++) {
    if (num_board == settings->config.DIs.DI[j].PCF8575_adr.board_num){
      if (i == settings->config.DIs.DI[j].PCF8575_adr.input_num){
        link_to_DI_in_config[i] = j;
        break;
      }
    }
  }
  str_adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/DI_" + String(i) + ".val";
  MQTT_link_val[i] = my_Tags->CreateNewTag(&str_adr, true);   //error here
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::setup_DO_one(byte i, byte num_board){
  String str_adr;
  DI[i].is_enable = false;
  DO[i].is_enable = true;
  DO[i].setup(settings);
  link_to_DO_in_config[i] = -1; 
  for (byte j = 0; j < settings->config.DOs.col; j++) {
    if (num_board == settings->config.DOs.DO[j].PCF8575_adr.board_num){
      if (i == settings->config.DOs.DO[j].PCF8575_adr.input_num){
        link_to_DO_in_config[i] = j;
        break;
      }
    }
  }  
  str_adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/DO_" + String(i) + ".val";
  // MQTT_link_val[i] = my_Tags->CreateNewTag(&adr, true);     //error here
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::start(){
  if(All_DI_is_set == false){   //  если есть неустановившийся режим для DI
    read_state_DI();
  }
  // for (byte i = 0; i < 16; i++){ // 292 mks
  //   if (DI[i].is_enable){
  //     read_state_one_DI(i);
  //   } 
  //   if (DO[i].is_enable){
  //     // state = settings->config.DOs.DO[link_to_DO_in_config[i]].Cmd;
  //     bool enable = settings->config.DOs.DO[link_to_DO_in_config[i]].LinkTo.Enable;
  //     if (enable){
  //       byte type = settings->config.DOs.DO[link_to_DO_in_config[i]].LinkTo.type;
  //       byte link = settings->config.DOs.DO[link_to_DO_in_config[i]].LinkTo.link;
  //       byte state = 0;
  //       if (type == 1){
  //         state = RSTrig(settings, link);
  //       }
  //       write_state_one_DO(i, state);
  //     }
  //   }
  // }
  // for (byte i = 0; i < 16; i++){ // 292 mks
    // if (DI[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DI[i].val_curr);
    // if (DO[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DO[i].val_curr);
  // } 
  // uint8_t adr = board.Get_I2C_Addres(); // 8 mks
  // is_online = board.IsStatusTwoWireOnline(adr);
  // my_Tags->SetTagVal(MQTT_link_adr[0], is_online); // 192 mks  //error here
  // my_Tags->SetTagVal(MQTT_link_adr[1], adr); // 19 mks   //error here
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::interrupt(){
  read_state_DI(); 
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::read_state_DI(){// 7390 mks
  if (board_type == 1){   // DI
    byte read_value_1;
    byte read_value_2;
    if (Wire.requestFrom(I2C_adr, 2) == 2){
      read_value_1=(Wire.read());
      read_value_2=(Wire.read());

      bool bits[16];
      bits[0] = read_value_1 & 1; 
      bits[1] = read_value_1 & 2; 
      bits[2] = read_value_1 & 4; 
      bits[3] = read_value_1 & 8; 
      bits[4] = read_value_1 & 16; 
      bits[5] = read_value_1 & 32; 
      bits[6] = read_value_1 & 64; 
      bits[7] = read_value_1 & 128; 
      bits[8] = read_value_2 & 1; 
      bits[9] = read_value_2 & 2; 
      bits[10] = read_value_2 & 4; 
      bits[11] = read_value_2 & 8; 
      bits[12] = read_value_2 & 16; 
      bits[13] = read_value_2 & 32; 
      bits[14] = read_value_2 & 64; 
      bits[15] = read_value_2 & 128; 

      All_DI_is_set = true;
      for (byte i = 0; i < 16; i++){ 
        bool b1;
        if (bits[i] == 0){b1 = true;}
        if (bits[i] == 1){b1 = false;}
        DI[i].debounce(b1);        
        
        if (DI[i].is_set_val_curr){
          settings->config.DIs.DI[link_to_DI_in_config[i]].Val = DI[i].val_curr;
          if (DI[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DI[i].val_curr);
        }else{
          All_DI_is_set = false;
        }
      }
      if (All_DI_is_set){
        my_ethernet->DEBUG(" Byte 1:", read_value_1);
        my_ethernet->DEBUG(" Byte 2:", read_value_2);
      }

    }   
  }
  // for (byte i = 0; i < 16; i++){ // 292 mks
    // if (DI[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DI[i].val_curr);
    // if (DO[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DO[i].val_curr);
  // }           
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::write_state_one_DO(byte i, byte state){
  // board.digitalWrite(i, state);        
  // DO[i].debounce(state);
}
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-----------------(методы класса MyClass_all_PCF8575)---------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_all_PCF8575::setup(MyClass_Config *my_config){
  Wire.begin();
  settings = my_config;
  my_ethernet = (MyClass_Ethernet*)settings->data.Ethernet;
  col_board = settings->config.PCF8575.col;
  board = new MyClass_PCF8575[col_board];
  is_online = new bool[col_board];
  for (byte i = 0; i < col_board; i++) {
    board[i].setup(settings, i);
  }
  settings->status.lib_PCF8575.version_lib = "0.18";
  settings->status.lib_PCF8575.date_lib    = "28.10.2022";    
  Serial.println("MyClass_all_PCF8575 setup done");
}
//-----------------(методы класса MyClass_all_PCF8575)---------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_all_PCF8575::start(){
  uint32_t t[2];
  t[0] = micros();
  for (byte i = 0; i < col_board; i++) {
    board[i].start();
    is_online[i] = board[i].is_online;
  }
  t[1] = micros();
  settings->data.dt[5] = t[1] - t[0];
}
//-----------------(методы класса MyClass_all_PCF8575)---------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_all_PCF8575::interrupt(){
  for (byte i = 0; i < col_board; i++) {
    board[i].interrupt();
    // my_ethernet->DEBUG("inner interrupt");
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-----------------(методы класса MyClass_PCF8575_DI)----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575_DI::debounce(bool val){
  if (val != val_curr and is_set_val_curr == true){        //если считанное значение отличается от установивщегося текущего
    val_prev = val_curr;
    is_set_val_prev = true;
    is_set_val_curr = false;
  } 
  
  if (is_set_val_curr == false and timer_bounce.is_stop){  //если текущее значение не установлено
    is_bounce = true;
    timer_bounce.start();
  }
     
  if (is_first_start == true){                             //если первый запуск
    is_bounce = true;
    is_first_start = false;
    timer_bounce.start();
  }
     
  if (is_bounce == true and timer_bounce.is_done()){       //процесс устранения дребезга
    val_curr = val;
    is_set_val_curr = true;
    is_bounce = false;
    timer_bounce.stop_();
  }
}
//-----------------(методы класса MyClass_PCF8575_DI)----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575_DI::setup(MyClass_Config *my_config){
  settings = my_config;
  is_bounce = false;
  is_set_val_curr = false;
  is_set_val_prev = true;  
  val_prev = false;
  val_curr = false;
  timer_bounce.time_delay_const = 50;
}