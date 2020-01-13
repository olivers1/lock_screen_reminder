#include "Timer.h"

Timer::Timer(const unsigned long timerPeriod, StateRegisterHandler* stateRegisterHandlerObj)
	: m_timerPeriod(timerPeriod), m_stateRegisterHandlerObj(stateRegisterHandlerObj) {}


void Timer::ActivateTimer()
{

}


unsigned long Timer::TimeLeft()
{

}

void Timer::TimerActivationHandler()
{

}

// Remove TIMER_FINISHED flag set in StateRegisterHandler.cpp