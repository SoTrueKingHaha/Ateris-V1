#include "CheckForSequenceChange.h"

void __fastcall Hooks::CheckForSequenceChange::Func( void *ecx, void *edx, CStudioHdr *hdr, int nCurSequence, bool bForceNewSequence, bool bInterpolate )
{
	return Hook.CallOriginal<fn>( )( ecx, edx, hdr, nCurSequence, bForceNewSequence, Vars::Misc::AnimFix.var ? false : bInterpolate );
}

void Hooks::CheckForSequenceChange::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 53 8B 5D 08 57 8B F9 85 DB 0F" ) );
	Hook.Create( pSrc, Func );
}

void __fastcall Hooks::MaintainSequenceTransitions::Func( void *ecx, void *edx, void *boneSetup, float flCycle, Vec3 pos[ ], Vector4D q[ ] )
{
	return;
}

void Hooks::MaintainSequenceTransitions::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC ? 56 8B 75 ? 57 8B F9 8B CE E8 ? ? ? ? 85 C0" ) );
	Hook.Create( pSrc, Func );
}