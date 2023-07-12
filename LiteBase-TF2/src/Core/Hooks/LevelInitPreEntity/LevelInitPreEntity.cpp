#include "LevelInitPreEntity.h"
#include "../../Features/Visuals/Outlines/Outlines.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"
#include "../../Features/Visuals/Render/GUI/Menu/PlayerList/PlayerList.h"
#include "../../Features/Events/BadActors/BadActors.h"

void __fastcall Hooks::LevelInitPreEntity::Func(void *ecx, void *edx, char const *pMapName)
{
	g_Backtrack.Restart( );
	g_BadActors.OnLoad( );

	float rate = 1.f / I::GlobalVars->interval_per_tick;

	g_ConVars.cl_updaterate->SetValue( rate );
	g_ConVars.cl_cmdrate->SetValue( rate );

	/*for ( const auto &[Team, Player] : g_PlayerList.PlayerCache )
	{
		if ( Player.m_bFriend )
		{
			g_GlobalInfo.PlayerPriority[ Player.m_iFriendID ].Mode = 0;
		}
	}*/

	Hook.CallOriginal<fn>()(ecx, edx, pMapName);
}

void Hooks::LevelInitPreEntity::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::BaseClientDLL, 5));
	Hook.Create(FN, Func);
}