#include "Sensor.h"

using namespace std;

unsigned int Sensor::ReadLight()
{
	return analogRead(lightSensorPin);
}

unsigned int Sensor::ReadDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    return (pulseIn(echoPin, HIGH) / 2) / 29.1;
}