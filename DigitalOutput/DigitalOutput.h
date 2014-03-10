/**
 * DigitalOutput.h provides an OO approach to handle digital outputs.
 *
 * DigitalOutput encapsulates a digital output.
 *
 * DigitalOutputController is DigitalOutput with operation levels: scheduled, manual or emergency.
 * 
 * Daniel Bergström
 * dabergst@kth.se
 */

#ifndef _DigitalOutput_H_
#define _DigitalOutput_H_

#include <stdint.h>

class DigitalOutput {
	const uint8_t pin;
public:
	DigitalOutput(const uint8_t pin);
	~DigitalOutput() {}
	void setState(uint8_t state);
	uint8_t getState() const;
};

class DigitalOutputController : public DigitalOutput {
	uint8_t scheduledState;
	uint8_t manualState;
	bool manualMode;
	bool emergencyMode;
public:
	DigitalOutputController(const uint8_t pin);
	~DigitalOutputController() {}	
	void scheduledSetState(uint8_t state);
	void manualSetState(uint8_t state);
	void emergencySetState(uint8_t state);
	void endManualMode();
	void endEmergencyMode();
private:
	DigitalOutput::setState;
};

#endif //_DigitalOutput_H_