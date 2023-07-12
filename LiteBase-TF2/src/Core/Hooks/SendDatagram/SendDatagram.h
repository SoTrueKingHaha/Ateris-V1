#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"

namespace Hooks
{
	namespace SendDatagram
	{
		inline CHook Hook;
		using fn = int( __fastcall * )( INetChannel *, void *, bf_write * );
		int __fastcall Func( INetChannel *netChannel, void *edx, bf_write *datagram );
		void Init( );
	}
}