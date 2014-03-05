#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>

enum RelayState { OFF, ON };

class Relay {
	const uint8_t pin;
	RelayState state;
public:
	Relay(const uint8_t pin);
	~Relay() {}
	void setRelayState(RelayState state);
	RelayState getRelayState() const;
};

class ScheduledRelay : public Relay {
	RelayState scheduledState;
	RelayState manualState;
	bool manualMode;
	bool emergencyMode;
public:
	ScheduledRelay(const uint8_t pin);
	~ScheduledRelay() {}	
	void scheduledSetState(RelayState state);
	void manualSetState(RelayState state);
	void emergencySetState(RelayState state);
	void endManualMode();
	void endEmergencyMode();
private:
	Relay::setRelayState;
};

#endif //RELAY_H