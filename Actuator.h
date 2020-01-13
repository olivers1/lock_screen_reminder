#pragma once

#include <Arduino.h>
#include "StateRegisterHandler.h"

class Actuator
{
public:
	enum AlarmType : byte {
		NO_ALARM = 0,
		LED_LIGHT_ALARM = 1,
		BUZZER_ALARM = 2,
		LED_AND_BUZZER_ALARM = 3
	};
private:
	const byte m_ledLightPin;
	const byte m_nLedAlarmLoops;
	const unsigned int m_ledAlarmTimePeriod;
	const byte m_buzzerSoundPin;
	const byte m_nBuzzerAlarmLoops;
	const unsigned int m_buzzerAlarmTimePeriod;
	byte m_alarmTypeSelect;
	StateRegisterHandler* m_stateRegisteristerHandlerObj;
	byte m_ledAlarmCnt;
	byte m_buzzerAlarmCnt;
	unsigned long m_ledPreviousMillis;
	unsigned long m_buzzerPreviousMillis;
	byte m_alarmTypeSelectCopy;

	void LedAlarmOn();
	void LedAlarmOff(); 
	void BuzzerAlarmOn();
	void BuzzerAlarmOff();
	void DisableSelectedAlarm(AlarmType);
	void CheckAlarmLoops();
public:
	Actuator(const byte ledLightPin, const byte nLedAlarmLoops, const unsigned int ledAlarmTimePeriod, const byte buzzerSoundPin, const byte nBuzzerAlarmLoops, const unsigned int buzzerAlarmTimePeriod, AlarmType alarmTypeSelect, StateRegisterHandler* stateRegisterHandlerObj);
	void AlarmActivationHandler();
};

