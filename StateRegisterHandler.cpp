#include "StateRegisterHandler.h"

StateRegisterHandler::StateRegisterHandler(byte nDistanceChecks, byte nLightChecks)
	: m_nDistanceChecks(nDistanceChecks), m_nLightChecks(nLightChecks), m_distanceAboveCnt(0), m_distanceBelowCnt(0), m_lightBelowCnt(0), m_lightAboveCnt(0)
{
    States stateReg = RESET;
}


void StateRegisterHandler::SetFlagStateRegister(States state)
{
    States stateReg = state;
    /*
    RESET = 0,
        WORKPLACE_EMPTY = 1 << 0,   // binary 0000'0001
        MONITOR_ON = 1 << 1,        // binary 0000'0010
        TIMER_ENABLED = 1 << 2,     // binary 0000'0100
        TIMER_FINISHED = 1 << 3,    // binary 0000'1000
        ALARM_DISABLED = 1 << 4,    // binary 0001'0000
        AUDIO_VISUAL_ON = 1 << 5,   // binary 0010'0000
        MASK = B11111111            // binary 1111'1111
    */
}


/*
byte m_distanceAboveCnt;
byte m_distanceBelowCnt;
const byte m_nDistanceChecks;
byte m_lightBelowCnt;
byte m_lightAboveCnt;
const byte m_nLightChecks;
*/

//void CheckFlagStateRegister(byte);	// maybe change to string parameter instead

void ClearFlagStateRegister(byte)
{}

void CheckWorkplace()
{}