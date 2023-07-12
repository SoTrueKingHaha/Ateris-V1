#include "DoPostScreenSpaceEffects.h"
#include "../../Features/Misc/Misc.h"

bool __stdcall Hooks::DoPostScreenSpaceEffects::Hook(const CViewSetup* pSetup)
{
	if ( !Vars::Models::Main::ModelOrder.var )
	{
		Models.Render( );
		Outlines.Render( );
	}
	else
	{
		Outlines.Render( );
		Models.Render( );
	}
	g_Misc.DrawPrediction( );

	return Func.CallOriginal<fn>()(I::ClientModeShared, pSetup);
}

void Hooks::DoPostScreenSpaceEffects::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::ClientModeShared, 39));
	Func.Create(FN, Hook);
}