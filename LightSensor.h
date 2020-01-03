#pragma once

#include <Arduino.h>

class LightSensor
{
private:
	byte m_lightSensorPin;
	byte m_lightThreshold;
	bool m_isBelowThreshold;
	int m_lightValue;

	int ReadLight();
public:
	LightSensor(byte, byte);
	const bool GetLightStatus();
	const int GetLightValue();
};

