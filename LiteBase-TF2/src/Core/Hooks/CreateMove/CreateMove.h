#pragma once
#include "../../Features/Misc/Misc.h"
#include "../../Features/Exploits/Engine Prediction/Prediction.h"

namespace Hooks
{
	namespace CreateMove
	{
		inline CHook Hook;
		using fn = bool(__fastcall *)( void *, void *, float, CUserCmd *);
		bool __fastcall Func(void *ecx, void *edx, float flInputSampleTime, CUserCmd *pCmd);
		void Init();
	}
}