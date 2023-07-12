#pragma once
#include "igameevents.h"
#include "../../../Utils/VFunc/VFunc.h"

class CGameEventListener
{
public:
	virtual ~CGameEventListener(void) {};

	virtual void FireGameEvent(IGameEvent* pEvent) = 0;
	virtual int GetEventDebugID() { return m_nDebug; }

private:
	int m_nDebug;
};

class CSVCMsg_GameEvent;
class CGameEventManager
{
public:
	virtual				~CGameEventManager() { }
	virtual int			LoadEventsFromFile(const char* szFileName) = 0;
	virtual void		Reset() = 0;
	virtual bool		AddListener(CGameEventListener* pListener, const char* szName, bool bServerSide) = 0;
	virtual bool		FindListener(CGameEventListener* pListener, const char* szName) = 0;
	virtual void		RemoveListener(CGameEventListener* pListener) = 0;
	virtual void		AddListenerGlobal(CGameEventListener* pListener, bool bServerSide) = 0;

	// virtual IGameEvent* CreateNewEvent(const char* szName, bool bForce = false, int* unknown = nullptr) = 0;
	IGameEvent* CreateNewEvent(const char* szName, bool bForce = false) {
		return GetVFunc<IGameEvent* (__thiscall*)(void*, const char*, bool)>(this, 6)(this, szName, bForce);
	}

	virtual bool		FireEvent(IGameEvent* pEvent, bool bDontBroadcast = false) = 0;
	virtual bool		FireEventClientSide(IGameEvent* pEvent) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* pEvent) = 0;
	virtual void		FreeEvent(IGameEvent* pEvent) = 0;
	virtual bool		SerializeEvent(IGameEvent* pEvent, CSVCMsg_GameEvent* pEventMsg) = 0;
	virtual IGameEvent* UnserializeEvent(const CSVCMsg_GameEvent& eventMsg) = 0;
	virtual void* GetEventDataTypes(IGameEvent* pEvent) = 0;
};

namespace I { inline CGameEventManager* GameEvent; };