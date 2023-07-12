#include "AttributeChanger.h"

#include "../../Visuals/Render/GUI/Menu/ConfigManager/ConfigManager.h"
#include "../../Vars.h"

constexpr int MAX_BUFFER = ( MAX_PATH * 3 );

std::array<int, 46> australium_table{
	4, 7, 13, 14, 15, 16, 18, 19, 20, 21, 29, 36, 38, 45, 61, 132, 141, 194, 197, 200, 201, 202, 203, 205, 206, 207,
	208, 211, 228, 424, 654, 658, 659, 662, 663, 664, 665, 669, 1000, 1004, 1006, 1007, 1078, 1082, 1085, 1149
};

std::array<std::pair<int, int>, 12> redirects{
	std::pair{264, 1071}, std::pair{18, 205}, std::pair{13, 200}, std::pair{21, 208}, std::pair{19, 206},
	std::pair{20, 207}, std::pair{15, 202}, std::pair{7, 197}, std::pair{29, 211}, std::pair{14, 201},
	std::pair{16, 203}, std::pair{4, 194}
};

void CAttributeChanger::Run( )
{
	if ( !I::EngineClient->IsInGame( ) || AttributeMap.empty( ) )
	{
		return;
	}

	//Needed here, runs 2 stages before cache is filled
	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		

		const auto myWeapons = reinterpret_cast< size_t * >( reinterpret_cast<DWORD>( pLocal ) + 0xCF8 );
		for ( int n = 0; myWeapons[ n ]; n++ )
		{
			if ( const auto &pWeapon = reinterpret_cast< CTFWeaponBase * >( I::ClientEntityList->GetClientEntityFromHandle( myWeapons[ n ] ) ) )
			{
				const auto pList = reinterpret_cast< CAttributeList * >( reinterpret_cast< DWORD >( pWeapon ) + 0x9C4 );
				if ( !pList || pList->m_Attributes.Count( ) > 0 )
				{
					continue;
				}

				const auto nIndex = pWeapon->m_iItemDefinitionIndex( );
				if ( AttributeMap.find( nIndex ) != AttributeMap.end( ) )
				{
					switch ( nIndex )
					{
						case 264:
							pWeapon->SetItemDefIndex( 1071 );
							break;
						case 18:
							pWeapon->SetItemDefIndex( 205 );
							break;
						case 13:
							pWeapon->SetItemDefIndex( 200 );
							break;
						case 21:
							pWeapon->SetItemDefIndex( 208 );
							break;
						case 19:
							pWeapon->SetItemDefIndex( 206 );
							break;
						case 20:
							pWeapon->SetItemDefIndex( 207 );
							break;
						case 15:
							pWeapon->SetItemDefIndex( 202 );
							break;
						case 7:
							pWeapon->SetItemDefIndex( 197 );
							break;
						case 29:
							pWeapon->SetItemDefIndex( 211 );
							break;
						case 14:
							pWeapon->SetItemDefIndex( 201 );
							break;
						case 16:
							pWeapon->SetItemDefIndex( 203 );
							break;
						case 4:
							pWeapon->SetItemDefIndex( 194 );
							break;
					}

					const auto &attrInfo = AttributeMap[ nIndex ];
					if ( attrInfo.StyleOverride )
					{
						pList->Add( ItemStyleOverride, true );
					}

					if ( attrInfo.Effect )
					{
						pList->Add( UnusualEffect, attrInfo.Effect );
					}

					if ( attrInfo.Particle )
					{
						pList->Add( ParticleEffect, attrInfo.Particle );
					}

					if ( attrInfo.Ancient )
					{
						pList->Add( AncientPowers, true );
					}

					if ( attrInfo.Sheen )
					{
						pList->Add( Sheen, attrInfo.Sheen );
					}

					if ( attrInfo.Australium )
					{
						pList->Add( IsAustraliumItem, attrInfo.Australium );
					}
				}
			}
		}
	}
}

void CAttributeChanger::Init( )
{
	LoadConfig( );
}

void CAttributeChanger::SaveConfig( )
{
	if ( AttributePath.empty( ) )
	{
		AttributePath = ( std::filesystem::current_path( ).wstring( ) + ( L"\\ateris" ) + ( L"\\Core\\ATTRIBUTES.ini" ) );
	}

	WriteStream = std::ofstream( AttributePath, std::ios::out );

	if ( WriteStream.is_open( ) )
	{
		for ( const auto &attribute : AttributeMap )
		{
			wchar_t szKey[ 85 ];
			swprintf( szKey, 85, L"%i", attribute.second.ItemDefIndex );

			SaveInt( szKey, L"Index", attribute.second.ItemDefIndex );
			SaveInt( szKey, L"Effect", attribute.second.Effect );
			SaveInt( szKey, L"Particle", attribute.second.Particle );
			SaveInt( szKey, L"Sheen", attribute.second.Sheen );
			SaveBool( szKey, L"Ancient", attribute.second.Ancient );
			SaveBool( szKey, L"StyleOverride", attribute.second.StyleOverride );
			SaveBool( szKey, L"Australium", attribute.second.Australium );
		}

		WriteStream.close( );
	}
}

void CAttributeChanger::LoadConfig( )
{
	if ( AttributePath.empty( ) )
	{
		AttributePath = ( std::filesystem::current_path( ).wstring( ) + ( L"\\ateris" ) + L"\\Core\\ATTRIBUTES.ini" );
	}

	ReadStream = std::ifstream( AttributePath );

	if ( ReadStream.is_open( ) )
	{
		wchar_t szSections[ MAX_BUFFER ];
		GetPrivateProfileSectionNamesW( szSections, MAX_BUFFER, AttributePath.c_str( ) );
		wchar_t *szSection = szSections;

		while ( *szSection != NULL )
		{
			const int nIndex = LoadInt( szSection, L"Index" );
			AttributeMap[ nIndex ] = {
				nIndex, LoadInt( szSection, L"Effect" ),
				LoadInt( szSection, L"Particle" ), LoadInt( szSection, L"Sheen" ),
				LoadBool( szSection, L"Ancient" ), LoadBool( szSection, L"StyleOverride" ), LoadBool( szSection, L"Australium" )
			};

			szSection += ( wcslen( szSection ) + 1 );
		}

		ReadStream.close( );
	}

	if ( const auto& pLocal = g_EntityCache.GetLocal( ); pLocal && !pLocal->IsDormant( ) && !pLocal->deadflag( ) )
		SetAttribute( );
}

void CAttributeChanger::SetAttribute( )
{
	//Needed here, runs 2 stages before cache is filled
	const auto& pLocal = g_EntityCache.GetLocal( );

	const auto& pWeapon = g_EntityCache.GetWeapon( );

	const int nIndex = pWeapon->m_iItemDefinitionIndex( );
	AttributeMap[ nIndex ] = {
		nIndex, Vars::Visuals::Skins::Effect.var,
		Vars::Visuals::Skins::Particle.var, Vars::Visuals::Skins::Sheen.var,
		Vars::Visuals::Skins::Acient.var, Vars::Visuals::Skins::Override.var, Vars::Visuals::Skins::Australium.var
	};

	I::ClientState->ForceFullUpdate( );
}