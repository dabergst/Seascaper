#ifndef _RTTask_H_
#define _RTTask_H_

#include <stdint.h>
#include <Wire.h>
#include <RTClib.h>
#include <Task.h>

typedef void (*function_ptr)();

class RTTManagerClass {
	RTC_DS1307 rtc;
	Task* taskList;
public:
	RTTManagerClass() : taskList(0) { Wire.begin(); rtc.begin(); }
	~RTTManagerClass() { delete taskList; }
	void addTimeTask(function_ptr, uint8_t hour, uint8_t minute, uint8_t second=0, bool repeat=false, bool callRetroactive=false);
	void addDelayedTask(function_ptr, uint32_t seconds, bool repeat=false, bool callOnceImmediatly=false);
	void run();
private:
	void addTask(Task* task);
};

extern RTTManagerClass RTTManager;

#endif // _RTTask_H_