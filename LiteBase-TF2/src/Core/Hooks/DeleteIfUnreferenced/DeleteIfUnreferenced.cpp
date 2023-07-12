#include "DeleteIfUnreferenced.h"
#include "../../Features/Visuals/Models/Models.h"

void __fastcall Hooks::DeleteIfUnreferenced::Func( IMaterial *ecx, void* edx )
{
	if ( ecx ) {
		/*const std::string materialName = ecx->GetName( );
		if ( materialName.find( "m_pmat" ) != std::string::npos || materialName.find( "glow_color" ) != std::string::npos )
		{
			return;
		}*/
		const std::string materialName = ecx->GetName( );
		if ( materialName.find( "m_pmat" ) != std::string::npos || materialName.find( "glow_color" ) != std::string::npos )
		{
			return;
		}
	}

	if ( ecx )
	{
		return Hook.CallOriginal<fn>( )( ecx, edx );
	}
}

void Hooks::DeleteIfUnreferenced::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"materialsystem.dll", L"56 8B F1 83 7E 1C 00 7F 51" ) );
	Hook.Create( pSrc, Func );
}