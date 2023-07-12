#pragma once
#include "../../../SDK/SDK.h"
#include "../../../SDK/Include/TF2/proxyfnhook.h"

class NetVarHooks
{
private:
	bool HookNetVar( std::vector<std::string> path, ProxyFnHook &hook, RecvVarProxyFn function );

public:
	void Init( );
};

inline NetVarHooks g_NetVarHk;