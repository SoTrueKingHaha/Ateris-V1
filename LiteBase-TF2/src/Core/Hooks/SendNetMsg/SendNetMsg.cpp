#include "SendNetMsg.h"

bool __fastcall Hooks::SendNetMsg::Func( CNetChannel *CNetChan, void *edi, INetMessage &msg, bool bForceReliable, bool bVoice )
{
	switch ( msg.GetType( ) ) {
		case clc_VoiceData: {		// stop lag with voice chat
			bVoice = true;
			break;
		}
		case clc_FileCRCCheck: {	// whitelist
			if ( Vars::Misc::PureBypass.var ) {
				return false;
			}
			break;
		}
		case clc_Move:
		{
			const int iAllowedNewCommands = 24 - ( g_GlobalInfo.m_nTicksToShift - g_GlobalInfo.m_nShifted );
			const auto &moveMsg = reinterpret_cast< CLC_Move & >( msg );
			if ( moveMsg.m_nNewCommands > iAllowedNewCommands )
			{
				g_GlobalInfo.m_nShifted += moveMsg.m_nNewCommands - iAllowedNewCommands;
			}
			break;
		}
	}

	return Hook.CallOriginal<fn>( )( CNetChan, edi, msg, bForceReliable, bVoice );
}

void Hooks::SendNetMsg::Init( )
{
	void *pSrc = reinterpret_cast< void* >( g_Pattern.Find( L"engine.dll", L"55 8B EC 57 8B F9 8D 8F ? ? ? ? E8 ? ? ? ? 85 C0 75 07 B0 01 5F 5D C2 0C 00" ) );
	Hook.Create( pSrc, Func );
}