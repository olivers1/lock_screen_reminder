#include "StateRegisterHandler.h"

StateRegisterHandler::StateRegisterHandler(byte nDistanceChecks, byte nLightChecks, DistanceSensor* distanceSensorObj, LightSensor* lightSensorObj)
	: m_nDistanceChecks(nDistanceChecks), m_nLightChecks(nLightChecks), m_distanceSensorObj(distanceSensorObj), m_lightSensorObj(lightSensorObj), m_distanceAboveCnt(0), m_distanceBelowCnt(0), m_lightBelowCnt(0), m_lightAboveCnt(0), m_stateReg(RESET) {}


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
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= TIME_INTERVAL_1)
	{
		previousMillis = currentMillis;
		//while ((m_nDistanceChecks * 2) > 0)
		//{
		if (m_distanceSensorObj->GetDistanceStatus())
		{
			m_distanceAboveCnt++;
			m_distanceBelowCnt = 0;

			if (m_distanceAboveCnt == m_nDistanceChecks)
			{
				SetFlagStateRegister(WORKPLACE_EMPTY);
				m_distanceAboveCnt = 0;		// reset counter
			}
		}
		else
		{
			m_distanceBelowCnt++;
			m_distanceAboveCnt = 0;

			if (m_distanceBelowCnt == m_nDistanceChecks)
			{
				ClearFlagStateRegister(WORKPLACE_EMPTY);
				m_distanceBelowCnt = 0;		// reset counter
			}
		}
		//m_nDistanceChecks--;	
	//}

		Serial.print("stateReg: ");
		Serial.println(m_stateReg);

		Serial.print("m_distanceAboveCnt: ");
		Serial.println(m_distanceAboveCnt);

		Serial.print("m_distanceBelowCnt: ");
		Serial.println(m_distanceBelowCnt);
	}
}