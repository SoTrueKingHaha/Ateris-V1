#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace SendNetMsg
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( CNetChannel *, void *, INetMessage &, bool, bool );
		bool __fastcall Func( CNetChannel *CNetChan, void *edi, INetMessage &msg, bool bForceReliable, bool bVoice );
		void Init( );
	}
}