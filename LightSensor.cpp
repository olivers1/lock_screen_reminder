#include "LightSensor.h"

LightSensor::LightSensor(unsigned int lightSensorPin, unsigned int lightThreshold)
	: m_lightSensorPin(lightSensorPin), m_lightThreshold(lightThreshold), m_isBelowThreshold(false), m_lightValue(-1) {}

int LightSensor::ReadLight()
{
	m_lightValue = analogRead(m_lightSensorPin);
	return m_lightValue;
}

const bool LightSensor::GetLightStatus()
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

const int LightSensor::GetLightValue()
{
	return LightSensor::ReadLight();
}