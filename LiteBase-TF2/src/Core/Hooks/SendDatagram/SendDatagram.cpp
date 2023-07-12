#include "SendDatagram.h"

int __fastcall Hooks::SendDatagram::Func( INetChannel *netChannel, void *edx, bf_write *datagram )
{
	if ( CTFPlayer *pLocal = GET_ENT_I( LOCAL_IDX )->As<CTFPlayer>( ); !pLocal || pLocal->deadflag( ) || pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) ) { g_Backtrack.AllowLatency = false; return Hook.CallOriginal<fn>( )( netChannel, edx, datagram ); }
	if ( !Vars::Backtrack::Enabled.var || !Vars::Backtrack::Latency.var
		|| !netChannel || netChannel->IsLoopback( )
		|| !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) || I::EngineClient->IsDrawingLoadingImage( ) )
	{
		g_BacktrackNew.bFakeLatency = false;
		return Hook.CallOriginal<fn>( )( netChannel, edx, datagram );
	}

	g_BacktrackNew.bFakeLatency = true;

	const int inSequence = netChannel->m_nInSequenceNr;
	const int inState = netChannel->m_nInReliableState;

	g_BacktrackNew.AdjustPing( netChannel );

	const int original = Hook.CallOriginal<fn>( )( netChannel, edx, datagram );
	netChannel->m_nInSequenceNr = inSequence;
	netChannel->m_nInReliableState = inState;

	return original;
}

void Hooks::SendDatagram::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B D9" ) );
	Hook.Create( pSrc, Func );
}