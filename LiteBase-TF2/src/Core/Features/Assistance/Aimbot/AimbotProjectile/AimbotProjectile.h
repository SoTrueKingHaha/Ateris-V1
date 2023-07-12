#pragma once
#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotProjectile
{
private:
	struct ProjectileInfo_t
	{
		float m_flVelocity = 0.0f;
		float m_flGravity = 0.0f;
		float m_flMaxTime = 2.0f;
	};

	struct Predictor_t
	{
		CBaseEntity *m_pEntity	= nullptr;
		Vec3 m_vPosition		= {};
		Vec3 m_vVelocity		= {};
		Vec3 m_vAcceleration	= {};

		Vec3 Extrapolate(float time);
	};

	struct Solution_t
	{
		float m_flPitch = 0.0f;
		float m_flYaw	= 0.0f;
		float m_flTime	= 0.0f;
	};

	bool GetProjectileInfo(CTFWeaponBase *pWeapon, ProjectileInfo_t &out);
	bool CalcProjAngle(const Vec3 &vLocalPos, const Vec3 &vTargetPos, const ProjectileInfo_t &ProjInfo, Solution_t &out);
	bool SolveProjectile(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, Predictor_t &Predictor, const ProjectileInfo_t &ProjInfo, Solution_t &out);

private:
	Vec3 GetAimPos(CTFPlayer *pLocal, CBaseEntity *pEntity, const Vec3 &targetPredPos );
	Vec3 GetAimPosBuilding( CTFPlayer *pLocal, CBaseEntity *pEntity );
	bool WillProjectileHit( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, const Vec3 &vPredictedPos, Solution_t &out, const ProjectileInfo_t &projInfo, const Predictor_t &predictor );
	ESortMethod GetSortMethod();
	bool GetTargets(CTFPlayer *pLocal, CTFWeaponBase *pWeapon);
	bool VerifyTarget(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, Target_t &Target);
	bool GetTarget(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, Target_t &Out);
	void Aim(CUserCmd *pCmd, CTFWeaponBase *pWeapon, Vec3 &vAngle);
	bool ShouldFire(CUserCmd *pCmd, Target_t target);
	bool IsAttacking(const CUserCmd *pCmd, CTFWeaponBase *pWeapon);

	bool m_bIsFlameThrower;
	bool m_bIsBoosted;

public:
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
};

inline CAimbotProjectile g_AimbotProjectile;