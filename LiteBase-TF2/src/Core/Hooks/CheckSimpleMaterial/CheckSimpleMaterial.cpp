#include "CheckSimpleMaterial.h"

bool __cdecl Hooks::CheckSimpleMaterial::Func( IMaterial * pMaterial)
{
	return true;
	//return Hook.CallOriginal<fn>( )( pMaterial );
}

void Hooks::CheckSimpleMaterial::Init( )
{
	void *pSrc = reinterpret_cast<void*>(g_Pattern.Find( L"engine.dll", L"55 8B EC 56 8B 75 08 85 F6 75 05 32 C0 5E 5D C3 8B 06 8B CE 57 FF 90 ? ? ? ? 6A 10 8B F8 68 ? ? ? ? 57 E8 ? ? ? ? 83 C4" ) );
	Hook.Create( pSrc, Func );
}