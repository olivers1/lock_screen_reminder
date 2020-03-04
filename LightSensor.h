#pragma once

#include <Arduino.h>

class LightSensor
{
private:
	const byte m_lightSensorPin;
	const unsigned int m_lightThreshold;
	bool m_isBelowThreshold;

	unsigned int ReadLight();
public:
	LightSensor(const byte lightSensorPin, const unsigned int lightThreshold);
	bool& GetLightStatus();
	int GetLightValue();
};

