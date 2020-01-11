#include "Actuator.h"

Actuator::Actuator(const byte ledLightPin, const byte nLedAlarmLoops, const unsigned int ledAlarmTimePeriod, const byte buzzerSoundPin, const byte nBuzzerAlarmLoops, const unsigned int buzzerAlarmTimePeriod, AlarmType alarmTypeSelect, StateRegisterHandler* stateRegisterHandlerObj)
	: m_ledLightPin(ledLightPin), m_nLedAlarmLoops(nLedAlarmLoops), m_ledAlarmTimePeriod(ledAlarmTimePeriod), m_buzzerSoundPin(buzzerSoundPin), m_nBuzzerAlarmLoops(nBuzzerAlarmLoops), m_buzzerAlarmTimePeriod(buzzerAlarmTimePeriod), m_alarmTypeSelect(alarmTypeSelect), m_stateRegisterHandlerObj(stateRegisterHandlerObj)
{
	m_ledAlarmCnt = 0;
	m_buzzerAlarmCnt = 0;
	m_ledPreviousMillis = 0;
	m_buzzerPreviousMillis = 0;
	m_alarmTypeSelectCopy = m_alarmTypeSelect;	// store a copy of initialized value

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

void Actuator::DisableSelectedAlarm(AlarmType alarmType)
{
	// check which alarm types that was initialized when object was instatiated
	switch (m_alarmTypeSelect)
	{
	case LED_LIGHT_ALARM:		// only led light alarm was active
		m_alarmTypeSelect = 0;
		m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->ALARM_ENABLED);	// disable alarm function
		break;
	case BUZZER_ALARM:			// only buzzer alarms was active
		m_alarmTypeSelect = 0;
		m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->ALARM_ENABLED);	// disable alarm function
		break;
	case LED_AND_BUZZER_ALARM:	// both led light- and buzzer-alarm was active
		{
			if (alarmType == LED_LIGHT_ALARM)
			{
				m_alarmTypeSelect = BUZZER_ALARM;		// deactive led light alarm but keep buzzer alarm active
			}
			else	// BUZZER_ALARM is active
			{
				m_alarmTypeSelect = LED_LIGHT_ALARM;	// deactive buzzer alarm but keep led light alarm active
			}
		}
		break;
	}
}

void Actuator::CheckAlarmLoops()
{
	if (m_ledAlarmCnt == m_nLedAlarmLoops)
	{
		m_ledAlarmCnt = 0;		// reset counter
		DisableSelectedAlarm(LED_LIGHT_ALARM);
	}

	if (m_buzzerAlarmCnt == m_nBuzzerAlarmLoops)
	{
		m_buzzerAlarmCnt = 0;	// reset counter
		DisableSelectedAlarm(BUZZER_ALARM);
	}
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
				if (m_alarmTypeSelect == LED_LIGHT_ALARM || m_alarmTypeSelect == LED_AND_BUZZER_ALARM)	// check if led light alarm is enabled by alarm selector
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
						m_ledAlarmCnt++;	// increase led light alarm counter to keep track of the number of loops
					}
					Serial.print("m_alarmTypeSelect: ");
					Serial.println(m_alarmTypeSelect);
					Serial.print("m_ledAlarmCnt: ");
					Serial.println(m_ledAlarmCnt);
					Serial.print("m_buzzerAlarmCnt: ");
					Serial.println(m_buzzerAlarmCnt);
				}
			}

			// buzzer alarm control
			if(m_currentMillis - m_buzzerPreviousMillis >= m_buzzerAlarmTimePeriod)
			{
				if (m_alarmTypeSelect == BUZZER_ALARM || m_alarmTypeSelect == LED_AND_BUZZER_ALARM)	// check if buzzer alarm is enabled by alarm selector
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
						m_buzzerAlarmCnt++;		// increase buzzer alarm counter to keep track of the number of loops
					}
					Serial.print("m_alarmTypeSelect: ");
					Serial.println(m_alarmTypeSelect);
					Serial.print("m_ledAlarmCnt: ");
					Serial.println(m_ledAlarmCnt);
					Serial.print("m_buzzerAlarmCnt: ");
					Serial.println(m_buzzerAlarmCnt);
				}
			}
			CheckAlarmLoops();	// check if number of alarm loops for led light- and buzzer-alarm have respectively reached specified number of loops
			
			/*
			Serial.print("m_alarmTypeSelect: ");
			Serial.println(m_alarmTypeSelect);

			Serial.print("m_ledAlarmCnt: ");
			Serial.println(m_ledAlarmCnt);

			Serial.print("m_buzzerAlarmCnt: ");
			Serial.println(m_buzzerAlarmCnt);
			*/
		}
	}
}
