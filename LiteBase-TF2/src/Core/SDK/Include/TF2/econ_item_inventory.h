#pragma once
#include "gcclient_sharedobjectcache.h"

class CPlayerInventory;

class IInventoryUpdateListener : public ISharedObjectListener
{
public:
	virtual void InventoryUpdated(CPlayerInventory *pInventory) = 0;
	virtual void SOCreated(const CSteamID &steamIDOwner, const CSharedObject *pObject, ESOCacheEvent eEvent) = 0;
	virtual void PreSOUpdate(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
	virtual void SOUpdated(const CSteamID &steamIDOwner, const CSharedObject *pObject, ESOCacheEvent eEvent) = 0;
	virtual void PostSOUpdate(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
	virtual void SODestroyed(const CSteamID & steamIDOwner, const CSharedObject * pObject, ESOCacheEvent eEvent) = 0;
	virtual void SOCacheSubscribed(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
	virtual void SOCacheUnsubscribed(const CSteamID &steamIDOwner, ESOCacheEvent eEvent) = 0;
};