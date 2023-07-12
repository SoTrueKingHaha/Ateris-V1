#include "Paint.h"
#include "../../Features/Visuals/Render/ESP/ESP.h"
#include "../../Features/Visuals/Render/GUI/Menu/GUI.h"
#include "../../Features/Visuals/Render/GUI/Menu/InputHelper/InputHelper.h"
#include "../../Features/Exploits/CritHack/CritHack.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"
#include "../../Features/Visuals/Render/GUI/Indicators/Indicators.h"
#include "../../Utils/Timer/Timer.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Visuals/Render/GUI/Menu/PlayerList/PlayerList.h"

void __fastcall Hooks::Paint::Func( void *ecx, void *edx, int mode )
{
	if ( !g_Draw.m_nScreenW || !g_Draw.m_nScreenH )
		g_Draw.UpdateScreenSize( );

	Hook.CallOriginal<fn>( )( ecx, edx, mode );

	if ( mode & PAINT_UIPANELS )
	{
		g_Draw.UpdateW2SMatrix( );

		I::Surface->StartDrawing( );
		{
			if ( const auto &pLocal = g_EntityCache.GetLocal( ) )
			{
				ESP.Render( );
				g_Indicators.Run( );
				//g_Crits.Render( );
			}
			g_Menu.Render( );
			//g_Menu.PlayerList( );
		}
		I::Surface->FinishDrawing( );
	}
}

void Hooks::Paint::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::EngineVGui, 14));
	Hook.Create(FN, Func);
}