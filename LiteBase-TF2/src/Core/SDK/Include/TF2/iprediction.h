#pragma once
#include "iclientmode.h"

class IMoveHelper;
using EntityHandle_t = CBaseHandle;

class CMoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	EntityHandle_t m_nPlayerHandle;
	int m_nImpulseCommand;
	Vec3 m_vecViewAngles;
	Vec3 m_vecAbsViewAngles;
	int m_nButtons;
	int m_nOldButtons;
	float m_flForwardMove;
	float m_flOldForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vec3 m_vecVelocity;
	Vec3 m_vecAngles;
	Vec3 m_vecOldAngles;
	float m_outStepHeight;
	Vec3 m_outWishVel;
	Vec3 m_outJumpVel;
	Vec3 m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	Vec3 m_vecAbsOrigin;
};

class IPrediction
{
public:
	virtual ~IPrediction(void) {};
	virtual void Init(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command) = 0;
	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void PostEntityPacketReceived(void) = 0;
	virtual void PostNetworkDataReceived(int commands_acknowledged) = 0;
	virtual void OnReceivedUncompressedPacket(void) = 0;
	virtual void GetViewOrigin(Vector &org) = 0;
	virtual void SetViewOrigin(Vector &org) = 0;
	virtual void GetViewAngles(QAngle &ang) = 0;
	virtual void SetViewAngles(QAngle &ang) = 0;
	virtual void GetLocalViewAngles(QAngle &ang) = 0;
	virtual void SetLocalViewAngles(QAngle &ang) = 0;
};

class CPrediction : public IPrediction
{
public:
	virtual ~CPrediction();
	virtual void Init();
	virtual void Shutdown();
	virtual void Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command);
	virtual void OnReceivedUncompressedPacket();
	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet);
	virtual void PostEntityPacketReceived();
	virtual void PostNetworkDataReceived(int commands_acknowledged);
	virtual bool InPrediction();
	virtual bool IsFirstTimePredicted();
	virtual int	GetIncomingPacketNumber();
	virtual void GetViewOrigin(Vec3 &org);
	virtual void SetViewOrigin(Vec3 &org);
	virtual void GetViewAngles(QAngle &ang);
	virtual void SetViewAngles(QAngle &ang);
	virtual void GetLocalViewAngles(QAngle &ang);
	virtual void SetLocalViewAngles(QAngle &ang);
	virtual void RunCommand(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper);
	virtual void SetupMove(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move);
	virtual void FinishMove(CBaseEntity *player, CUserCmd *ucmd, CMoveData *move);
	virtual void SetIdealPitch(CBaseEntity *player, const Vec3 &origin, const Vec3 &angles, const Vec3 &viewheight);
	virtual void _Update(bool received_new_world_update, bool validframe, int incoming_acknowledged, int outgoing_command);

	//CHandle< C_BaseEntity > m_hLastGround;
	uint32_t m_hLastGround;
	bool m_bInPrediction;
	bool m_bFirstTimePredicted;
	bool m_bOldCLPredictValue;
	bool m_bEnginePaused;
	int m_nPreviousStartFrame;
	int m_nCommandsPredicted;
	int m_nServerCommandsAcknowledged;
	int m_bPreviousAckHadErrors;
	int m_nIncomingPacketNumber;
	float m_flIdealPitch;
};

namespace I { inline CPrediction *Prediction; }