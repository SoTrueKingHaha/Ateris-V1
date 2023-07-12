#include "DrawStaticProps.h"

bool bDrawing = false;

void __fastcall Hooks::DrawStaticProps::Func( void *ecx, void *edx, IClientRenderable **pProps, int count, bool bShadowDepth, bool drawVCollideWireframe )
{
	bDrawing = true;
	Hook.CallOriginal<fn>( )( ecx, edx, pProps, count, bShadowDepth, drawVCollideWireframe );
	bDrawing = false;
}

void __fastcall Hooks::SetColorModulation::Func( void *ecx, void *edx, float const *pColor )
{
	float custom[ 3 ] = {
		Utils::CLR2FLT( Vars::Colors::StaticPropModulation.r ),
		Utils::CLR2FLT( Vars::Colors::StaticPropModulation.g ),
		Utils::CLR2FLT( Vars::Colors::StaticPropModulation.b )
	};

	bool bShouldUseCustom = Vars::Visuals::WorldModulation.var && bDrawing;
	Hook.CallOriginal<fn>( )( ecx, edx, bShouldUseCustom ? custom : pColor );
}

void __fastcall Hooks::SetAlphaModulation::Func( void *ecx, void *edx, float flAlpha )
{
	Hook.CallOriginal<fn>( )( ecx, edx, bDrawing ? Utils::CLR2FLT( Vars::Colors::StaticPropModulation.a ) : flAlpha );
}

void Hooks::DrawStaticProps::Init( )
{
	fn pSrc = reinterpret_cast< fn >( g_Pattern.Find( ( L"engine.dll" ), ( L"55 8B EC 83 EC 10 8B 15 ? ? ? ? 53 56 57 33 F6 89 4D FC 33 FF 89 75 F0 89 7D F4 8B 42 08" ) ) );
	Hook.Create( reinterpret_cast< void * >( pSrc ), reinterpret_cast< void * >( Func ) );
}

void Hooks::SetColorModulation::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr(I::StudioRender, 27 ) );
	Hook.Create( reinterpret_cast< void * >( FN ), reinterpret_cast< void * >( Func ) );
}

void Hooks::SetAlphaModulation::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::StudioRender, 28 ) );
	Hook.Create( reinterpret_cast< void * >( FN ), reinterpret_cast< void * >( Func ) );
}