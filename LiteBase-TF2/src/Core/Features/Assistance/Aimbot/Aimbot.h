#pragma once
#include "../../../SDK/SDK.h"

class CAimbot
{
private:
	bool ShouldRun( CTFPlayer *pLocal, CTFWeaponBase *pWeapon );

public:
	void Run(CUserCmd* pCmd);
};

inline CAimbot g_Aimbot;