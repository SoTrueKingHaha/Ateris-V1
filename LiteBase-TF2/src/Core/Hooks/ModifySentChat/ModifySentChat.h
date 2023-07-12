#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace ModifySentChat
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, char *, int );
		void  __fastcall Func( void *ecx, void *edx, char *pBuf, int iBufSize );
		void Init( );
	}
}