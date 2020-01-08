#include "Actuator.h"

Actuator::Actuator(const byte ledLightPin, const byte buzzerSoundPin, const byte nLedAlarmLoops, const byte nBuzzerAlarmLoops, byte alarmTypeSelect, const unsigned int alarmTimePeriod, StateRegisterHandler* stateRegisterHandlerObj)
	: m_ledLightPin(ledLightPin), m_buzzerSoundPin(buzzerSoundPin), m_nLedAlarmLoops(nLedAlarmLoops), m_nBuzzerAlarmLoops(nBuzzerAlarmLoops), m_alarmTypeSelect(alarmTypeSelect), m_alarmTimePeriod(alarmTimePeriod), m_stateRegisterHandlerObj(stateRegisterHandlerObj)
{
	m_ledAlarmCnt = 0;
	m_buzzerAlarmCnt = 0;

	// catch faulty parameter input
	if (alarmTimePeriod > 1000 || (alarmTypeSelect > 3))
	{
		Serial.println("Actuator object initializing failed");
		if (alarmTimePeriod > 1000)
		{
			Serial.println("alarmTimePeriod parameter value is invalid, must be < 1000");
		}
		else if(alarmTypeSelect > 3)
		{
			Serial.println("alarmTypeSelect parameter value is invalid, must be <= 3");
		}
		for (;;);	// Don't proceed, loop forever
	}
}


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
	if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->ALARM_ENABLED))
	{
		Serial.println("hello");
	}
	// wait for the countdown timer to finish. read stateRegister to see when countdowntimer is finished 
	// add millis() here to activate and deactivate ledLightAlarm and buzzerSoundAlarm respectively according to predefined timer interval
}
