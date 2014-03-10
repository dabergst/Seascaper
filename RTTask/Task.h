#ifndef _Task_H_
#define _Task_H_

typedef void (*function_ptr)();

class Task {
public:
	function_ptr callback;
	uint32_t nextCallbackTime;
	bool repeat;
	Task* nextTask;
	Task* prevTask;
public:
	Task(function_ptr callback, bool repeat) : callback(callback), nextCallbackTime(0), repeat(repeat), nextTask(0), prevTask(0) {}
	virtual ~Task() { delete nextTask; delete prevTask; }
	void virtual setNextCallbackTime(DateTime now) =0;
};

class TimeTask : public Task {
public:
	uint8_t hour, minute, second;
public:
	TimeTask(function_ptr callback, uint8_t hour, uint8_t minute, uint8_t second, bool repeat) : Task(callback, repeat), hour(hour), minute(minute), second(second) {}
	~TimeTask() {}
	void setNextCallbackTime(DateTime now) {
		nextCallbackTime = DateTime(now.year(), now.month(), now.day(), hour, minute, second).unixtime();
		if (nextCallbackTime < now.unixtime()) nextCallbackTime += 86400; // Add one day of time.
	}
};

class DelayedTask : public Task {
public:
	uint32_t seconds;
public:
	DelayedTask(function_ptr callback, uint32_t seconds, bool repeat) : Task(callback, repeat), seconds(seconds) {}
	~DelayedTask() {}
	void setNextCallbackTime(DateTime now) { nextCallbackTime = now.unixtime() + seconds; }
};

#endif // _Task_H_