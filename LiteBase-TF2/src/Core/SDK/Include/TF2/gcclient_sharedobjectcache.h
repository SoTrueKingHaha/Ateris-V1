#pragma once

class CSteamID;
class CSharedObject;

enum ESOCacheEvent
{
	eSOCacheEvent_None = 0,
	eSOCacheEvent_Subscribed = 1,
	eSOCacheEvent_Unsubscribed = 2,
	eSOCacheEvent_Resubscribed = 3,
	eSOCacheEvent_Incremental = 4,
	eSOCacheEvent_ListenerAdded = 5,
	eSOCacheEvent_ListenerRemoved = 6,
};

class ISharedObjectListener
{
public:
	virtual void SOCreated(const CSteamID &steamIDOwner, const CSharedObject *pObject, ESOCacheEvent eEvent) = 0;
	virtual void PreSOUpdate(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
	virtual void SOUpdated(const CSteamID &steamIDOwner, const CSharedObject *pObject, ESOCacheEvent eEvent) = 0;
	virtual void PostSOUpdate(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
	virtual void SODestroyed(const CSteamID &steamIDOwner, const CSharedObject *pObject, ESOCacheEvent eEvent) = 0;
	virtual void SOCacheSubscribed(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
	virtual void SOCacheUnsubscribed(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
};