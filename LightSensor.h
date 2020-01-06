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
	LightSensor(byte, unsigned int);
	bool GetLightStatus();
	int GetLightValue();
};

