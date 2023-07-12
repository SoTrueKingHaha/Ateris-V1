#pragma once
#include <Windows.h>
#include "../../Utils/WinAPI/WinAPI.h"

class CInterface
{
private:
	typedef PVOID(*InstantiateInterface)();

	struct Interface_t
	{
		InstantiateInterface Interface;
		PCHAR szInterfaceName;
		Interface_t *NextInterface;
	};

public:
	PVOID Get(LPCWSTR szModule, PCCH szObject);

	template<typename T>
	__inline T Get( LPCWSTR szModule, PCCH szObject )
	{
		typedef void *( *CreateFn )( const char *pName, int *pReturnCode );

		if ( const auto dwModule = reinterpret_cast< DWORD >( WinAPI::GetModuleHandleW( szModule ) ) )
		{
			if ( const auto Factory = reinterpret_cast< CreateFn >( WinAPI::GetProcessAddr( dwModule, ( "CreateInterface" ) ) ) )
				return reinterpret_cast< T >( Factory( szObject, nullptr ) );
		}

		return nullptr;
	}
};

inline CInterface g_Interface;