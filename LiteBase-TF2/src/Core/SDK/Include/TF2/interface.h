#pragma once

class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};

typedef void *(*CreateInterfaceFn)(const char *pName, int *pReturnCode);
typedef void *(*InstantiateInterfaceFn)();