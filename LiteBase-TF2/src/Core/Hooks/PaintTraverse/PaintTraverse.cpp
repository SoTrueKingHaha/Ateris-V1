#include "PaintTraverse.h"

void __stdcall Hooks::PaintTraverse::Func( unsigned int vgui_panel, bool force_repaint, bool allow_force )
{
	if ( g_Misc.RemoveScope( vgui_panel ) )
		return;

	Hook.CallOriginal<fn>( )( I::Panel, vgui_panel, force_repaint, allow_force );
}

void Hooks::PaintTraverse::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::Panel, 41 ) );
	Hook.Create( FN, Func );
}