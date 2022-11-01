//version 0.11 date 01/11/2022
#include "lib_NTP.h"
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_NTP::setup(MyClass_Config *my_config){
  settings = my_config;
  settings->data.NTP = (int*)this;
  timeClient.Set_UDP(ntpUDP);
  timeClient.setTimeOffset(settings->config.NTP.offset*3600);
  timeClient.setUpdateInterval(60*1000);

  settings->status.NTP.version_lib = "0.11";
  settings->status.NTP.date_lib    = "01.11.2022"; 
  Serial.println("MyClass_NTP setup done");
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_NTP::start(){
  uint32_t t[2];
  t[0] = micros();    
  timeClient.update();
  settings->data.time = timeClient.getFormattedTime();
  settings->data.date = timeClient.getDate();
  settings->data.time_stamp = settings->data.date + " " + settings->data.time + ":" + timeClient.getMillisec();
  // timeClient.
  t[1] = micros();
  settings->data.dt[4] = t[1] - t[0];     
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_NTP::SetLocalTime(String remote_date_time){
  unsigned long dif;
  // unsigned long dif;
  // dif = remote_epoch_time - timeClient.getEpochTime();
  // timeClient.setTimeOffset(dif);
  // timeClient.s
  struct tm ptm;
  ptm.tm_year = StrWithSeparator_GetNStr(remote_date_time, ',', 0).toInt();
  ptm.tm_mon = StrWithSeparator_GetNStr(remote_date_time, ',', 1).toInt();
  ptm.tm_mday = StrWithSeparator_GetNStr(remote_date_time, ',', 2).toInt();
  ptm.tm_hour = StrWithSeparator_GetNStr(remote_date_time, ',', 3).toInt();
  ptm.tm_min = StrWithSeparator_GetNStr(remote_date_time, ',', 4).toInt();
  ptm.tm_sec = StrWithSeparator_GetNStr(remote_date_time, ',', 5).toInt();
  ptm.tm_wday = StrWithSeparator_GetNStr(remote_date_time, ',', 6).toInt();
  ptm.tm_yday = StrWithSeparator_GetNStr(remote_date_time, ',', 7).toInt();
  ptm.tm_isdst = StrWithSeparator_GetNStr(remote_date_time, ',', 8).toInt();

  time_t rem_dt;
  time_t loc_dt;
  rem_dt = mktime(&ptm);
  // rem_dt = ptm.tm_sec + ptm.tm_min * 60 + ptm.tm_hour * 60 * 60 + StrWithSeparator_GetNStr(remote_date_time, ',', 7).toInt() * 60 * 60 * 24 + (ptm.tm_year - 1970) * 60 * 60 * 24 * 365;
  loc_dt = timeClient.getEpochTime();

  dif = loc_dt - rem_dt;
  timeClient.setTimeOffset(dif);
  
  Serial.println(rem_dt);
  Serial.println(loc_dt);
  Serial.println(dif);

  Serial.println(ptm.tm_year);
  Serial.println(ptm.tm_mon);
  Serial.println(ptm.tm_mday);
  Serial.println(ptm.tm_hour);
  Serial.println(ptm.tm_min);
  Serial.println(ptm.tm_sec);
  Serial.println(ptm.tm_wday);
  Serial.println(ptm.tm_yday);
  Serial.println(ptm.tm_isdst);

  Serial.println(remote_date_time);

  // ptm.tm_year = StrWithSeparator_GetNStr(local_date_and_time, '_', 0).toInt();
  // ptm.tm_mon = StrWithSeparator_GetNStr(local_date_and_time, '_', 1).toInt();  
  // ptm.tm_mday = StrWithSeparator_GetNStr(local_date_and_time, '_', 2).toInt();  
  
  // ptm.tm_yday = StrWithSeparator_GetNStr(local_date_and_time, '_', 2).toInt();  

  // gmtime()
  // double	   difftime (time_t _time2, time_t _time1);
  // time_t	   mktime (struct tm *_timeptr);
  // time_t	   time (time_t *_timer);
  
  // struct tm *ptm = gmtime ((time_t *)&epochTime); 

  // int monthDay = ptm->tm_mday;
  // Serial.print("Month day: ");
  // Serial.println(monthDay);

  // int currentMonth = ptm->tm_mon+1;
  // Serial.print("Month: ");
  // Serial.println(currentMonth);

  // String currentMonthName = months[currentMonth-1];
  // Serial.print("Month name: ");
  // Serial.println(currentMonthName);

  // int currentYear = ptm->tm_year+1900;
  // Serial.print("Year: ");
  // Serial.println(currentYear);

  // //Print complete date:
  // String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  // Serial.print("Current date: ");
  // Serial.println(currentDate);  

}