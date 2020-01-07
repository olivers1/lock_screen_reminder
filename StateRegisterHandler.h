#pragma once

#include <Arduino.h>
#include "DistanceSensor.h"
#include "LightSensor.h"

class StateRegisterHandler
{
private:
	const unsigned int TIME_INTERVAL_1 = 1000;   // interval for how often (in milliseconds) program should loop
	unsigned long previousMillis = 0;
	byte m_distanceAboveCnt;
	byte m_distanceBelowCnt;
	const byte m_nDistanceChecks;
	byte m_lightBelowCnt;
	byte m_lightAboveCnt;
	const byte m_nLightChecks;
	byte m_stateReg;

	enum States : byte {
		RESET = 0,
		WORKPLACE_EMPTY = 1 << 0,   // binary 0000'0001
		MONITOR_ON = 1 << 1,        // binary 0000'0010
		TIMER_ENABLED = 1 << 2,     // binary 0000'0100
		TIMER_FINISHED = 1 << 3,    // binary 0000'1000
		ALARM_DISABLED = 1 << 4,    // binary 0001'0000
		AUDIO_VISUAL_ON = 1 << 5,   // binary 0010'0000
		MASK = B11111111            // binary 1111'1111
	};

	DistanceSensor* m_distanceSensorObj;
	LightSensor* m_lightSensorObj;

	void SetFlagStateRegister(States);
	void ClearFlagStateRegister(States);
public:
	StateRegisterHandler(byte, byte, DistanceSensor*, LightSensor*);
	void CheckWorkplace();
};