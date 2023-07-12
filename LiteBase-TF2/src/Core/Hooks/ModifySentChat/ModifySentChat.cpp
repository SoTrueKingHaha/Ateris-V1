#include "ModifySentChat.h"

struct s_CTFGameRules
{
	char pad[ 1031 ];
	bool m_bPlayingMedieval;
};

void  __fastcall Hooks::ModifySentChat::Func( void *ecx, void *edx, char *pBuf, int iBufSize )
{
	if ( !ecx || !edx || !pBuf )
	{
		return;
	}

	if ( const auto pGameRules = static_cast< s_CTFGameRules * >( ecx ) )
	{
		ConVar *tf_medieval_autorp = g_ConVars.FindVar( "tf_medieval_autorp" );
		ConVar *english = g_ConVars.FindVar( "english" );
		if ( tf_medieval_autorp && english )
		{
			const bool bOriginalAutoRP = tf_medieval_autorp->GetBool( );
			const bool bOriginalEnglish = english->GetBool( );
			const bool bOriginalPlayingMedieval = pGameRules->m_bPlayingMedieval;

			pGameRules->m_bPlayingMedieval = false;
			tf_medieval_autorp->SetValue( 0 );
			english->SetValue( 0 );

			Hook.CallOriginal<fn>( )( ecx, edx, pBuf, iBufSize );
			pGameRules->m_bPlayingMedieval = bOriginalPlayingMedieval;
			tf_medieval_autorp->SetValue( bOriginalAutoRP );
			english->SetValue( bOriginalEnglish );
			return;
		}
	}
}

void Hooks::ModifySentChat::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 80 B9 ? ? ? ? ? 56 8B 75 08 74 26" ) );
	Hook.Create( pSrc, Func );
}