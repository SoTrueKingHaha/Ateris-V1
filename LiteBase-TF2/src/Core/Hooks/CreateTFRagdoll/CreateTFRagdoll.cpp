#include "CreateTFRagdoll.h"

#define Offset(type, ent, offset) *reinterpret_cast<type>(reinterpret_cast<DWORD>(ent) + offset)

void ClearEffects( CBaseEntity *pEntity )
{
	Offset( bool *, pEntity, 0xC91 ) = false; // Gib
	Offset( bool *, pEntity, 0xC92 ) = false; // Burning
	Offset( bool *, pEntity, 0xC93 ) = false; // Electrocuted
	Offset( bool *, pEntity, 0xC99 ) = false; // Become ash
	Offset( bool *, pEntity, 0xCA0 ) = false; // Gold
	Offset( bool *, pEntity, 0xCA1 ) = false; // Ice
}

void __fastcall Hooks::CreateTFRagdoll::Func( void *ecx, void *edx )
{
	if ( Vars::Visuals::RagdollEffects::Active.var )
	{
		I::EngineClient->ClientCmd_Unrestricted( "cl_ragdoll_fade_time 15; cl_ragdoll_forcefade 0; cl_ragdoll_physics_enable 1; g_ragdoll_fadespeed 600; g_ragdoll_lvfadespeed 100; ragdoll_sleepaftertime 5.0f" );

		if ( const auto &pEntity = static_cast< CBaseEntity * >( ecx ) )
		{
			if ( Vars::Visuals::RagdollEffects::EnemyOnly.var )
			{
				if ( const auto &pLocal = g_EntityCache.GetLocal( ) )
				{
					if ( Offset( int *, pEntity, 0xCBC ) == pLocal->m_iTeamNum( ) )
					{
						//Team offset
						return;
					}
				}
			}

			ClearEffects( pEntity );

			Offset( bool *, pEntity, 0xC92 ) = Vars::Visuals::RagdollEffects::Burning.var;
			Offset( bool *, pEntity, 0xC93 ) = Vars::Visuals::RagdollEffects::Electrocuted.var;
			Offset( bool *, pEntity, 0xC99 ) = Vars::Visuals::RagdollEffects::BecomeAsh.var;
			Offset( bool *, pEntity, 0xC95 ) = Vars::Visuals::RagdollEffects::Dissolve.var;
			Offset( bool *, pEntity, 0xCA0 ) = Vars::Visuals::RagdollEffects::RagdollType.var == 1;
			Offset( bool *, pEntity, 0xCA1 ) = Vars::Visuals::RagdollEffects::RagdollType.var == 2;
		}
	}
	
	Hook.CallOriginal<fn>( )( ecx, edx );
}

void Hooks::CreateTFRagdoll::Init( )
{
	void* pSrc = reinterpret_cast< void* >( g_Pattern.Find( L"client.dll", L"55 8B EC B8 ? ? ? ? E8 ? ? ? ? 53 56 57 8B F9 8B 8F ? ? ? ? 85 C9 0F 85" ) );
	Hook.Create( pSrc, Func );
}