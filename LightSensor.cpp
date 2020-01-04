#include "LightSensor.h"

LightSensor::LightSensor(byte lightSensorPin, byte lightThreshold)
	: m_lightSensorPin(lightSensorPin), m_lightThreshold(lightThreshold), m_isBelowThreshold(false), m_lightValue(-1) {}
	// m_lightValue set to '-1' mean no value has been read out

byte LightSensor::ReadLight()
{
	m_lightValue = analogRead(m_lightSensorPin);
	return m_lightValue;
}

bool LightSensor::GetLightStatus()
{
	byte lightValue = LightSensor::ReadLight();
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