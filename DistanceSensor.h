#pragma once

#include <Arduino.h>

class DistanceSensor
{
private:
	const byte m_ultrasonicTrigPin;
	const byte m_ultrasonicEchoPin;
	const unsigned int m_distanceThreshold;
	bool m_isAboveThreshold;
	//unsigned int m_distanceValue;

	unsigned int ReadDistance();
public:
	DistanceSensor(byte, byte, unsigned int);
	bool GetDistanceStatus();
	unsigned int GetDistanceValue();
};

