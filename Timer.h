#pragma once

#include "Arduino.h"
#include "StateRegisterHandler.h"

class Timer
{
	const unsigned long m_timerPeriod;
	StateRegisterHandler* m_stateRegisterHandlerObj;
	unsigned long m_timeNow;

	void StartTimer();
	void AbortTimer();
public:
	Timer(const unsigned long timerPeriod, StateRegisterHandler* stateRegisterHandlerObj);
	long TimeLeft(bool);
	void TimerActivationHandler();
};

