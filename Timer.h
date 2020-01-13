#pragma once
class Timer
{
	const unsigned long m_timerPeriod;
	StateRegisterHandler* m_stateRegisterHandlerObj;
	unsigned long timeNow;

	void ActivateTimer();
public:
	Timer(const unsigned long timerPeriod, StateRegisterHandler* stateRegisterHandlerObj);
	unsigned long TimeLeft();
	void TimerActivationHandler();
};

