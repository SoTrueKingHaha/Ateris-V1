#pragma once
#include "../../../SDK/SDK.h"
#include "../../../SDK/Include/TF2/globalinfo.h"
#include "../../Exploits/Backtrack/Backtrack.h"

class CTriggerbot
{
private:
	bool IsAimingAtValidTarget( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, float *pSimTime );
	bool ShouldFire( CTFPlayer *pLocal, CTFWeaponBase *pWeapon );

public:
	void Run( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd );
};

inline CTriggerbot g_Triggerbot;