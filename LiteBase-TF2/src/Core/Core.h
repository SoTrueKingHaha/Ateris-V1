#pragma once
#include "Hooks/Hooks.h"

class CCore
{
public:
	void Load();
	void Unload();
	bool ShouldUnload();
};

inline CCore g_Core;