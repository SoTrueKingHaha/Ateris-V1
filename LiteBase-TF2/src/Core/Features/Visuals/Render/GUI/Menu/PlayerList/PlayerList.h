#pragma once
#include "../../../../../../SDK/SDK.h"
#include "../GUI.h"
#include <mutex>

struct Player_t
{
	//Color_t color{ };
	const char* m_szName{ };
	bool m_bAlive{ };
	//bool m_bFriend{ };
	int m_iTeam{ };
	int m_iIndex{ };
	int m_iUserID{ };
	uint32_t m_iFriendID{ };
	int m_iScore{ };
	int m_iKills{ };
	int m_iDeaths{ };
	int m_iPing{ };
};

class CPlayerList
{
public:
	void Fill( );
	/*std::vector<Player_t> m_vecAll;
	std::vector<Player_t> m_vecEnemies;
	std::vector<Player_t> m_vecTeammates;
	std::vector<Player_t> m_vecOthers;*/

	std::mutex mutex;
	std::multimap<int, Player_t> PlayerCache{};
};

inline CPlayerList g_PlayerList;