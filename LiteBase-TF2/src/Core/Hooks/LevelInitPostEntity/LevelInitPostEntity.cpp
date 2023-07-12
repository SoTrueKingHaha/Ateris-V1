#include "LevelInitPostEntity.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

void __fastcall Hooks::LevelInitPostEntity::Func( void *ecx, void *edx )
{
	Hook.CallOriginal<fn>()( ecx, edx );
	//I::EngineClient->ClientCmd_Unrestricted( "r_maxdlights 4096" );
	I::EngineClient->ClientCmd_Unrestricted( "r_dynamic 1" );
	if ( I::EngineClient->IsConnected( ) || I::EngineClient->IsInGame( ) )
		g_Misc.ModulateWorld( );
}

void Hooks::LevelInitPostEntity::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::BaseClientDLL, 6));
	Hook.Create(FN, Func);
}