#include "Actuator.h"

Actuator::Actuator(const byte ledLightPin, const byte buzzerSoundPin, const byte nLedAlarmLoops, const byte nBuzzerAlarmLoops, byte alarmTypeSelect, const unsigned int alarmTimePeriod, StateRegisterHandler* stateRegisterHandlerObj)
	: m_ledLightPin(ledLightPin), m_buzzerSoundPin(buzzerSoundPin), m_nLedAlarmLoops(nLedAlarmLoops), m_nBuzzerAlarmLoops(nBuzzerAlarmLoops), m_alarmTypeSelect(alarmTypeSelect), m_alarmTimePeriod(alarmTimePeriod), m_stateRegisterHandlerObj(stateRegisterHandlerObj)  {}

void Actuator::ActivateLedAlarm()
{
	digitalWrite(m_ledLightPin, HIGH);
	m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
}

void Actuator::DeactivateLedAlarm()
{
	digitalWrite(m_ledLightPin, LOW);
	m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
}

void Actuator::ActivateBuzzerAlarm()
{
	digitalWrite(m_buzzerSoundPin, HIGH);
	m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
}

void Actuator::DeactivateBuzzerAlarm()
{
	digitalWrite(m_buzzerSoundPin, LOW);
	m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
}

bool Actuator::CheckForAlarmActivation()
{
	// wait for the countdown timer to finish. read stateRegister to see when countdowntimer is finished 
	// add millis() here to activate and deactivate ledLightAlarm and buzzerSoundAlarm respectively according to predefined timer interval
}
