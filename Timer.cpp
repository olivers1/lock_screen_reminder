#include "Timer.h"

Timer::Timer(const unsigned long timerPeriod, StateRegisterHandler* stateRegisterHandlerObj)
	: m_timerPeriod(timerPeriod), m_stateRegisterHandlerObj(stateRegisterHandlerObj)
{
	m_timeNow = 0;
}


void Timer::ActivateTimer()
{
	m_timeNow = millis();	// store current time
	m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->TIMER_ENABLED);	// set 'timer enabled' flag to indicate timer is running
}


long Timer::TimeLeft(bool inSeconds)
{
	unsigned long timeLeft;
	// check if timer is counting?
	if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->TIMER_ENABLED))
	{
		// yes, return remaining time
		if (inSeconds)
		{
			timeLeft = (m_timerPeriod + m_timeNow - millis()) / 1000;	// returns the timer's remaining time before finish, in seconds
		}
		else
		{
			timeLeft = m_timerPeriod + m_timeNow - millis();	// returns the timer's remaining time before finish, in milliseconds
		}
	}
	else
	{
		// no, return -1
		timeLeft = -1;
	}
	return timeLeft;
}

void Timer::TimerActivationHandler()
{
	// check if 'alarm enabled' flag is set?
	if (m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->ALARM_ENABLED))
	{
		// yes, check if timer has started counting?
		if (!(m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->TIMER_ENABLED)))
		{
			// no, start timer
			ActivateTimer();
		}
		else
		{
			// yes, check if timer has finished counting?
			if (TimeLeft(false) <= 0)
			{
				// yes, timer is finished
				m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->TIMER_FINISHED);		// set 'timer finished' flag
			}
			else
			{
				Serial.print("timeLeft: ");
				Serial.println(TimeLeft(true));
			}
		}
	}

}