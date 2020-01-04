#pragma once

#include <Arduino.h>

class DistanceSensor
{
private:
	const byte m_ultrasonicTrigPin;
	const byte m_ultrasonicEchoPin;
	const byte m_distanceThreshold;
	bool m_isAboveThreshold;
	int m_distanceValue;

	byte ReadDistance();
public:
	DistanceSensor(byte, byte, byte);
	bool GetDistanceStatus();
	int GetDistanceValue();
};

