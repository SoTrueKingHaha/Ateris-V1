#include "MaterialUncache.h"
#include "../../Features/Visuals/Models/Models.h"

void __fastcall Hooks::MaterialUncache::Func( IMaterial *ecx, void *edx, bool bPreserveVars )
{
	if ( ecx )
	{
		const std::string materialName = ecx->GetName( );
		if ( materialName.find( "m_pmat" ) != std::string::npos || materialName.find( "glow_color" ) != std::string::npos ) {
			return;
		}
	}

	if ( ecx )
	{
		Hook.CallOriginal<fn>( )( ecx, edx, bPreserveVars );
	}
}

void Hooks::MaterialUncache::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.E8( L"MaterialSystem.dll", L"E8 ? ? ? ? 83 7E 1C 00" ) );
	Hook.Create( pSrc, Func );
}