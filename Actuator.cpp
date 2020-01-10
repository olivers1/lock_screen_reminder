#include "Actuator.h"

Actuator::Actuator(const byte ledLightPin, const byte nLedAlarmLoops, const unsigned int ledAlarmTimePeriod, const byte buzzerSoundPin, const byte nBuzzerAlarmLoops, const unsigned int buzzerAlarmTimePeriod, byte alarmTypeSelect, StateRegisterHandler* stateRegisterHandlerObj)
	: m_ledLightPin(ledLightPin), m_nLedAlarmLoops(nLedAlarmLoops), m_ledAlarmTimePeriod(ledAlarmTimePeriod), m_buzzerSoundPin(buzzerSoundPin), m_nBuzzerAlarmLoops(nBuzzerAlarmLoops), m_buzzerAlarmTimePeriod(buzzerAlarmTimePeriod), m_alarmTypeSelect(alarmTypeSelect), m_stateRegisterHandlerObj(stateRegisterHandlerObj)
{
	m_ledAlarmCnt = 0;
	m_buzzerAlarmCnt = 0;
	m_ledPreviousMillis = 0;
	m_buzzerPreviousMillis = 0;

	// catch any faulty parameter input
	if (ledAlarmTimePeriod > 1000 || buzzerAlarmTimePeriod > 300 || (alarmTypeSelect > 3))
	{
		Serial.println("Actuator object initializing failed");
		if (ledAlarmTimePeriod > 1000)
		{
			Serial.println("ledAlarmTimePeriod parameter value is invalid, must be < 1000");
		}
		else if (buzzerAlarmTimePeriod > 300)
		{
			Serial.println("buzzerAlarmTimePeriod parameter value is invalid, must be < 300");
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

bool Actuator::AlarmActivationHandler()
{
	// check if 'alarm enabled' flag is set?
	if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->ALARM_ENABLED))
	{
		// yes, check if 'timer finished' flag is set?
		if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->TIMER_FINISHED))
		{
			// yes, countdown timer is finished. activate alarm
			unsigned long m_currentMillis = millis();
			
			// led light alarm control
			if (m_currentMillis - m_ledPreviousMillis >= m_ledAlarmTimePeriod)
			{
				m_ledPreviousMillis = m_currentMillis;
				if (!(m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON)))
				{
					LedAlarmOn();		// turn on alarm device
					m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->LED_ALARM_ON);
					Serial.println("alarm: LED");
				}
				else
				{
					LedAlarmOff();		// turn off alarm devices
					m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
					Serial.println("silent alarm: LED");
				}
			}

				// buzzer alarm control
			if(m_currentMillis - m_buzzerPreviousMillis >= m_buzzerAlarmTimePeriod)
			{
				m_buzzerPreviousMillis = m_currentMillis;
				if (!(m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON)))
				{
					BuzzerAlarmOn();	// turn on alarm devices
					m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
					Serial.println("alarm: BUZZER");
				}
				else
				{
					BuzzerAlarmOff();	// turn on alarm devices
					m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->BUZZER_ALARM_ON);
					Serial.println("silent alarm: BUZZER");
				}
			}
		}
	}
	// wait for the countdown timer to finish. read stateRegister to see when countdowntimer is finished 
	// add millis() here to activate and deactivate ledLightAlarm and buzzerSoundAlarm respectively according to predefined timer interval
}
