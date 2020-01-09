#include "Actuator.h"

Actuator::Actuator(const byte ledLightPin, const byte buzzerSoundPin, const byte nLedAlarmLoops, const byte nBuzzerAlarmLoops, byte alarmTypeSelect, const unsigned int alarmTimePeriod, StateRegisterHandler* stateRegisterHandlerObj)
	: m_ledLightPin(ledLightPin), m_buzzerSoundPin(buzzerSoundPin), m_nLedAlarmLoops(nLedAlarmLoops), m_nBuzzerAlarmLoops(nBuzzerAlarmLoops), m_alarmTypeSelect(alarmTypeSelect), m_alarmTimePeriod(alarmTimePeriod), m_stateRegisterHandlerObj(stateRegisterHandlerObj)
{
	m_ledAlarmCnt = 0;
	m_buzzerAlarmCnt = 0;
	m_currentMillis = 0;
	m_previousMillis = 0;

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


void Actuator::LedAlarmOn()
{
	digitalWrite(m_ledLightPin, HIGH);
	m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
}

void Actuator::LedAlarmOff()
{
	digitalWrite(m_ledLightPin, LOW);
	m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
}

void Actuator::BuzzerAlarmOn()
{
	digitalWrite(m_buzzerSoundPin, HIGH);
	m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
}

void Actuator::BuzzerAlarmOff()
{
	digitalWrite(m_buzzerSoundPin, LOW);
	m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
}

unsigned int Actuator::m_timePeriodLeft()
{
	return m_currentMillis - m_previousMillis;
}

bool Actuator::AlarmActivationHandler()
{
	// check if 'alarm enabled' flag is set?
	if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->ALARM_ENABLED))
	{
		// yes, check if 'timer finished' flag is set?
		if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->TIMER_FINISHED))
		{
			// yes, countdown timer is finished. activate alarm
			m_currentMillis = millis();
			
			if (m_timePeriodLeft() >= m_alarmTimePeriod)
			{
				m_previousMillis = m_currentMillis;
				if (!(m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON)))
				{
					LedAlarmOn();		// turn on alarm devices
					BuzzerAlarmOn();
					m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
					m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
					Serial.println("ALARM");
				}
				else
				{
					LedAlarmOff();		// turn off alarm devices
					BuzzerAlarmOff();
					m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
					m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
					Serial.println("silent_'ALARM'");
				}
			}
		}
	}
	// wait for the countdown timer to finish. read stateRegister to see when countdowntimer is finished 
	// add millis() here to activate and deactivate ledLightAlarm and buzzerSoundAlarm respectively according to predefined timer interval
}
