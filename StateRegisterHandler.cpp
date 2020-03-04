#include "StateRegisterHandler.h"

StateRegisterHandler::StateRegisterHandler(DistanceSensor* distanceSensorObj1, const byte nDistanceChecks, DistanceSensor* distanceSensorObj2, const byte nHeightChecks, LightSensor* lightSensorObj, const byte nLightChecks)
	: m_distanceSensorObj1(distanceSensorObj1), m_nDistanceChecks(nDistanceChecks), m_distanceSensorObj2(distanceSensorObj2), m_nHeightChecks(nHeightChecks), m_lightSensorObj(lightSensorObj), m_nLightChecks(nLightChecks)
{
	m_distanceAboveCnt = 0;
	m_distanceBelowCnt = 0;
	m_heightAboveCnt = 0;
	m_heightBelowCnt = 0;
	m_lightBelowCnt = 0;
	m_lightAboveCnt = 0;
	m_stateRegister = WORKPLACE_CHECK_ENABLED;	// enable check of workplace to determine if it is empty
	m_stateRegisterExtended = RESET_EXTENDED;
	m_forgotLockCnt = 0;
	m_deskElevatedCnt = 0;
	m_elapsedTime = 0;
	m_toggle1 = false;
	m_toggle2 = false;
}

void StateRegisterHandler::IncreaseDeskElevatedCounter()
{
	if (CheckFlagStateRegisterExtended(WORK_DESK_ELEVATED) && m_toggle2 == false)
	{
		m_deskElevatedCnt++;
		m_toggle2 = true;
	}
	else if (!(CheckFlagStateRegisterExtended(WORK_DESK_ELEVATED) && m_toggle2 == true))
	{
		m_toggle2 = false;
	}
}

void StateRegisterHandler::SetFlagStateRegister(States flag)
{
	m_stateRegister |= flag;
}

void StateRegisterHandler::ClearFlagStateRegister(States flag)
{
	m_stateRegister &= (MASK ^ flag);
}

bool StateRegisterHandler::CheckFlagStateRegister(States flag)
{
	return m_stateRegister & flag;
}

byte& StateRegisterHandler::GetStateRegister()
{
	return m_stateRegister;
}

void StateRegisterHandler::SetFlagStateRegisterExtended(StatesExtended flag)
{
	m_stateRegisterExtended |= flag;
}

void StateRegisterHandler::ClearFlagStateRegisterExtended(StatesExtended flag)
{
	m_stateRegisterExtended &= (MASK ^ flag);
}

bool StateRegisterHandler::CheckFlagStateRegisterExtended(StatesExtended flag)
{
	return m_stateRegisterExtended & flag;
}

byte& StateRegisterHandler::GetStateRegisterExtended()
{
	return m_stateRegisterExtended;
}

void StateRegisterHandler::CheckWorkplace()
{
	// check if workplace is empty and update stateRegister accordingly
	if (m_distanceSensorObj1->GetDistanceStatus())
	{
		if (CheckFlagStateRegister(WORKPLACE_CHECK_ENABLED))
		{
			m_distanceAboveCnt++;
			m_distanceBelowCnt = 0;

			if (m_distanceAboveCnt == m_nDistanceChecks)
			{
				m_distanceAboveCnt = 0;		// reset counter
				SetFlagStateRegister(WORKPLACE_EMPTY);	// set flag bit to indicate 'workplace is empty'
			}
		}
	}
	else
	{
		m_distanceBelowCnt++;
		m_distanceAboveCnt = 0;

		if (m_distanceBelowCnt == m_nDistanceChecks)
		{
			m_distanceBelowCnt = 0;		// reset counter
			ClearFlagStateRegister(WORKPLACE_EMPTY);
			ClearFlagStateRegister(MONITOR_ON);
			ClearFlagStateRegister(ALARM_ENABLED);	// clear 'alarm enabled' flag in state register
			SetFlagStateRegister(WORKPLACE_CHECK_ENABLED);	// set 'workplace check' flag to enable workplace check when the workplace has been confirmed to be reoccupied after an alarm has been trigged
			m_lightBelowCnt = 0;	// clear counter
		}
	}

	// check work desk height to determine if desk is elevated or not
	if (!(m_distanceSensorObj2->GetDistanceStatus()))
	{
		// work desk is elevated, user is assumed to be standing
		m_heightBelowCnt++;
		m_heightAboveCnt = 0;
		if (m_heightBelowCnt == m_nHeightChecks)
		{
			m_heightBelowCnt = 0;
			SetFlagStateRegisterExtended(WORK_DESK_ELEVATED);
		}
	}
	else
	{
		// work desk is not elevated, user is assumed to be sitting
		m_heightAboveCnt++;
		m_heightBelowCnt = 0;
		if(m_heightAboveCnt == m_nHeightChecks)
		{
			m_heightAboveCnt = 0;
			ClearFlagStateRegisterExtended(WORK_DESK_ELEVATED);
		}
	}
	IncreaseDeskElevatedCounter();	// run increase counter function which in itself has a logics built in to only increase counter once for each occation when desk is raised

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
				SetFlagStateRegister(ALARM_ENABLED);	// requirements for an alarm trigg is fulfilled. set 'alarm enabled' flag in state register to enable start of countdown timer
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
				ClearFlagStateRegister(WORKPLACE_EMPTY);
				ClearFlagStateRegister(ALARM_ENABLED);	// clear 'alarm enabled' flag in state register
			}
		}
	}

	Serial.print("stateReg: ");
	Serial.println(m_stateRegister);

	Serial.print("m_distanceAboveCnt: ");
	Serial.println(m_distanceAboveCnt);

	Serial.print("m_distanceBelowCnt: ");
	Serial.println(m_distanceBelowCnt);

	Serial.print("lightValue: ");
	Serial.println(m_lightSensorObj->GetLightValue());

	Serial.print("m_lightBelowCnt: ");
	Serial.println(m_lightBelowCnt);

	Serial.print("m_lightAboveCnt: ");
	Serial.println(m_lightAboveCnt);

	Serial.print("m_heightAboveCnt: ");
	Serial.println(m_heightAboveCnt);

	Serial.print("m_heightBelowCnt: ");
	Serial.println(m_heightBelowCnt);

	Serial.print("m_deskElevatedCnt: ");
	Serial.println(m_deskElevatedCnt);
}

byte& StateRegisterHandler::GetForgotLockCnt()
{
	return m_forgotLockCnt;
}

long& StateRegisterHandler::GetElapsedTime()
{
	m_elapsedTime = millis();	// read internal timer, counting up from zero when arduino was turned on
	m_elapsedTime = (m_elapsedTime / 1000) / 60;	// convert time to minutes
	return m_elapsedTime;	// elapsed time in minutes
}

void StateRegisterHandler::IncreaseForgotLockCounter()
{
	if (CheckFlagStateRegister(TIMER_FINISHED) && m_toggle1 == false)
	{
		m_forgotLockCnt++;	// increase counter by 1
		m_toggle1 = true;	// set toggle variable to true to only count the occation when 'timer finished' flag in state register changes to high
	}
	else if (!(CheckFlagStateRegister(TIMER_FINISHED) && m_toggle1 == true))
	{
		m_toggle1 = false;
	}
}  

byte& StateRegisterHandler::GetDeskElevatedCnt()
{
	return m_deskElevatedCnt;
}