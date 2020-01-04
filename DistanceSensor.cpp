#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(byte ultrasonicTrigPin, byte ultrasonicEchoPin, byte distanceThreshold)
	: m_ultrasonicTrigPin(ultrasonicTrigPin), m_ultrasonicEchoPin(ultrasonicEchoPin), m_distanceThreshold(distanceThreshold), m_isAboveThreshold(false), m_distanceValue(-1) {}
	// m_distanceValue set to '-1' mean no value has been read out

byte DistanceSensor::ReadDistance()
{
    digitalWrite(m_ultrasonicTrigPin, LOW);     // send sound waves to bounce at any object in front of the direction to where wultrasonic sensor is directed
    delayMicroseconds(5);
    digitalWrite(m_ultrasonicTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_ultrasonicTrigPin, LOW);
    return (pulseIn(m_ultrasonicEchoPin, HIGH) / 2) / 29.1;     // convert measured time delay, into a centimeter value, it takes for waves to bounce back 
}

bool DistanceSensor::GetDistanceStatus()
{
    byte distanceValue = DistanceSensor::ReadDistance();
    if (distanceValue > m_distanceThreshold)
    {
        m_isAboveThreshold = true;
    }
    else
        m_isAboveThreshold = false;

    return m_isAboveThreshold;
}

int DistanceSensor::GetDistanceValue()
{
    return DistanceSensor::ReadDistance();
}