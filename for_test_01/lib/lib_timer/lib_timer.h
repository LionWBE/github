//version 0.12
#ifndef lib_timer_h
#define lib_timer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MyClass_timer {
  public:
    MyClass_timer();
    void start();
    void stop_();
    bool is_done();
    bool is_start;
    bool is_stop;
    int get_time_delay();
    void print_time_delay();
    unsigned long time_delay_const; //время между событиями - необходимо задать в мс
    unsigned long time_delay;       //прошло времени в мс после изменения состояния
  private:
    unsigned long time_now;         //текущее время
    unsigned long time_prev;        //предыдущее время изменения состояния   
};
class MyClass_timer_mc {
  public:
    MyClass_timer_mc();
    void start();
    void stop_();
    bool is_done();
    bool is_start;
    bool is_stop;
    int get_time_delay();
    void print_time_delay();
    unsigned long time_delay_const; //время между событиями - необходимо задать
    unsigned long time_delay;       //прошло времени после изменения состояния
  private:
    unsigned long time_now;         //текущее время
    unsigned long time_prev;        //предыдущее время изменения состояния   
};
class MyClass_time_since_start {
  public:
	  void start();
	  uint64_t time_in_millisecond; //
	  char *stamp;
	  MyClass_timer timer;
	  char* get_time();
  private:
    int day;       //
    byte hour;     //	
	  byte minute;   //
	  byte second;   //
};
#endif
