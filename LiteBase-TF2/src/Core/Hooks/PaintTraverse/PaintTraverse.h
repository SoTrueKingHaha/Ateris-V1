#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Misc/Misc.h"
#include "../../SDK/Include/TF2/Panel.h"

namespace Hooks
{
	namespace PaintTraverse
	{
		inline CHook Hook;
		using fn = void( __thiscall * )( CPanel *, unsigned int, bool, bool );
		void __stdcall Func( unsigned int vgui_panel, bool force_repaint, bool allow_force );
		void Init( );
	}
}