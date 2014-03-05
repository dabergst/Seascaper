#include "rttask.h"
#include "Arduino.h"
#include<functional>
#include<RTClib.h>

RTTManagerClass RTTManager; // Create singleton of manager.

void RTTManagerClass::addTimeTask(void (*callback)(), uint8_t hour, uint8_t minute, uint8_t second, bool repeat, bool callRetroactive) {
	TimeTask* task = new TimeTask(callback, hour, minute, second, repeat);
	task->setNextCallbackTime(rtc.now());
	if (callRetroactive) 
		task->nextCallbackTime -= 86400; // Substract one day of time		
	addTask(task);
}

void RTTManagerClass::addDelayedTask(void (*callback)(), uint32_t seconds, bool repeat, bool callOnceImmediate) {
	DelayedTask* task = new DelayedTask(callback, seconds, repeat);
	if (callOnceImmediate)
		task->setNextCallbackTime(0);
	else
		task->setNextCallbackTime(rtc.now());
	addTask(task);
}

bool RTTManagerClass::removeTask(void (*callback)()) {
	Task* currentTask = taskList;
	Task* previousTask = NULL;	
	while (currentTask != NULL) {
		previousTask = currentTask;
		currentTask = currentTask->nextTask;
	}
	if (currentTask != (*task))
		return false;

	if (previousTask == NULL) 
		taskList = task->nextTask;
	else 
		previousTask->nextTask = task->nextTask;
	task->nextTask = 0;
	delete task;
	task = NULL;
	return true;
}

void RTTManagerClass::run() {
	uint32_t now = rtc.now().unixtime();
	Task* currentTask = taskList;
	Task* previousTask;

	while (currentTask != NULL && now >= currentTask->nextCallbackTime) {
		currentTask->callback();
		previousTask = currentTask;
		currentTask = currentTask->nextTask;
		taskList = currentTask;
		if (previousTask->repeat) {
			previousTask->setNextCallbackTime(rtc.now());
			addTask(previousTask);
		} else {
			previousTask->nextTask = 0;
			delete previousTask;
		}		
	}
}

void RTTManagerClass::addTask(Task* task) {	
	Task* currentTask = taskList;
	Task* previousTask = NULL;
	while (currentTask != NULL && currentTask->nextCallbackTime <= task->nextCallbackTime) {
		previousTask = currentTask;
		currentTask = currentTask->nextTask;
	}
	if (previousTask == NULL) 
		taskList = task;
	else 
		previousTask->nextTask = task;
	task->nextTask = currentTask;
}

void TimeTask::setNextCallbackTime(DateTime now) {
	nextCallbackTime = DateTime(now.year(), now.month(), now.day(), hour, minute, second).unixtime();
	if (nextCallbackTime < now.unixtime()) nextCallbackTime += 86400; // Add one day of time
}

void DelayedTask::setNextCallbackTime(DateTime now) {
	nextCallbackTime = now.unixtime() + seconds;
}