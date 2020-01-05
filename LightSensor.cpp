#include "LightSensor.h"

LightSensor::LightSensor(byte lightSensorPin, unsigned int lightThreshold)
	: m_lightSensorPin(lightSensorPin), m_lightThreshold(lightThreshold), m_isBelowThreshold(false) {}
	// m_lightValue set to '-1' mean no value has been read out

unsigned int LightSensor::ReadLight()
{
	return analogRead(m_lightSensorPin);
}

bool LightSensor::GetLightStatus()
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