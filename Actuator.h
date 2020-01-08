#pragma once

#include <Arduino.h>
#include "StateRegisterHandler.h"

class Actuator
{
private:
	const byte m_ledLightPin;
	const byte m_buzzerSoundPin;
	const byte m_nLedAlarmLoops;
	const byte m_nBuzzerAlarmLoops;
	byte m_alarmTypeSelect;
	const unsigned int m_alarmTimePeriod;
	StateRegisterHandler* m_stateRegisterHandlerObj;

	void ActivateLedAlarm();
	void DeactivateLedAlarm();
	void ActivateBuzzerAlarm();
	void DeactivateBuzzerAlarm();
public:
	Actuator(const byte, const byte, const byte, const byte, byte, const unsigned int, StateRegisterHandler*);
	bool CheckForAlarmActivation();
};

