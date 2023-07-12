#pragma once

#include "baseentity.h"

#define DT_WAIT_CALLS 26
#define MAX_NEW_COMMANDS 20

struct ResolveMode {
	int m_Pitch = 4; // Default to AUTO
	int m_Yaw = 0;
};

struct VelFixRecord {
	Vec3 m_vecOrigin;
	int m_nFlags;
	float m_flSimulationTime;
};

struct DormantData {
	Vec3 Location;
	float LastUpdate = 0.f;
};

struct Priority {
	int Mode = 2; // 0 - Friend, 1 - Ignore, 2 - Default, 3 - Rage, 4 - Cheater, 5 - Suspicious (ESP)
};

struct GlobalInfo_t
{
	int nextSafeTick = 0;
	int m_nCurrentTargetIdx = 0;
	int m_nCurItemDefIndex = 0;
	int m_nState = 0;
	int m_nDelay = 0;
	int m_nWaitForShift = 0;
	int m_nShifted = 0;
	int m_nTicksToShift = MAX_NEW_COMMANDS; // how many to shift
	int gNotifCounter = 0;
	int vEyeAngDelay = 25;
	int targetHealth = 0;
	int targetHitbox;
	int targetDamage = 0;
	int currentTickLoop = 0;
	int critTick = 0;
	int preventTick = 0;
	int tickShiftQueue = 0;
	int tickCount = 0;
	bool GetTargets = false;
	bool console = false;
	bool m_bWeaponCanHeadShot = false;
	bool m_bWeaponCanAttack = false;
	bool m_bWeaponCanSecondaryAttack = false;
	bool m_bShouldStop = false;
	bool m_bUpdateView = false;
	bool m_bAAActive = false;
	bool m_bScoutJumping = false;
	bool m_bHitscanSilentActive = false;
	bool m_bProjectileSilentActive = false; //flamethrower
	bool m_bAutoBackstabRunning = false;
	bool m_bHitscanRunning = false;
	bool m_bSilentTime = false;
	bool m_bLocalSpectated = false;
	bool m_bRollExploiting = false;
	bool m_bAttacking = false;
	bool m_bModulateWorld = true;
	bool m_bShouldShift = false;
	bool m_bRecharging = false;
	bool m_bRechargeQueued = false;
	bool m_bTeleporting = false;
	bool m_bForceSendPacket = false; // might not actually be useful 
	bool m_bForceChokePacket = false; // might not actually be useful 
	bool m_bChoking = false; // might not actually be useful 
	bool m_bPreventingCrit = false;
	bool m_bFoundCrit = false;
	bool m_bAvoidingBackstab = false;
	bool m_bFakeShotPitch = false;
	bool m_bNullCmd = false;
	bool unloadWndProcHook = false;
	bool calcprojangle = false;
	bool willprojhit = false;
	bool backtracktrace = false;
	bool firsttime = true;
	float m_flCurAimFOV = 0.0f;
	float lerpTime = 0.0f;
	float forwardmove = 0.0f;
	float sidemove = 0.0f;
	Vec3 m_vPredictedPos = {};
	Vec3 m_vAimPos = {};
	Vec3 m_vEyeAngDelayed = {};
	Vec3 m_vViewAngles = {};
	Vec3 m_vRealViewAngles = {};
	Vec3 m_vFakeViewAngles = {};
	Vec3 m_vPunchAngles = {};
	Vec3 linearPredLine = {}; // I HATE NIGGERS
	Vec3 PredictedPos = {};
	VMatrix m_WorldToProjection = {};
	std::vector<Vec3> PredictionLines;
	std::vector<Vec3> PredLinesBackup;
	EWeaponType m_WeaponType = {};
	CUserCmd* lateUserCmd{ nullptr };
	CUserCmd* currentUserCmd{ nullptr };
	std::map<int, bool> ignoredPlayers;
	std::map<int, ResolveMode> resolvePlayers;
	std::map < CTFPlayer*, VelFixRecord> velFixRecord;
	std::pair<bool, bool> AntiAim = {};	//	fakeset, realset
	std::unordered_map<int, DormantData> PartyPlayerESP; // <Index, DormantData>
	std::unordered_map<uint32_t, Priority> PlayerPriority; //<FriendsID, Priority>
	std::unordered_map<int, int> ChokeMap; // Choked packets of players <Index, Amount>
	std::vector<Vec3> predBeforeLines;
	std::vector<Vec3> predFutureLines;

	inline bool IsIgnored( uint32_t friendsID )
	{
		return PlayerPriority[ friendsID ].Mode < 2;
	}

	inline bool IsRage( uint32_t friendsID )
	{
		return PlayerPriority[ friendsID ].Mode > 2;
	}
};

inline GlobalInfo_t g_GlobalInfo;
