#pragma once

#include <Arduino.h>

class StateRegisterHandler
{
private:
	byte m_distanceAboveCnt;
	byte m_distanceBelowCnt;
	const byte m_nDistanceChecks;
	byte m_lightBelowCnt;
	byte m_lightAboveCnt;
	const byte m_nLightChecks;

	//void CheckFlagStateRegister(byte);	// maybe change to string parameter instead
	//void SetFlagStateRegister(States);
	void ClearFlagStateRegister(byte);
public:
	StateRegisterHandler(byte, byte);
	void CheckWorkplace();
};

