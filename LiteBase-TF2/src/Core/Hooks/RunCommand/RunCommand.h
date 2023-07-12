#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/TF2/imovehelper.h"

namespace Hooks
{
	namespace RunCommand
	{
		inline CHook Hook;
		using fn = void( __thiscall * )( CPrediction *, CTFPlayer *, CUserCmd *, IMoveHelper * );
		void __stdcall Func( CTFPlayer *pEntity, CUserCmd *pCmd, IMoveHelper *pMoveHelper );
		void Init( );
	}
}