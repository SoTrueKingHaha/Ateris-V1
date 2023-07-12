#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace KeyEvent
	{
		inline CHook Hook;
		using fn = int(__fastcall*)(IBaseClientDLL*, PVOID, int, int, const char*);
		int __fastcall Func(IBaseClientDLL* ecx, PVOID edx, int event_code, int key_num, const char* current_binding);
		void Init();
	}
}
