#include "Materials (Init Delete).h"
#include "../../Features/Visuals/Models/Models.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

void __fastcall Hooks::Connect::Func( void *ecx, void *edx, const char *szName, int nUserID, INetChannel *pNetChannel, bool bFakePlayer, int clientChallenge )
{
	Outlines.Init( );
	Models.Init( );
	return Hook.CallOriginal<fn>( )( ecx, edx, szName, nUserID, pNetChannel, bFakePlayer, clientChallenge );
}

void __fastcall Hooks::Disconnect::Func( void *ecx, void *edx, const char *fmt )
{
	Models.DeleteMaterials( );
	return Hook.CallOriginal<fn>( )( ecx, edx, fmt );
}

void Hooks::Connect::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 53 56 57 68 ? ? ? ? 8B F1" ) );
	Hook.Create( pSrc, Func );
}

void Hooks::Disconnect::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 81 EC ? ? ? ? 57 8B 7D 08 83 BF ? ? ? ? ? 0F 84 ? ? ? ? 56 8D 77 FC 56 E8 ? ? ? ? 8B C8 E8 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? C7 86 ? ? ? ? ? ? ? ? 8B" ) );
	Hook.Create( pSrc, Func );
}