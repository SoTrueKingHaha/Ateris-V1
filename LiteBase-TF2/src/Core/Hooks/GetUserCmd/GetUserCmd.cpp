#include "GetUserCmd.h"

inline CUserCmd *GetCmds( )
{
	return *reinterpret_cast< CUserCmd ** >( reinterpret_cast< DWORD >( I::Input ) + 0xFC );
}

CUserCmd *__stdcall Hooks::GetUserCmd::Func( int sequence_number )
{
	return &GetCmds( )[ sequence_number % 90 ];
}
void Hooks::GetUserCmd::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::Input, 8 ) );
	Hook.Create( FN, Func );
}