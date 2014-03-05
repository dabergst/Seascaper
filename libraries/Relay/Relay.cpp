#include "Relay.h"
#include "Arduino.h"

Relay::Relay(const uint8_t pin) : pin(pin) {
	pinMode(pin, OUTPUT);
	setRelayState(OFF);
}

void Relay::setRelayState(RelayState state) {
	switch (state) {
	case ON:
		digitalWrite(pin, HIGH);
		break;
	case OFF:
		digitalWrite(pin, LOW);
		break;
	}
	this->state = state;
}

RelayState Relay::getRelayState() const {
	return state;
}



ScheduledRelay::ScheduledRelay(const uint8_t pin) : Relay(pin), manualMode(false), emergencyMode(false) {}

void ScheduledRelay::scheduledSetState(RelayState state) {
	if (!manualMode && !emergencyMode) setRelayState(state);
	scheduledState = state;
}

void ScheduledRelay::manualSetState(RelayState state) {
	manualMode = true;
	if (!emergencyMode) setRelayState(state);
	manualState = state;
}

void ScheduledRelay::emergencySetState(RelayState state) {
	emergencyMode = true;
	setRelayState(state);
}

void ScheduledRelay::endManualMode() {
	manualMode = false;
	if (!emergencyMode) setRelayState(scheduledState);
}

void ScheduledRelay::endEmergencyMode() {
	emergencyMode = false;
	if (manualMode) setRelayState(manualState);
	else setRelayState(scheduledState);
}