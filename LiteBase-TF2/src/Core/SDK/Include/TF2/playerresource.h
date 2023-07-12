#pragma once
#include "baseentity.h"
#define MAX_PLAYERS 33
#define MAX_TEAMS 32

struct PlayerScoreboardStats_t
{
	int	m_iPrevDamage;
	int	m_iPrevDamageAssist;
	int	m_iPrevDamageBoss;
	int	m_iPrevHealing;
	int	m_iPrevHealingAssist;
	int m_iPrevDamageBlocked;
	int	m_iPrevCurrencyCollected;
	int	m_iPrevBonusPoints;
};

class CBaseEntity;
class IGameResources
{
public:
	virtual const char* GetTeamName(int index) = 0;
	virtual int				GetTeamScore(int index) = 0;
	virtual const Color_t& GetTeamColor(int index) = 0;

	// Player data access
	virtual bool	IsConnected(int index) = 0;
	virtual bool	IsAlive(int index) = 0;
	virtual bool	IsFakePlayer(int index) = 0;
	virtual bool	IsLocalPlayer(int index) = 0;

	virtual const char* GetPlayerName(int index) = 0;
	virtual int		GetPlayerScore(int index) = 0;
	virtual int		GetPing(int index) = 0;
	//	virtual int		GetPacketloss( int index ) = 0;
	virtual int		GetDeaths(int index) = 0;
	virtual int		GetFrags(int index) = 0;
	virtual int		GetTeam(int index) = 0;
	virtual int		GetHealth(int index) = 0;
};

class CPlayerResource : public CBaseEntity, public IGameResources
{
public:
	virtual int		GetTeamScore(int index);
	virtual const char* GetTeamName(int index);
	virtual const Color_t& GetTeamColor(int index);

	// Player data access
	virtual bool	IsConnected(int index);
	virtual bool	IsAlive(int index);
	virtual bool	IsFakePlayer(int index);
	virtual bool	IsLocalPlayer(int index);
	virtual bool	IsHLTV(int index);
	virtual bool	IsReplay(int index);

	virtual const char* GetPlayerName(int index);
	virtual int		GetPing(int index);
	//	virtual int		GetPacketloss( int index );
	virtual int		GetPlayerScore(int index);
	virtual int		GetDeaths(int index);
	virtual int		GetTeam(int index);
	virtual int		GetFrags(int index);
	virtual int		GetHealth(int index);

	virtual void ClientThink();
	virtual	void	OnDataChanged(DataUpdateType_t updateType);

	unsigned int GetAccountID(int iIndex);
	bool IsValid(int iIndex);

	void	UpdatePlayerName(int slot);
};

class CTFPlayerResource : public CPlayerResource
{
	int		m_iDamage[MAX_PLAYERS + 1];
	int		m_iDamageAssist[MAX_PLAYERS + 1];
	int		m_iDamageBoss[MAX_PLAYERS + 1];
	int		m_iHealing[MAX_PLAYERS + 1];
	int		m_iHealingAssist[MAX_PLAYERS + 1];
	int		m_iDamageBlocked[MAX_PLAYERS + 1];
	int		m_iCurrencyCollected[MAX_PLAYERS + 1];
	int		m_iBonusPoints[MAX_PLAYERS + 1];
	int		m_iPlayerLevel[MAX_PLAYERS + 1];
	int		m_iUpgradeRefundCredits[MAX_PLAYERS + 1];
	int		m_iBuybackCredits[MAX_PLAYERS + 1];

	int		m_iPartyLeaderBlueTeamIndex;
	int		m_iPartyLeaderRedTeamIndex;
	int		m_iEventTeamStatus;

	int		m_iPlayerClassWhenKilled[MAX_PLAYERS + 1];

	PlayerScoreboardStats_t m_aPlayerScoreStats[MAX_PLAYERS + 1];
};