#pragma once
#include "../../SDK/SDK.h"
#include <mutex>

namespace Hooks
{
	namespace RenderView
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void*, void*, const CViewSetup&, ClearFlags_t, RenderViewInfo_t );
		void __fastcall Func( void *ecx, void *edx, const CViewSetup &view, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw );
		void Init( );
	}
}