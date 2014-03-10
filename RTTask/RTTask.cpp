#include "RTTask.h"
#include "Arduino.h"

typedef void (*function_ptr)();

void RTTManagerClass::addTimeTask(function_ptr callback, uint8_t hour, uint8_t minute, uint8_t second, bool repeat, bool callRetroactive) {
	Task* task = new TimeTask(callback, hour, minute, second, repeat);
	task->setNextCallbackTime(rtc.now());
	if (callRetroactive) 
		task->nextCallbackTime -= 86400*2; // Subtracts two days of time.
	addTask(task);
}

void RTTManagerClass::addDelayedTask(void (*callback)(), uint32_t seconds, bool repeat, bool callOnceImmediatly) {
	Task* task = new DelayedTask(callback, seconds, repeat);
	if (callOnceImmediatly)
		task->nextCallbackTime = rtc.now().unixtime();
	else
		task->setNextCallbackTime(rtc.now());
	addTask(task);
}

void RTTManagerClass::run() {
	uint32_t now = rtc.now().unixtime();
	Task* currentTask = taskList;
	Task* nextTask;
	Task* prevTask;

	while (currentTask != 0 && now >= currentTask->nextCallbackTime) {
		currentTask->callback();
		nextTask = currentTask->nextTask;
		prevTask = currentTask->prevTask;
		if (nextTask != 0) nextTask->prevTask = prevTask;
		if (prevTask != 0) prevTask->nextTask = nextTask;
		if (taskList == currentTask) taskList = nextTask;
		
		if (currentTask->repeat) {
			currentTask->setNextCallbackTime(rtc.now());
			addTask(currentTask);
		}
		else {
			currentTask->nextTask = 0;
			currentTask->prevTask = 0;
			delete currentTask;
		}
		currentTask = nextTask;
	}
}

void RTTManagerClass::addTask(Task* task) {	
	Task* currentTask = taskList;
	Task* prevTask = 0;
	while (currentTask != 0 && currentTask->nextCallbackTime <= task->nextCallbackTime) {
		prevTask = currentTask;
		currentTask = currentTask->nextTask;
	}
	if (prevTask == 0)  {
		task->nextTask = taskList;
		if (taskList != 0) taskList->prevTask = task;
		taskList = task;		
	}
	else {
		task->prevTask = prevTask;
		task->nextTask = currentTask;
		prevTask->nextTask = task;
		if (currentTask != 0) currentTask->prevTask = task;
	}
}

RTTManagerClass RTTManager = RTTManagerClass();