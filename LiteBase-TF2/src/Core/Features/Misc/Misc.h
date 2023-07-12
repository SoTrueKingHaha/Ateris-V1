#pragma once
#include "../../SDK/SDK.h"

class CMisc {
	int m_nHudZoom = 0;
	void AccurateMovement(CUserCmd* pCmd, CTFPlayer* pLocal);
	void AutoJump(CUserCmd* pCmd, CTFPlayer* pLocal);
	void AutoStrafe(CUserCmd* pCmd, CTFPlayer* pLocal);
	void AutoPeek( CUserCmd *pCmd, CTFPlayer *pLocal );
	void LegJitter( CUserCmd *pCmd, CTFPlayer *pLocal );
	void AntiBackstab( CUserCmd *pCmd, CTFPlayer *pLocal );
	//void AntiBackstab(CTFPlayer* pLocal, CUserCmd* pCmd);

	void CheatsBypass();

	void Viewmodel();
	//void Teleport(const CUserCmd* pCmd);
	void ServerHitbox( CTFPlayer* pLocal);
	void WeaponSway();
	void FixOutlines( );
	void Console( );
	void DetectChoke();

	void FastStop(CUserCmd* pCmd, CTFPlayer* pLocal);
	//void StopMovement(CUserCmd* pCmd, bool* pSendPacket);
	// Late
	//void FastStop(CUserCmd* pCmd, CTFPlayer* pLocal);
	//void AutoRocketJump(CUserCmd* pCmd, CTFPlayer* pLocal);
	void AutoScoutJump(CUserCmd* pCmd, CTFPlayer* pLocal);
	float m_flSpinYaw = 0.f;

	bool SteamCleared = false;
	bool bAntiWarp = false;
	bool bMovementStopped = false;
public:
	bool TauntControl(CUserCmd* pCmd);
	void ManualNetwork( StartSoundParams_t &params );
	bool RemoveScope( int nPanel );
	void Run(CUserCmd* pCmd);
	void RunLate(CUserCmd* pCmd);
	void UnlockAchievements( );
	void LockAchievements( );
	void SteamRPC( );
	void DrawPrediction( );
	void DrawHitboxes( CTFPlayer *pPlayer, Color_t colorface, Color_t coloredge, float time );

	void ThirdPerson(CViewSetup* pView);
	void FOV(CViewSetup* pView);
	inline static int ViewFOV;
	inline static float AspectRatio;

	void ModulateWorld( );
	void UpdateWorldModulation( );
	void RestoreWorldModulation( );

	//void UnlockAchievements();
	//void LockAchievements();

	Vec3 PeekReturnPos;
};

inline CMisc g_Misc;

constexpr auto MAX_NOTIFY_SIZE = 8;

class NotifyText {
public:
	std::string Text;
	Color_t Color;
	float Time;

	NotifyText() { }
	NotifyText(std::string text, Color_t color, float time) : Text{ std::move(text) }, Color{ color }, Time{ time } { }
};

class CNotifications {
	std::vector<std::shared_ptr<NotifyText>> NotificationTexts;

public:
	CNotifications() = default;

	__forceinline void Add(const std::string& text, Color_t color = { 255, 255, 255, 255 },
		float time = 5.0f)
	{
		NotificationTexts.push_back(std::make_shared<NotifyText>(text, color, time));
	}

	void Think();
};

inline NotifyText g_Notify;