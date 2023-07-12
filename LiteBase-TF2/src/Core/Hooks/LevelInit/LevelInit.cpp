#include "LevelInit.h"
#include "../../Features/Misc/Misc.h"

void __fastcall Hooks::LevelInit::Func( void *ecx, void *edx )
{
	Hook.CallOriginal<fn>( )( ecx, edx );
}

void Hooks::LevelInit::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::ViewRender, 1 ) );
	Hook.Create( FN, Func );
}