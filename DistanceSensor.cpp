#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(byte ultrasonicTrigPin, byte ultrasonicEchoPin, unsigned int distanceThreshold)
	: m_ultrasonicTrigPin(ultrasonicTrigPin), m_ultrasonicEchoPin(ultrasonicEchoPin), m_distanceThreshold(distanceThreshold), m_isAboveThreshold(false) {}

unsigned int DistanceSensor::ReadDistance()
{
    digitalWrite(m_ultrasonicTrigPin, LOW);     // send sound waves to bounce at any object in front of the direction to where wultrasonic sensor is directed
    delayMicroseconds(5);
    digitalWrite(m_ultrasonicTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_ultrasonicTrigPin, LOW);
    unsigned int distanceValue = (pulseIn(m_ultrasonicEchoPin, HIGH) / 2) / 29.1;     // convert measured time delay, into a centimeter value, it takes for waves to bounce back 
    delay(10);      // delay to enable sequentional readouts, since arduino cpu executes code faster than the capability of the ultrasonic sensor
    return distanceValue;
}

bool DistanceSensor::GetDistanceStatus()
{
    unsigned int distanceValue = DistanceSensor::ReadDistance();
    if (distanceValue > m_distanceThreshold)
    {
        m_isAboveThreshold = true;
    }
    else
    {
        m_isAboveThreshold = false;
    }
    return m_isAboveThreshold;
}

unsigned int DistanceSensor::GetDistanceValue()
{
    return DistanceSensor::ReadDistance();
}