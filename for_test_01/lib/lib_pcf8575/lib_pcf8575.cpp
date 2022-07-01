//version 0.16
#include "lib_PCF8575.h"
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::setup(MyClass_Config *my_config, byte num_board){
  String adr;
  settings = my_config;
  my_Tags = (MyClass_MQTT_Tag*)settings->data.Tags;
  board.Set_address(settings->config.PCF8575.board[num_board].I2C_adr);
  MQTT_link_val = new uint16_t[16];

  if (settings->config.PCF8575.board[num_board].Type == 1){                    //16 DI
    for (byte i = 0; i < 16; i++) {
      setup_DI_one(i, num_board);
    }
  }
  if (settings->config.PCF8575.board[num_board].Type == 2){                    //16 DO
    for (byte i = 0; i < 16; i++) {
      setup_DO_one(i, num_board);   
    }  
  }
  if (settings->config.PCF8575.board[num_board].Type == 3){                    //8 DI + 8 DO
    for (byte i = 0; i < 8; i++) {
      setup_DI_one(i, num_board);
    }
    for (byte i = 8; i < 16; i++) {
      setup_DO_one(i, num_board);   
    }      
  }
  adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/online"; 
  // MQTT_link_adr[0] = my_Tags->CreateNewTag(&adr);      //error here
  adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/I2C_addres"; 
  // MQTT_link_adr[1] = my_Tags->CreateNewTag(&adr);      //error here
  board.begin();
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::setup_DI_one(byte i, byte num_board){
  String adr;
  DI[i].is_enable = true;
  DO[i].is_enable = false;
  board.pinMode(uint8_t(i), INPUT);
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
  adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/DI_" + String(i) + ".val";
  // MQTT_link_val[i] = my_Tags->CreateNewTag(&adr, true);   //error here
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::setup_DO_one(byte i, byte num_board){
  String adr;
  DI[i].is_enable = false;
  DO[i].is_enable = true;
  board.pinMode(i, OUTPUT);
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
  adr = settings->config.MQTT.Home_topic + "PCF8575/board " + String(num_board) + "/DO_" + String(i) + ".val";
  // MQTT_link_val[i] = my_Tags->CreateNewTag(&adr, true);     //error here
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::start(byte num_board){

  for (byte i = 0; i < 16; i++){ // 292 mks
    if (DI[i].is_enable){
      read_state_one_DI(i);
    } 
    if (DO[i].is_enable){
      // state = settings->config.DOs.DO[link_to_DO_in_config[i]].Cmd;
      bool enable = settings->config.DOs.DO[link_to_DO_in_config[i]].LinkTo.Enable;
      if (enable){
        byte type = settings->config.DOs.DO[link_to_DO_in_config[i]].LinkTo.type;
        byte link = settings->config.DOs.DO[link_to_DO_in_config[i]].LinkTo.link;
        byte state = 0;
        if (type == 1){
          state = RSTrig(settings, link);
        }
        write_state_one_DO(i, state);
      }
    }
  }
  for (byte i = 0; i < 16; i++){ // 292 mks
    // if (DI[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DI[i].val_curr);
    // if (DO[i].is_enable) my_Tags->SetTagVal(MQTT_link_val[i], DO[i].val_curr);
  } 
  uint8_t adr = board.Get_I2C_Addres(); // 8 mks
  is_online = board.IsStatusTwoWireOnline(adr);
  // my_Tags->SetTagVal(MQTT_link_adr[0], is_online); // 192 mks  //error here
  // my_Tags->SetTagVal(MQTT_link_adr[1], adr); // 19 mks   //error here
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::read_state_one_DI(byte i){// 7390 mks
  bool b1;
  uint8_t b2;
  board.digitalWrite(i, 1);             // 6749 / 16 = mks
  board.readBuffer();                  // 511 mks
  b2 = board.digitalRead(uint8_t(i));  // 58 mks
  if (b2 == 0){b1 = false;}
  if (b2 == 1){b1 = true;}
  DI[i].debounce(b1);

  if (DI[i].is_set_val_curr){
    settings->config.DIs.DI[link_to_DI_in_config[i]].Val = DI[i].val_curr;
  }
}
//-----------------(методы класса MyClass_PCF8575)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_PCF8575::write_state_one_DO(byte i, byte state){
  board.digitalWrite(i, state);        
  DO[i].debounce(state);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------

//-----------------(методы класса MyClass_all_PCF8575)---------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_all_PCF8575::setup(MyClass_Config *my_config){
  settings = my_config;
  col_board = settings->config.PCF8575.col;
  board = new MyClass_PCF8575[col_board];
  is_online = new bool[col_board];
  for (byte i = 0; i < col_board; i++) {
    board[i].setup(settings, i);
  }
  Serial.println("MyClass_all_PCF8575 setup done");
}
//-----------------(методы класса MyClass_all_PCF8575)---------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_all_PCF8575::start(){
  for (byte i = 0; i < col_board; i++) {
    board[i].start(i);
    is_online[i] = board[i].is_online;
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