#pragma once

#include <Arduino.h>
#include "DistanceSensor.h"
#include "LightSensor.h"

class StateRegisterHandler
{
public:
	enum States : byte {
		RESET = 0,
		WORKPLACE_CHECK_ENABLED = 1 << 0,	// binary 0000'0001
		WORKPLACE_EMPTY = 1 << 1,			// binary 0000'0010
		MONITOR_ON = 1 << 2,				// binary 0000'0100
		ALARM_ENABLED = 1 << 3,				// binary 0000'1000
		TIMER_ENABLED = 1 << 4,				// binary 0001'0000
		TIMER_FINISHED = 1 << 5,			// binary 0010'0000
		LED_ALARM_ON = 1 << 6,				// binary 0100'0000
		BUZZER_ALARM_ON = 1 << 7,			// binary 1000'0000
		MASK = B11111111					// binary 1111'1111
	};
	enum StatesExtended : byte {
		RESET_EXTENDED = 0,
		WORK_DESK_ELEVATED = 1 << 0		// binary 0000'0001
	};
private:
	DistanceSensor* m_distanceSensorObj1;
	const byte m_nDistanceChecks;
	byte m_distanceAboveCnt;
	byte m_distanceBelowCnt;
	DistanceSensor* m_distanceSensorObj2;
	const byte m_nHeightChecks;
	unsigned int m_heightAboveCnt;
	unsigned int m_heightBelowCnt;
	LightSensor* m_lightSensorObj;
	const byte m_nLightChecks;
	byte m_lightBelowCnt;
	byte m_lightAboveCnt;
	
	byte m_stateRegister;
	byte m_stateRegisterExtended;
	byte m_forgotLockCnt;
	long m_elapsedTime;
	bool m_toggle;
public:
	StateRegisterHandler(DistanceSensor* distanceSensorObj1, const byte nDistanceChecks, DistanceSensor* distanceSensorObj2, const byte nHeightChecks, LightSensor* lightSensorObj, const byte nLightChecks);
	//StateRegisterHandler(const byte nDistanceChecks, const byte nLightChecks, DistanceSensor* distanceSensorObj, LightSensor* lightSensorObj);
	void SetFlagStateRegister(States);
	void ClearFlagStateRegister(States);
	bool CheckFlagStateRegister(States);
	byte& GetStateRegister();
	void SetFlagStateRegisterExtended(StatesExtended);
	void ClearFlagStateRegisterExtended(StatesExtended);
	bool CheckFlagStateRegisterExtended(StatesExtended);
	byte& GetStateRegisterExtended();
	void CheckWorkplace();
	byte& GetForgotLockCnt();
	long& GetElapsedTime();
	void IncreaseForgotLockCounter();
};