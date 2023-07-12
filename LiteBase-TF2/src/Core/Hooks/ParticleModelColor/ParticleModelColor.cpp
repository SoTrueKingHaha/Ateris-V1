#include "ParticleModelColor.h"
#include "../../Features/Visuals/Colors.h"

void __fastcall Hooks::RenderSpriteCard::Func( void *ecx, void *edx, void *pMeshBuilder, void *pCtx, SpriteRenderInfo_t &info, int hParticle, void *pSortList, void *pCamera )
{
	const auto &pLocal = g_EntityCache.GetLocal( );

	if ( !ecx || !edx || !pMeshBuilder || !pCtx || !pSortList || !pCamera || !pLocal )
	{
		return;
	}

	Color_t teamcolor;
	if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
	{
		if ( pLocal->m_iTeamNum( ) == TEAM_RED )
			teamcolor = Vars::Colors::Red;
		else if ( pLocal->m_iTeamNum( ) == TEAM_BLU )
			teamcolor = Vars::Colors::Blu;
		else
			teamcolor = Vars::Colors::ParticleModulation;
	}
	else
	{
		teamcolor = Vars::Colors::ParticleModulation;
	}

	Color_t particle = Vars::Visuals::ParticlesTeamColor.var ? teamcolor : Vars::Colors::ParticleModulation;
	info.m_pRGB[ ( ( hParticle / 4 ) * info.m_nRGBStride ) + 0 ].m128_f32[ hParticle & 0x3 ] = Utils::CLR2FLT( particle.r );
	info.m_pRGB[ ( ( hParticle / 4 ) * info.m_nRGBStride ) + 1 ].m128_f32[ hParticle & 0x3 ] = Utils::CLR2FLT( particle.g );
	info.m_pRGB[ ( ( hParticle / 4 ) * info.m_nRGBStride ) + 2 ].m128_f32[ hParticle & 0x3 ] = Utils::CLR2FLT( particle.b );

	Hook.CallOriginal<fn>( )( ecx, edx, pMeshBuilder, pCtx, info, hParticle, pSortList, pCamera );
}

void Hooks::RenderSpriteCard::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 28 56 8B 75 10 57 8B 7D 14 8B C7 99 83 E7 03 83 E2 03" ) );
	Hook.Create( pSrc, Func );
}