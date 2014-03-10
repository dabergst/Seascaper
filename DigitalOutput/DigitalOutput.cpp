/**
 * DigitalOutput.cpp
 *
 * Daniel Bergström
 * dabergst@kth.se
 */

#include "DigitalOutput.h"
#include "Arduino.h"

DigitalOutput::DigitalOutput(const uint8_t pin) : pin(pin) {
	pinMode(pin, OUTPUT);
	setState(LOW);
}

void DigitalOutput::setState(uint8_t state) {
	digitalWrite(pin, state);
}

uint8_t DigitalOutput::getState() const {
	return digitalRead(pin);
}


DigitalOutputController::DigitalOutputController(const uint8_t pin) : DigitalOutput(pin), manualMode(false), emergencyMode(false) {}

void DigitalOutputController::scheduledSetState(uint8_t state) {
	if (!manualMode && !emergencyMode) setState(state);
	scheduledState = state;
}

void DigitalOutputController::manualSetState(uint8_t state) {
	manualMode = true;
	if (!emergencyMode) setState(state);
	manualState = state;
}

void DigitalOutputController::emergencySetState(uint8_t state) {
	emergencyMode = true;
	setState(state);
}

void DigitalOutputController::endManualMode() {
	manualMode = false;
	if (!emergencyMode) setState(scheduledState);
}

void DigitalOutputController::endEmergencyMode() {
	emergencyMode = false;
	if (manualMode) setState(manualState);
	else setState(scheduledState);
}