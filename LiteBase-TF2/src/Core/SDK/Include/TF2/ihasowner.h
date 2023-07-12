#pragma once
#include "baseentity.h"

class IHasOwner
{
public:
	virtual CBaseEntity *GetOwnerViaInterface(void) = 0;
};