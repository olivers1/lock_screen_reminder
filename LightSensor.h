#pragma once

#include <Arduino.h>

class LightSensor
{
private:
	const byte m_lightSensorPin;
	const byte m_lightThreshold;
	bool m_isBelowThreshold;
	int m_lightValue;

	byte ReadLight();
public:
	LightSensor(byte, byte);
	bool GetLightStatus();
	int GetLightValue();
};

