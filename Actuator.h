#pragma once

#include <Arduino.h>
#include "StateRegisterHandler.h"

class Actuator
{
private:
	const byte m_ledLightPin;
	const byte m_nLedAlarmLoops;
	const unsigned int m_ledAlarmTimePeriod;
	const byte m_buzzerSoundPin;
	const byte m_nBuzzerAlarmLoops;
	const unsigned int m_buzzerAlarmTimePeriod;
	byte m_alarmTypeSelect;
	StateRegisterHandler* m_stateRegisterHandlerObj;
	byte m_ledAlarmCnt;
	byte m_buzzerAlarmCnt;
	unsigned long m_ledPreviousMillis;
	unsigned long m_buzzerPreviousMillis;

	void LedAlarmOn();
	void LedAlarmOff();
	void BuzzerAlarmOn();
	void BuzzerAlarmOff();
public:
	Actuator(const byte ledLightPin, const byte nLedAlarmLoops, const unsigned int ledAlarmTimePeriod, const byte buzzerSoundPin, const byte nBuzzerAlarmLoops, const unsigned int buzzerAlarmTimePeriod, byte alarmTypeSelect, StateRegisterHandler* stateRegisterHandlerObj);
	bool AlarmActivationHandler();
};

