#include "SetColorModulation.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

void __fastcall Hooks::RenderViewSetColorModulation::Func( void *ecx, void *edx, float const *blend )
{
	if ( !ecx || !edx || !blend )
	{
		return;
	}

	if ( !Outlines.bColoringViewmodel )
	{
		Hook.CallOriginal<fn>( )( ecx, edx, blend );
	}
}

void __fastcall Hooks::RenderViewSetBlend::Func( void *ecx, void *edx, float flAlpha )
{
	if ( !ecx || !edx )
	{
		return;
	}

	if ( !Outlines.bColoringViewmodel )
	{
		Hook.CallOriginal<fn>( )( ecx, edx, flAlpha );
	}
}

void Hooks::RenderViewSetColorModulation::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::RenderView, 6 ) );
	Hook.Create( FN, Func );
}

void Hooks::RenderViewSetBlend::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::RenderView, 4 ) );
	Hook.Create( FN, Func );
}