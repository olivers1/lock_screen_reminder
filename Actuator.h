#pragma once

#include <Arduino.h>
#include "StateRegisterHandler.h"

class Actuator
{
private:
	byte m_ledLightPin;
	const byte m_buzzerSoundPin;
	const byte m_nLedAlarmLoops;
	const byte m_nBuzzerAlarmLoops;
	byte m_alarmTypeSelect;
	const unsigned int m_alarmTimePeriod;
	StateRegisterHandler* m_stateRegisterHandlerObj;
	byte m_ledAlarmCnt;
	byte m_buzzerAlarmCnt;
	unsigned long m_currentMillis;
	unsigned long m_previousMillis;

	void LedAlarmOn();
	void LedAlarmOff();
	void BuzzerAlarmOn();
	void BuzzerAlarmOff();
	unsigned int m_timePeriodLeft();
public:
	Actuator(const byte ledLightPin, const byte buzzerSoundPin, const byte nLedAlarmLoops, const byte nBuzzerAlarmLoops, byte alarmTypeSelect, const unsigned int alarmTimePeriod, StateRegisterHandler* StateRegisterHandler);
	bool AlarmActivationHandler();
};

