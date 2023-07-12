#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/EntityCache/EntityCache.h"

namespace Hooks
{
	namespace FrameStageNotify
	{
		inline CHook Hook;
		using fn = void(__fastcall *)( void *ecx, void *edx, ClientFrameStage_t);
		void __fastcall Func( void *ecx, void *edx, ClientFrameStage_t curStage);
		void Init();
		inline int outgoing_command, current_command;
		inline CUserCmd *cmd;
	}
}