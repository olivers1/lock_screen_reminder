#pragma once

#include <Arduino.h>

class LightSensor
{
private:
	unsigned int m_lightSensorPin;
	unsigned int m_lightThreshold;
	bool m_isBelowThreshold;
	int m_lightValue;

	int ReadLight();
public:
	LightSensor(unsigned int, unsigned int);
	const bool GetLightStatus();
	const int GetLightValue();
};

