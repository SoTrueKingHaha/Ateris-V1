#pragma once
#include "../../../../SDK/SDK.h"

class CMovementSimulation
{
public:
	CTFPlayer *m_pPlayer = nullptr;
	CMoveData m_MoveData = {};

private:
	//void SetCurrentCommand(CBaseEntity* pPlayer, CUserCmd* pCmd);
	void SetupMoveData( CTFPlayer *pPlayer, CMoveData *pMoveData );

private:
	bool bFirstRunTick = true;
	bool m_bOldInPrediction = false;
	bool m_bOldFirstTimePredicted = false;
	bool bDontPredict = false;
	float m_flOldFrametime = 0.0f;
	int iTick = 0;

	std::map<int, std::deque<Vec3>> m_Velocities;
public:
	bool Initialize( CTFPlayer *pPlayer );
	void Restore( );
	void FillVelocities( );
	bool StrafePrediction( );
	void RunTick( CMoveData &moveDataOut, Vec3 &m_vecAbsOrigin );
	const Vec3 &GetOrigin( ) { return m_MoveData.m_vecAbsOrigin; }
};

inline CMovementSimulation g_MoveSim;