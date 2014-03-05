#pragma once

#include "Arduino.h"

RTC_DS1307 rtc;

void printTrace(const char* msg);
#define PRINT_TRACE printTrace(__FUNCTION__);


int freeRam () {
	extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void printRTC() {
	char timestr[] = "HH:MM:SS";
	DateTime now = rtc.now();
	timestr[0] = '0'+now.hour()/10;
	timestr[1] = '0'+now.hour()%10;
	timestr[3] = '0'+now.minute()/10;
	timestr[4] = '0'+now.minute()%10;
	timestr[6] = '0'+now.second()/10;
	timestr[7] = '0'+now.second()%10;
	Serial.print(timestr);
}

void printFreeRAM() {	
	printRTC();
	Serial.print(F(" Free RAM: "));
	Serial.print(freeRam());
	Serial.println(F(" bytes"));
}

void printTrace(const __FlashStringHelper* msg) {
	printRTC();
	Serial.print(" ");
	Serial.println(msg);
}

void printTrace(const char* msg) {
#ifndef NDEBUG
	printRTC();
	Serial.print(" ");
	Serial.println(msg);
#endif
}