#pragma once

#include <Arduino.h>
#include "DistanceSensor.h"
#include "LightSensor.h"

class StateRegisterHandler
{
public:
	enum States : byte {
		RESET = 0,
		WORKPLACE_EMPTY = 1 << 0,   // binary 0000'0001
		MONITOR_ON = 1 << 1,        // binary 0000'0010
		TIMER_ENABLED = 1 << 2,     // binary 0000'0100
		TIMER_FINISHED = 1 << 3,    // binary 0000'1000
		ALARM_ENABLED  = 1 << 4,	// binary 0001'0000
		LED_ALARM_ON = 1 << 5,		// binary 0010'0000
		BUZZER_ALARM_ON = 1 << 6,	// binary 0100'0000
		MASK = B11111111            // binary 1111'1111
	};
private:
	byte m_distanceAboveCnt;
	byte m_distanceBelowCnt;
	const byte m_nDistanceChecks;
	byte m_lightBelowCnt;
	byte m_lightAboveCnt;
	const byte m_nLightChecks;
	byte m_stateReg;
	DistanceSensor* m_distanceSensorObj;
	LightSensor* m_lightSensorObj;
public:
	StateRegisterHandler(const byte nDistanceChecks, const byte nLightChecks, DistanceSensor* distanceSensorObj, LightSensor* lightSensorObj);
	void SetFlagStateRegister(States);
	void ClearFlagStateRegister(States);
	bool CheckFlagStateRegister(States); 
	void CheckWorkplace();
};