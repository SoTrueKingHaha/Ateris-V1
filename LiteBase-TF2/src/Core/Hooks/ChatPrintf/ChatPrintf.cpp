#include "ChatPrintf.h"

struct ChatFlags_t
{
	std::string Color;
	const char *Name;
};
inline Color_t local = { 16, 255, 32, 255 };

void __cdecl Hooks::ChatPrintf::Func( void *ecx, int iPlayerIndex, int iFilter, const char *fmt, ... )
{
	va_list marker;
	char buffer[ 4096 ];
	va_start( marker, fmt );
	vsnprintf_s( buffer, sizeof( buffer ), fmt, marker );
	va_end( marker );

	if ( strlen( buffer ) > 0 && buffer[ strlen( buffer ) - 1 ] == '\n' )
	{
		buffer[ strlen( buffer ) - 1 ] = 0;
	}
	char *msg = buffer;
	while ( *msg && ( *msg == '\n' || *msg == '\r' || *msg == '\x1A' ) )
	{
		msg++;
	}
	if ( !*msg ) { return; }

	std::string finalMsg = msg, name = {};

	player_info_t info{};
	if ( !I::EngineClient->GetPlayerInfo( iPlayerIndex, &info ) )
	{
		return Hook.CallOriginal<fn>( )( ecx, iPlayerIndex, iFilter, "%s", finalMsg.c_str( ) );
	}

	name = info.m_sName;
	if ( finalMsg.find( name ) == std::string::npos )
	{
		return Hook.CallOriginal<fn>( )( ecx, iPlayerIndex, iFilter, "%s", finalMsg.c_str( ) );
	}

	if ( iPlayerIndex/* && Vars::Misc::ChatFlags.var */ )
	{
		ChatFlags_t chatFlag;
		bool flagSet = false;

		if ( iPlayerIndex == I::EngineClient->GetLocalPlayer( ) )
		{
			chatFlag = { local.to_hex_alpha( ), "[You]" };
			flagSet = true;
		}
		else if ( GET_ENT_I( iPlayerIndex )->As<CTFPlayer>( )->PlayerOnFriendsList( ) )
		{
			chatFlag = { local.to_hex_alpha( ), "[Friend]" };
			flagSet = true;
		}
		else if ( I::EngineClient->GetPlayerInfo( iPlayerIndex, &info ) && g_GlobalInfo.PlayerPriority[ info.friendsID ].Mode == 4 )
		{
			static constexpr auto RED = Color_t{ 255, 0, 0, 255 };
			chatFlag = { RED.to_hex_alpha( ), "[Cheater]" };
			flagSet = true;
		}

		if ( flagSet )
		{
			finalMsg = chatFlag.Color + chatFlag.Name + " \x3" + finalMsg;
			if ( auto offset = finalMsg.find( name ) )
			{
				finalMsg = finalMsg.replace( offset + name.length( ), 0, "\x1" );
			}
		}
	}

	Hook.CallOriginal<fn>( )( ecx, iPlayerIndex, iFilter, "%s", finalMsg.c_str( ) );
}

void Hooks::ChatPrintf::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::ClientModeShared->m_pChatElement, 19 ) );
	Hook.Create( FN, Func );
}