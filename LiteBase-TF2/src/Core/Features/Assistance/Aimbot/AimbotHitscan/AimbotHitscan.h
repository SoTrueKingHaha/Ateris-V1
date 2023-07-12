#pragma once

#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotHitscan
{
private:
	int GetHitbox(CTFPlayer *pLocal, CTFWeaponBase *pWeapon);
	ESortMethod GetSortMethod();
	bool GetTargets( CTFPlayer *pLocal, CTFWeaponBase *pWeapon);
	bool ScanHitboxes( CTFPlayer *pLocal, Target_t &Target);
	bool ScanHead( CTFPlayer *pLocal, Target_t &Target);
	bool ScanBuildings( CTFPlayer *pLocal, Target_t &Target);
	bool VerifyTarget( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, Target_t &Target);
	bool GetTarget( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, Target_t &Out);
	void Aim(CUserCmd *pCmd, Vec3 &vAngle);
	bool ShouldFire( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, const Target_t &Target);

public:
	bool IsAttacking( CUserCmd *pCmd, CTFWeaponBase *pWeapon );
	void Run( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
};

inline CAimbotHitscan g_AimbotHitscan;