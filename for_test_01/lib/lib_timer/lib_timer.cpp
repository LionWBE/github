#include "lib_timer.h"
//-----------------(методы класса MyClass_timer)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_timer::start(){
	time_now = millis();
  time_prev = millis();
  time_delay = 0;
  is_start = true;
  is_stop = false;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_timer::stop_(){
  time_delay = 0;
  is_start = false;
  is_stop = true;
}
//-------------------------------------------------------------------------------------------------------------------------------------
int MyClass_timer::get_time_delay(){
  time_now = millis();
  time_delay = time_now - time_prev;
  return time_delay;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_timer::print_time_delay(){
	get_time_delay();
	Serial.print("time_delay =");
	Serial.println(time_delay);	
}
//-------------------------------------------------------------------------------------------------------------------------------------
MyClass_timer::MyClass_timer(){
  stop_();
}
//-------------------------------------------------------------------------------------------------------------------------------------
bool MyClass_timer::is_done(){
  bool a;
  a = false;
  time_now = millis();
  time_delay = time_now - time_prev;
  if (time_delay >= time_delay_const) {             //задержка на устранение дребезга (только если на входе висит другое значение)
    a = true;
    time_prev = time_now;
  }
  return a;
}
//-----------------(методы класса MyClass_timer_mc)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_timer_mc::start(){
	time_now = micros();
  time_prev = micros();
  time_delay = 0;
  is_start = true;
  is_stop = false;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_timer_mc::stop_(){
  time_delay = 0;
  is_start = false;
  is_stop = true;
}
//-------------------------------------------------------------------------------------------------------------------------------------
int MyClass_timer_mc::get_time_delay(){
  time_now = micros();
  time_delay = time_now - time_prev;
  return time_delay;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_timer_mc::print_time_delay(){
	get_time_delay();
	Serial.print("time_delay =");
	Serial.println(time_delay);	
}
//-------------------------------------------------------------------------------------------------------------------------------------
MyClass_timer_mc::MyClass_timer_mc(){
  stop_();
}
//-------------------------------------------------------------------------------------------------------------------------------------
bool MyClass_timer_mc::is_done(){
  bool a;
  a = false;
  time_now = micros();
  time_delay = time_now - time_prev;
  if (time_delay >= time_delay_const) {             //задержка на устранение дребезга (только если на входе висит другое значение)
    a = true;
    time_prev = time_now;
  }
  return a;
}
//-----------------(методы класса MyClass_time_since_start)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
char* MyClass_time_since_start::get_time(){
	uint64_t time_in_second;
	if (timer.is_done()) {
		time_in_millisecond = time_in_millisecond + timer.time_delay;
		time_in_second = time_in_millisecond/1000;
		day = time_in_second / (86400);
		hour = time_in_second / (3600) - day*24;
		minute = time_in_second / (60) - day*24*60 - hour*60;
		second = time_in_second - day*86400 - hour*60*60 - minute*60;
		sprintf (stamp, "%04d day %02d:%02d:%02d", day, hour, minute, second);
	}
	return stamp;
}
//-----------------(методы класса MyClass_time_since_start)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_time_since_start::start(){
	timer.time_delay_const = 1000;
	time_in_millisecond = 0;
	stamp = new char[40];
  timer.start();
}
