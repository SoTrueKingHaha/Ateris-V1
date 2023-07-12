#include "ViewmodelPosition.h"
#include "../../SDK/Include/TF2/globalinfo.h"

void __fastcall Hooks::ViewOffset::Func( void *ecx, void *edx, CTFPlayer *ownerEntity, Vec3 &eyePosition, Vec3 &eyeAngles )
{
	if ( Vars::Visuals::ViewmodelPos.var )
	{
		Vec3 AxisX{}, AxisY{}, AxisZ{};

		Utils::AngleVectors( eyeAngles, &AxisY, &AxisX, &AxisZ );

		Vec3 eyepos = eyePosition + ( AxisX * Vars::Visuals::ViewX.var ) + ( AxisY * Vars::Visuals::ViewY.var ) + ( AxisZ * Vars::Visuals::ViewZ.var );
		eyeAngles.z += Vars::Visuals::ViewRoll.var;

		Hook.CallOriginal<fn>( )( ecx, edx, ownerEntity, eyepos, eyeAngles );
	}
	else
	{
		Hook.CallOriginal<fn>( )( ecx, edx, ownerEntity, eyePosition, eyeAngles );
	}
}

void Hooks::ViewOffset::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 70 8B 55 0C 53 8B 5D 08 89 4D FC 8B 02 89 45 E8 8B 42 04 89 45 EC 8B 42 08 89 45 F0 56 57" ) );
	Hook.Create( pSrc, Func );
}