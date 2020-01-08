#pragma once

#include <Arduino.h>
#include "StateRegisterHandler.h"

class Actuator
{
private:
	byte m_ledLightPin;
	const byte m_buzzerSoundPin;
	byte m_ledAlarmCnt;
	const byte m_nLedAlarmLoops;
	byte m_buzzerAlarmCnt;
	const byte m_nBuzzerAlarmLoops;
	byte m_alarmTypeSelect;
	const unsigned int m_alarmTimePeriod;
	StateRegisterHandler* m_stateRegisterHandlerObj;

	void ActivateLedAlarm();
	void DeactivateLedAlarm();
	void ActivateBuzzerAlarm();
	void DeactivateBuzzerAlarm();
public:
	Actuator(byte, byte, const byte, const byte, byte, const unsigned int, StateRegisterHandler*);
	bool CheckForAlarmActivation();
};

