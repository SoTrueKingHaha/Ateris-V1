#include "../../Features/Visuals/Render/GUI/Menu/GUI.h"
#include "KeyEvent.h"

int __fastcall Hooks::KeyEvent::Func( IBaseClientDLL *ecx, PVOID edx, int event_code, int key_num, const char *current_binding )
{
	static auto original = Hook.CallOriginal<fn>( )( ecx, edx, event_code, key_num, current_binding );

	if ( g_Menu.m_bOpen )
		return 0;

	return original;
}

void Hooks::KeyEvent::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::BaseClientDLL, 20 ) );
	Hook.Create( FN, Func );
}