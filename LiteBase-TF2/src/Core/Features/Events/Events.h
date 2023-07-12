#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/TF2/GameEventListener.h"
#include "../../Utils/Hash/FNV1A.h"
#include <deque>

static std::string yellow( { '\x7', 'E', 'D', 'B', '6', '2', '8' } ); //EDB628
static std::string blue( { '\x7', '0', 'D', '9', '2', 'F', 'F' } ); //0D92FF
static std::string white( { '\x7', 'F', 'F', 'F', 'F', 'F', 'F' } ); //FFFFFF
static std::string red( { '\x7', 'F', 'F', '3', 'A', '3', 'A' } ); //FF3A3A
static std::string green( { '\x7', '3', 'A', 'F', 'F', '4', 'D' } ); //3AFF4D

class IEvents
{
public:
	virtual void Setup( const std::deque<const char *> &arrEvents ) = 0;
	virtual void Destroy( ) = 0;
	virtual void GameEvents( IGameEvent *pEvent, const FNV1A_t uNameHash ) = 0;
	virtual void OnHurt( IGameEvent *pEvent, const FNV1A_t uNameHash ) = 0;
};

class CEvents : public IEvents, public CGameEventListener
{
public:
	virtual void Setup( const std::deque<const char *> &arrEvents ) override;
	virtual void Destroy( ) override;
	void PrintOnAttach( );
	void PrintOnDetach( );

	virtual void FireGameEvent( IGameEvent *pEvent ) override;
	virtual int GetEventDebugID( ) override { return 42; }
	virtual void OnHurt( IGameEvent *pEvent, const FNV1A_t uNameHash ) override;

	virtual void GameEvents( IGameEvent *pEvent, const FNV1A_t uNameHash ) override;
};

inline CEvents g_Events;