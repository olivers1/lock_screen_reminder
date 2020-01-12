#include "StateRegisterHandler.h"

StateRegisterHandler::StateRegisterHandler(const byte nDistanceChecks, const byte nLightChecks, DistanceSensor* distanceSensorObj, LightSensor* lightSensorObj)
	: m_nDistanceChecks(nDistanceChecks), m_nLightChecks(nLightChecks), m_distanceSensorObj(distanceSensorObj), m_lightSensorObj(lightSensorObj)
{
	m_distanceAboveCnt = 0;
	m_distanceBelowCnt = 0;
	m_lightBelowCnt = 0;
	m_lightAboveCnt = 0;
	m_stateReg = WORKPLACE_CHECK_ENABLED;	// enable check of workplace to determine if it is empty
}


void StateRegisterHandler::SetFlagStateRegister(States flag)
{
	m_stateReg |= flag;
}

void StateRegisterHandler::ClearFlagStateRegister(States flag)
{
	m_stateReg &= (MASK ^ flag);
}

bool StateRegisterHandler::CheckFlagStateRegister(States flag)
{
	return m_stateReg & flag;
}

void StateRegisterHandler::CheckWorkplace()
{
	// check if workplace is empty and update stateRegister accordingly
	if (m_distanceSensorObj->GetDistanceStatus())
	{
		if (CheckFlagStateRegister(WORKPLACE_CHECK_ENABLED))
		{
			m_distanceAboveCnt++;
			m_distanceBelowCnt = 0;

			if (m_distanceAboveCnt == m_nDistanceChecks)
			{
				SetFlagStateRegister(WORKPLACE_EMPTY);
				m_distanceAboveCnt = 0;		// reset counter
			}
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
			SetFlagStateRegister(WORKPLACE_CHECK_ENABLED);	// set 'workplace check' flag to enable workplace check when the workplace has been confirmed to be reoccupied after an alarm has been trigged
		}
	}

	// check if 'workplace empty' flag is set?
	if (StateRegisterHandler::CheckFlagStateRegister(WORKPLACE_EMPTY))
	{
		// yes, 'workplace empty' flag is set
		// read light sensor to check if external monitor is still on
		if (m_lightSensorObj->GetLightStatus())
		{
			m_lightBelowCnt++;
			m_lightAboveCnt = 0;
			if (m_lightBelowCnt == m_nLightChecks)
			{
				// monitor is still on
				m_lightBelowCnt = 0;	// reset counter
				SetFlagStateRegister(MONITOR_ON);	// updated flag in state register
				SetFlagStateRegister(ALARM_ENABLED);	// requirements for an alarm is fulfilled, set 'alarm enabled' flag in state register
				SetFlagStateRegister(TIMER_FINISHED);	// TEMPORARY, SHOULD NOT BE HERE, JUST TEST. REMOVE
			}
		}
		else
		{
			m_lightAboveCnt++;
			m_lightBelowCnt = 0;
			if (m_lightAboveCnt == m_nLightChecks)
			{
				// monitor is turned off
				m_lightAboveCnt = 0;	// reset counter
				ClearFlagStateRegister(MONITOR_ON);		// clear flag in state register
			}
		}
	}

	Serial.print("stateReg: ");
	Serial.println(m_stateReg);

	Serial.print("m_distanceAboveCnt: ");
	Serial.println(m_distanceAboveCnt);

	Serial.print("m_distanceBelowCnt: ");
	Serial.println(m_distanceBelowCnt);

	Serial.print("m_lightBelowCnt: ");
	Serial.println(m_lightBelowCnt);

	Serial.print("m_lightAboveCnt: ");
	Serial.println(m_lightAboveCnt);
}