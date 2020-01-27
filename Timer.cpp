#include "Timer.h"

Timer::Timer(const unsigned long timerPeriod, StateRegisterHandler* stateRegisterHandlerObj)
	: m_timerPeriod(timerPeriod), m_stateRegisterHandlerObj(stateRegisterHandlerObj)
{
	m_timeNow = 0;
}


void Timer::StartTimer()
{
	m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->TIMER_ENABLED);	// set 'timer enabled' flag to indicate timer is running
	m_timeNow = millis();	// store current time
}

void Timer::AbortTimer()
{
	m_stateRegisterHandlerObj->ClearFlagStateRegister(m_stateRegisterHandlerObj->TIMER_ENABLED);	// set 'timer enabled' flag to indicate timer is running
	m_timeNow = 0;	// reset timer
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
			StartTimer();
		}
		else
		{
			// yes, check if timer has finished counting?
			if (TimeLeft(false) <= 0)
			{
				// yes, timer is finished
				m_stateRegisterHandlerObj->IncreaseForgotLockCounter();		// increase counter that keeps track of number of times user forgot to lock computer when leaving his/her workplace
				m_stateRegisterHandlerObj->SetFlagStateRegister(m_stateRegisterHandlerObj->TIMER_FINISHED);		// set 'timer finished' flag
			}
			else
			{
				// timer is still counting print current time
				Serial.print("timeLeft: ");
				Serial.println(TimeLeft(true));
			}
		}
	}
	else
	{
		// check if user is back at his/her workplace or external monitor is turned off?
		if (!(m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->WORKPLACE_EMPTY)) || !(m_stateRegisterHandlerObj->CheckFlagStateRegister(m_stateRegisterHandlerObj->MONITOR_ON)))
		{
			// yes, abort timer
			AbortTimer();
		}
	}
}