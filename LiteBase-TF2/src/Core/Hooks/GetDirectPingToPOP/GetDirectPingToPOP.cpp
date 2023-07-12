#include "GetDirectPingToPOP.h"

static const std::map<std::string, int> DataCenterHashMap{
	{"ams",  DC_AMS},
	{"atl",  DC_ATL},
	{"bom",  DC_BOM},
	{"dxb",  DC_DXB},
	{"eat",  DC_EAT},
	{"mwh",  DC_MWH},
	{"fra",  DC_FRA},
	{"gnrt", DC_GNRT},
	{"gru",  DC_GRU},
	{"hkg",  DC_HKG},
	{"iad",  DC_IAD},
	{"jnb",  DC_JNB},
	{"lax",  DC_LAX},
	{"lhr",  DC_LHR},
	{"lim",  DC_LIM},
	{"lux",  DC_LUX},
	{"maa",  DC_MAA},
	{"mad",  DC_MAD},
	{"man",  DC_MAN},
	{"okc",  DC_OKC},
	{"ord",  DC_ORD},
	{"par",  DC_PAR},
	{"scl",  DC_SCL},
	{"sea",  DC_SEA},
	{"sgp",  DC_SGP},
	{"sto",  DC_STO},
	{"sto2", DC_STO},
	{"syd",  DC_SYD},
	{"tyo",  DC_TYO},
	{"tyo2", DC_TYO},
	{"tyo1", DC_TYO},
	{"vie",  DC_VIE},
	{"waw",  DC_WAW}
};
bool IsRegionAllowed( int iIndex );

int __fastcall Hooks::GetDirectPingToPOP::Func( void *ecx, void *edx, SteamNetworkingPOPID popID )
{
	if ( !Vars::Misc::Regions::RegionSelector.var || !ecx || !edx )
	{
		return Hook.CallOriginal<fn>( )( ecx, edx, popID );
	}

	char popIDName[ 5 ];
	popID.ToString( popIDName );

	for ( auto &dc : DataCenterHashMap )
	{
		if ( dc.first == popIDName && IsRegionAllowed( dc.second ) )
		{
			return 1;
		}

		if ( dc.first == popIDName && !IsRegionAllowed( dc.second ) )
		{
			return std::numeric_limits<int>::max( );
		}
	}

	return Hook.CallOriginal<fn>( )( ecx, edx, popID );
}

void Hooks::GetDirectPingToPOP::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::NetworkUtils, 9 ) );
	Hook.Create( FN, Func );
}

bool IsRegionAllowed( int iIndex )
{
	switch ( iIndex )
	{
		case ( 1 << 6 ):
		case ( 1 << 25 ):
		case ( 1 << 29 ):
		case ( 1 << 28 ):
		{
			return Vars::Misc::Regions::EUEast.var;
			break;
		}
		case ( 1 << 0 ):
		case ( 1 << 13 ):
		case ( 1 << 15 ):
		case ( 1 << 17 ):
		case ( 1 << 21 ):
		{
			return Vars::Misc::Regions::EUWest.var;
			break;
		}
		case ( 1 << 18 ):
		case ( 1 << 24 ):
		{
			return Vars::Misc::Regions::Asia.var;
			break;
		}
		case ( 1 << 3 ):
		{
			return Vars::Misc::Regions::MiddleEast.var;
			break;
		}
		case ( 1 << 9 ):
		{
			return Vars::Misc::Regions::HongKong.var;
			break;
		}
		case ( 1 << 7 ):
		case ( 1 << 27 ):
		{
			return Vars::Misc::Regions::Japan.var;
			break;
		}
		case ( 1 << 2 ):
		case ( 1 << 16 ):
		{
			return Vars::Misc::Regions::India.var;
			break;
		}
		case ( 1 << 26 ):
		{
			return Vars::Misc::Regions::Australia.var;
			break;
		}
		case ( 1 << 11 ):
		{
			return Vars::Misc::Regions::Africa.var;
			break;
		}
		case ( 1 << 22 ):
		{
			return Vars::Misc::Regions::Chile.var;
			break;
		}
		case ( 1 << 14 ):
		{
			return Vars::Misc::Regions::Peru.var;
			break;
		}
		case ( 1 << 8 ):
		{
			return Vars::Misc::Regions::Brazil.var;
			break;
		}
		case ( 1 << 1 ):
		case ( 1 << 10 ):
		case ( 1 << 20 ):
		{
			return Vars::Misc::Regions::USEast.var;
			break;
		}
		case ( 1 << 4 ):
		case ( 1 << 5 ):
		case ( 1 << 23 ):
		{
			return Vars::Misc::Regions::USNorthWest.var;
			break;
		}
		case ( 1 << 12 ):
		case ( 1 << 19 ):
		{
			return Vars::Misc::Regions::USSouthWest.var;
			break;
		}
		default: return true;
	}

	return true;
}