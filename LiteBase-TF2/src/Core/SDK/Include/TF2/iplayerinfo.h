#pragma once
#include "cdll_int.h"
#include "globalvars_base.h"

struct edict_t;

class CBotCmd
{
public:
	int	command_number;
	int	tick_count;
	QAngle viewangles;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	byte impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
};

class IPlayerInfo
{
public:
	virtual const char *GetName() = 0;
	virtual int GetUserID() = 0;
	virtual const char *GetNetworkIDString() = 0;
	virtual int GetTeamIndex() = 0;
	virtual void ChangeTeam(int iTeamNum) = 0;
	virtual int	GetFragCount() = 0;
	virtual int	GetDeathCount() = 0;
	virtual bool IsConnected() = 0;
	virtual int	GetArmorValue() = 0;
	virtual bool IsHLTV() = 0;
	virtual bool IsPlayer() = 0;
	virtual bool IsFakeClient() = 0;
	virtual bool IsDead() = 0;
	virtual bool IsInAVehicle() = 0;
	virtual bool IsObserver() = 0;
	virtual const Vector GetAbsOrigin() = 0;
	virtual const QAngle GetAbsAngles() = 0;
	virtual const Vector GetPlayerMins() = 0;
	virtual const Vector GetPlayerMaxs() = 0;
	virtual const char *GetWeaponName() = 0;
	virtual const char *GetModelName() = 0;
	virtual const int GetHealth() = 0;
	virtual const int GetMaxHealth() = 0;
	virtual CBotCmd GetLastUserCommand() = 0;
	virtual bool IsReplay() = 0;
};

class IPlayerInfoManager
{
public:
	virtual IPlayerInfo *GetPlayerInfo(edict_t *pEdict) = 0;
	virtual CGlobalVarsBase *GetGlobalVars() = 0;
};

namespace I { inline IPlayerInfoManager *PlayerInfoManager; }