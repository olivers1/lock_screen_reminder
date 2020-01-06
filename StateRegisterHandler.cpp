#include "StateRegisterHandler.h"

StateRegisterHandler::StateRegisterHandler(byte nDistanceChecks, byte nLightChecks)
	: m_nDistanceChecks(nDistanceChecks), m_nLightChecks(nLightChecks), m_distanceAboveCnt(0), m_distanceBelowCnt(0), m_lightBelowCnt(0), m_lightAboveCnt(0), m_stateReg(RESET) {}


void StateRegisterHandler::SetFlagStateRegister(States flag)
{
	m_stateReg |= flag;
}

void StateRegisterHandler::ClearFlagStateRegister(States flag)
{
	m_stateReg &= (MASK ^ flag);
}

void StateRegisterHandler::CheckWorkplace()
{
	Serial.print("stateReg0: ");
	Serial.println(m_stateReg);

	SetFlagStateRegister(WORKPLACE_EMPTY);
	Serial.print("stateReg1: ");
	Serial.println(m_stateReg);
	delay(5000);

	SetFlagStateRegister(ALARM_DISABLED);
	Serial.print("stateReg2: ");
	Serial.println(m_stateReg);
	delay(3000);

	ClearFlagStateRegister(WORKPLACE_EMPTY);
	Serial.print("stateReg3: ");
	Serial.println(m_stateReg);
	delay(3000);

	SetFlagStateRegister(MONITOR_ON);
	Serial.print("stateReg4: ");
	Serial.println(m_stateReg);
	delay(3000);

	m_stateReg = RESET;
	Serial.print("stateReg5: ");
	Serial.println(m_stateReg);
	delay(3000);
}