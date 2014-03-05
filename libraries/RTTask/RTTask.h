#ifndef RTTASK_H
#define RTTASK_H

#include<Wire.h>
#include<RTClib.h>
#include<functional>

class RTTManagerClass;

/**
 * Protected classes for internal use only.
 */

class Task {
	void (*callback)();
	bool repeat;
protected:
	uint32_t nextCallbackTime;	
	Task* nextTask;
protected:
	Task(void (*callback)(), bool repeat) : callback(callback), repeat(repeat), nextCallbackTime(0), nextTask(0) {}
	virtual ~Task() { delete nextTask; }
	void virtual setNextCallbackTime(DateTime now) =0;
	friend class RTTManagerClass;
public:
	uint32_t getNextCallbackTime() { return nextCallbackTime; }
};

class TimeTask : protected Task {
	uint8_t hour, minute, second;
protected:
	TimeTask(void (*callback)(), uint8_t hour, uint8_t minute, uint8_t second, bool repeat) : Task(callback, repeat), hour(hour), minute(minute), second(second) {}
	~TimeTask() {}
	void setNextCallbackTime(DateTime now);
	friend class RTTManagerClass;
};

class DelayedTask : protected Task {
	uint32_t seconds;
protected:
	DelayedTask(void (*callback)(), uint32_t seconds, bool repeat) : Task(callback, repeat), seconds(seconds) {}
	~DelayedTask() {}
	void setNextCallbackTime(DateTime now);
	friend class RTTManagerClass;
};

/**
 * RealTimeTaskManager
 */
class RTTManagerClass {
	RTC_DS1307 rtc;
	Task* taskList;	
public:
	RTTManagerClass() : taskList(0) { Wire.begin(); }
	~RTTManagerClass() { delete taskList; }
	void addTimeTask(void (*callback)(), uint8_t hour, uint8_t minute, uint8_t second=0, bool repeat=false, bool callRetroactive=false);
	void addDelayedTask(void (*callback)(), uint32_t seconds, bool repeat=false, bool callOnceImmediate=false);
	bool isPending(void (*callback)());
	bool removeTask(void (*callback)());
	void run();
private:
	void addTask(Task* task);
};

extern RTTManagerClass RTTManager;

#endif //RTTASK_H