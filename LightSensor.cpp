#include "LightSensor.h"

LightSensor::LightSensor(const byte lightSensorPin, const unsigned int lightThreshold)
	: m_lightSensorPin(lightSensorPin), m_lightThreshold(lightThreshold), m_isBelowThreshold(false) {}

unsigned int LightSensor::ReadLight()
{
	return analogRead(m_lightSensorPin);
}

bool& LightSensor::GetLightStatus()
{
	unsigned int lightValue = LightSensor::ReadLight();
	if (lightValue < m_lightThreshold)
	{
		m_isBelowThreshold = true;
	}
	else
	{
		m_isBelowThreshold = false;
	}
	return m_isBelowThreshold;
}

int LightSensor::GetLightValue()
{
	return LightSensor::ReadLight();
}