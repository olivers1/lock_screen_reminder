#pragma once

#include <Arduino.h>

class DistanceSensor
{
private:
	const byte m_ultrasonicTrigPin;
	const byte m_ultrasonicEchoPin;
	const unsigned int m_distanceThreshold;
	bool m_isAboveThreshold;

	unsigned int ReadDistance();
public:
	DistanceSensor(const byte ultrasonicTrigPin, const byte ultrasonicEchoPin, const unsigned int distanceThreshold);
	bool& GetDistanceStatus();
	unsigned int GetDistanceValue();
};

