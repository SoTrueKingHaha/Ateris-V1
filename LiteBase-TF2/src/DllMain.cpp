#include "Core/Core.h"

DWORD WINAPI MainThread(LPVOID lpParam)
{
	g_Core.Load();
	
	while (!g_Core.ShouldUnload())
		Sleep(100);

	g_Core.Unload();

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hinstDLL);

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (HANDLE hMainThread = CreateThread(0, 0, MainThread, hinstDLL, 0, 0))
			CloseHandle(hMainThread);
	}

	return TRUE;
}