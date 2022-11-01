/**
 * The MIT License (MIT)
 * Copyright (c) 2015 by Fabrice Weinberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "lib_NTPClient.h"
NTPClient::NTPClient() {
}

void NTPClient::Set_UDP(UDP& udp){
  this->_udp            = &udp;
}

NTPClient::NTPClient(UDP& udp) {
  this->_udp            = &udp;
}

NTPClient::NTPClient(UDP& udp, int timeOffset) {
  this->_udp            = &udp;
  this->_timeOffset     = timeOffset;
}

NTPClient::NTPClient(UDP& udp, const char* poolServerName) {
  this->_udp            = &udp;
  this->_poolServerName = poolServerName;
}

NTPClient::NTPClient(UDP& udp, const char* poolServerName, int timeOffset) {
  this->_udp            = &udp;
  this->_timeOffset     = timeOffset;
  this->_poolServerName = poolServerName;
}

NTPClient::NTPClient(UDP& udp, const char* poolServerName, int timeOffset, int updateInterval) {
  this->_udp            = &udp;
  this->_timeOffset     = timeOffset;
  this->_poolServerName = poolServerName;
  this->_updateInterval = updateInterval;
}

void NTPClient::begin() {
  this->begin(NTP_DEFAULT_LOCAL_PORT);
}

void NTPClient::begin(int port) {
  this->_port = port;

  this->_udp->begin(this->_port);

  this->_udpSetup = true;
}

bool NTPClient::forceUpdate() {
  #ifdef DEBUG_NTPClient
    Serial.println("Update from NTP Server");
  #endif

  this->sendNTPPacket();

  // Wait till data is there or timeout...
  byte timeout = 0;
  int cb = 0;
  do {
    delay ( 10 );
    cb = this->_udp->parsePacket();
    if (timeout > 5) return false; // timeout after 50 ms до этого было 1000 мс
    timeout++;
  } while (cb == 0);
  // ****************************************************************** (тут ошибкаб ожидает 1 с) **************************** 

  this->_lastUpdate = millis() - (10 * (timeout + 1)); // Account for delay in reading the time

  this->_udp->read(this->_packetBuffer, NTP_PACKET_SIZE);

  unsigned long highWord = word(this->_packetBuffer[40], this->_packetBuffer[41]);
  unsigned long lowWord = word(this->_packetBuffer[42], this->_packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;

  this->_currentEpoc = secsSince1900 - SEVENZYYEARS;

  return true;
}

bool NTPClient::update() {
  if ((millis() - this->_lastUpdate >= this->_updateInterval) or this->_lastUpdate == 0) {  // Update after _updateInterval OR Update if there was no update yet.
    if(this->timer_time_update_if_not_sync.is_done() and this->_lastUpdate == 0){
      if (!this->_udpSetup) this->begin();                          // setup the UDP client if needed
      return this->forceUpdate();
      run_once = false;
    }
  }
  return true;
}

unsigned long NTPClient::getEpochTime() {
  return this->_timeOffset + // User offset
         this->_currentEpoc + // Epoc returned by the NTP server
         ((millis() - this->_lastUpdate) / 1000); // Time since last update
}

int NTPClient::getDay() {
  return (((this->getEpochTime()  / 86400L) + 4 ) % 7); //0 is Sunday
}
int NTPClient::getHours() {
  return ((this->getEpochTime()  % 86400L) / 3600);
}
int NTPClient::getMinutes() {
  return ((this->getEpochTime() % 3600) / 60);
}
int NTPClient::getSeconds() {
  return (this->getEpochTime() % 60);
}

String NTPClient::getDate() {
  unsigned long a;
  unsigned long b, c, d, e, g[13], i;
  String h1, h2, h3, h;
  c = 1968;
  d = 1970;
  a = getDate_col_day();

  e = d;
  while (a > 365){
    e = e + 1;
    d = ((e - c) % 4);
    if(d == 0){
      a = a - 366;
    } else {
      a = a - 365;
    }
  }

  curr_year = e;
  b = 0;
  if(((e - c) % 4) == 0){b = 1;}

  g[0] = 0;
  g[1] = 31;
  g[2] = 31+28+b;
  g[3] = 31+28+31+b;
  g[4] = 31+28+31+30+b;
  g[5] = 31+28+31+30+31+b;
  g[6] = 31+28+31+30+31+30+b;
  g[7] = 31+28+31+30+31+30+31+b;
  g[8] = 31+28+31+30+31+30+31+31+b;
  g[9] = 31+28+31+30+31+30+31+31+30+b;
  g[10] = 31+28+31+30+31+30+31+31+30+31+b;
  g[11] = 31+28+31+30+31+30+31+31+30+31+30+b;
  g[12] = 31+28+31+30+31+30+31+31+30+31+30+31+b;
  for (i = 0; i < 12; i++) {
    if (a > g[i] and a<=g[i+1]){
      e = i + 1;
      a = a - g[i];
      break;
    }
  }
  curr_mounth = e;
  curr_day = a;
  h1 = String(curr_day);
  if (h1.length() < 2) {h1 = "0" + h1;}
  h2 = String(curr_mounth);
  if (h2.length() < 2) {h2 = "0" + h2;}
  h3 = String(curr_year);  
  h = h1 + ":" + h2 +  ":" + h3; 
  return h;



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
unsigned int NTPClient::getDate_col_day() {
  unsigned long a,b;
  a = this->getEpochTime(); 
  b = a/(60*60*24);
  b = b + 1;
  return b;
}
String NTPClient::getMillisec() {
  unsigned long a;
  a = ((millis() - this->_lastUpdate) % 1000);
  return String(a);
}

String NTPClient::getFormattedTime() {
  unsigned long rawTime = this->getEpochTime();
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

void NTPClient::end() {
  this->_udp->stop();

  this->_udpSetup = false;
}

void NTPClient::setTimeOffset(int timeOffset) {
  this->_timeOffset = timeOffset;
  this->run_once = true;
}

void NTPClient::setUpdateInterval(int updateInterval) {
  this->_updateInterval = updateInterval;
  this->timer_time_update_if_not_sync.time_delay_const = 60 * 1000;
  this->timer_time_update_if_not_sync.start();
}

void NTPClient::sendNTPPacket() {
  // set all bytes in the buffer to 0
  memset(this->_packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  this->_packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  this->_packetBuffer[1] = 0;     // Stratum, or type of clock
  this->_packetBuffer[2] = 6;     // Polling Interval
  this->_packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  this->_packetBuffer[12]  = 49;
  this->_packetBuffer[13]  = 0x4E;
  this->_packetBuffer[14]  = 49;
  this->_packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  this->_udp->beginPacket(this->_poolServerName, 123); //NTP requests are to port 123
  this->_udp->write(this->_packetBuffer, NTP_PACKET_SIZE);
  this->_udp->endPacket();
}
