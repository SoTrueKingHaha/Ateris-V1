#include "AimbotGlobal.h"

#include "../../../Vars.h"

bool CAimbotGlobal::IsKeyDown()
{
	return !Vars::Aimbot::Global::AimKey.var ? true : (GetAsyncKeyState(Vars::Aimbot::Global::AimKey.var ) & 0x8000);
}

void CAimbotGlobal::SortTargets(const ESortMethod &Method)
{
	std::sort(m_vecTargets.begin(), m_vecTargets.end(), [&](const Target_t &a, const Target_t &b) -> bool
	{
		switch (Method)
		{
			case ESortMethod::FOV: return (a.m_flFOVTo < b.m_flFOVTo);
			case ESortMethod::DISTANCE: return (a.m_flDistTo < b.m_flDistTo);
			default: return false;
		}
	});

	std::sort( m_vecTargets.begin( ), m_vecTargets.end( ), [ & ] ( const Target_t &a, const Target_t &b ) -> bool
	{
		return ( a.m_Priority.Mode > b.m_Priority.Mode );
	} );
}

Priority CAimbotGlobal::GetPriority( int targetIdx )
{
	const auto &playerResource = g_EntityCache.GetPR( );
	if ( playerResource && playerResource->GetValid( targetIdx ) )
	{
		const uint32_t priorityID = g_EntityCache.GetPR( )->GetAccountID( targetIdx );
		return g_GlobalInfo.PlayerPriority[ priorityID ];
	}

	return { };
}