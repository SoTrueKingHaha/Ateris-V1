#include "Indicators.h"
#include "../../../Models/Models.h"
#include "Arrows.h"

void CIndicators::Run( )
{
	if ( !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) || I::EngineVGui->IsGameUIVisible( ) ) { return; }
	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		if ( !pLocal || pLocal->deadflag( ) || pLocal->IsDormant( ) )
			return;

		DrawDT( );
		if ( const auto &pWeapon = g_EntityCache.GetWeapon( ) )
		{
			if ( pWeapon && !pWeapon->IsDormant( ) )
				g_Crits.Render( );
		}
		DrawSpectators( );
		DrawBT( );
		DrawInfo( );
		DrawAA( );
		g_Arrows.Run( );
		//DrawSpectators( );
	}
}

void CIndicators::DrawDT( )
{
	if ( !Vars::Misc::CL_Move::Indicator.var || Vars::Misc::StacBypass.var ) { return; }

	const auto& pLocal = g_EntityCache.GetLocal( );

	switch ( Vars::Misc::CL_Move::Indicator.var )
	{
		case 1:
		{
			const int offset = 3;
			int shifted = 0;
			if ( Vars::Misc::CL_Move::DTTicksMoving.var < Vars::Misc::CL_Move::DTTicksStanding.var )
			{
				shifted = Vars::Misc::CL_Move::DTTicksMoving.var;
			}
			else if ( Vars::Misc::CL_Move::DTTicksMoving.var > Vars::Misc::CL_Move::DTTicksStanding.var )
			{
				shifted = Vars::Misc::CL_Move::DTTicksStanding.var;
			}
			else
			{
				shifted = Vars::Misc::CL_Move::DTTicksMoving.var;
			}

			const float ratio = std::clamp( ( ( shifted - g_GlobalInfo.m_nShifted ) / ( float )shifted ), 0.f, 1.f );
			const float alpha = std::clamp( 255.f * ratio, 0.f, 255.f );
			const float rectalpha = std::clamp( 222.f * ratio, 0.f, 222.f );
			static int y = m_nNextIndicatorHeight;
			I::Surface->DrawSetColor( { 0, 0, 0, ( byte )alpha } );
			I::Surface->DrawFilledRectFade( 0, y + 2, 126, y + 2 + 51, rectalpha, 0, true );

			I::Surface->DrawSetColor( { Vars::Menu::Colors::CustomTitle.r,
										Vars::Menu::Colors::CustomTitle.g,
										Vars::Menu::Colors::CustomTitle.b,
										( byte )alpha } );

			I::Surface->DrawFilledRectFade( 0, y + 1, 126, y + 2, alpha, 0, true );
			I::Surface->DrawFilledRectFade( 0, y + 1 + 51, 126, y + 2 + 51, alpha, 0, true );

			static int texture = g_Draw.CreateTextureFromArray( g_RawData.rawData, 106, 36 );
			g_Draw.DrawTextureRect( 5 - offset, y + ( offset * 2 ), 106, 36, texture, { 90, 90, 90, ( byte )alpha } );
			g_Draw.DrawTextureRect( 5, y + ( offset * 4 ), 106, 36, texture, { 180, 180, 180, ( byte )alpha } );
			m_nNextIndicatorHeight = y + 2 + 51;
			break;
		}
		case 2:
		{
			int w = 110;
			int h = 6;
			int x = g_Draw.m_nScreenC - w / 2;
			int y = g_Draw.m_nScreenH / 2 + 100;
			const float ratioCurrent = std::clamp( ( ( ( float )g_GlobalInfo.m_nTicksToShift - ( float )g_GlobalInfo.m_nShifted ) / ( float )g_GlobalInfo.m_nTicksToShift ), 0.0f, 1.0f );
			static float ratioInterp = 0.00f; ratioInterp = g_Draw.EaseIn( ratioInterp, ratioCurrent, 0.95f ); Utils::Clamp( ratioInterp, 0.00f, 1.00f );

			if ( g_GlobalInfo.m_nShifted > g_GlobalInfo.m_nTicksToShift - 1 && iAlpha > 0 )
			{
				if ( iAlpha )
					iAlpha -= 3;
			}

			else if ( g_GlobalInfo.m_nShifted < g_GlobalInfo.m_nTicksToShift && iAlpha < 256 )
			{
				if ( iAlpha != 255 )
					iAlpha += 3;
			}

			g_Draw.Rect( x + 1, y + 1, ( w - 2 ), h - 2, { 39, 35, 37, iAlpha } );
			g_Draw.OutlinedRect( x, y, w, h, { 0, 0, 0, iAlpha } );
			g_Draw.GradientRectWH( x + 1, y + 1, ratioInterp * ( w - 2 ), h - 2, { 255, 100, 0, iAlpha }, { 0, 0, 255, iAlpha }, true );

			if ( g_GlobalInfo.m_nWaitForShift && !g_GlobalInfo.m_bRecharging )
			{
				g_Draw.String( g_Fonts[ EFonts::WEAPON ].m_dwFont, g_Draw.m_nScreenC, y - 11, { 255, 255, 255, iAlpha }, ALIGN_CENTERHORIZONTAL, "%.2f", TICKS_TO_TIME( g_GlobalInfo.m_nWaitForShift ) );
			}
			break;
		}

		case 3:
		{
			int w = 80;
			int h = 3;
			int offset = 100;//30;
			int x = g_Draw.m_nScreenC - w / 2;
			int y = g_Draw.m_nScreenH / 2 + offset;

			const float ratio = std::clamp( ( static_cast< float >( 24 - g_GlobalInfo.m_nShifted ) / 24.f ), 0.0f, 1.0f );

			g_Draw.GradientRectWH( x - 1, y - 1, w + 2, h + 2, { 35, 35, 35, 255 }, { 20, 20, 20, 255 }, false );
			g_Draw.OutlinedRect( x - 1, y - 1, w + 2, h + 2, { 20, 20, 20, 255 } );
			g_Draw.GradientRectWH( x, y, w * ratio, h, { 255, 255, 255, 255 }, Vars::Menu::Colors::CustomTitle, true );

			if ( g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_bRecharging )
			{
				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 191, 70, 70, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(RapidFire) Too Expensive %i < %i", 24 - g_GlobalInfo.m_nShifted, g_GlobalInfo.m_nTicksToShift );

				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall * 2, { 191, 70, 70, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(Teleport) Too Expensive %i < %i", 24 - g_GlobalInfo.m_nShifted, 24 );
			}
			else if ( g_GlobalInfo.m_bRecharging )
			{
				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 191, 121, 50, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(Recharging) %i/%i", 24 - g_GlobalInfo.m_nShifted, 24 );
			}
			else if ( g_GlobalInfo.m_nWaitForShift )
			{
				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 191, 121, 50, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(RapidFire) Wait %i/%i", DT_WAIT_CALLS - g_GlobalInfo.m_nWaitForShift, DT_WAIT_CALLS );

				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall * 2, { 60, 160, 110, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(Teleport) Ready" );
			}
			else
			{
				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 60, 160, 110, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(RapidFire) Ready" );

				g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall * 2, { 60, 160, 110, 255 }, ALIGN_CENTERHORIZONTAL,
							   "(Teleport) Ready" );
			}
			break;
		}

		case 4:
		{
			int width = 80;
			int height = 4;
			const float ratio = std::clamp( ( ( 24.f - g_GlobalInfo.m_nShifted ) / 24.f ), 0.0f, 1.0f );
			const float waitratio = std::clamp( ( ( float )( DT_WAIT_CALLS - g_GlobalInfo.m_nWaitForShift ) / DT_WAIT_CALLS ), 0.0f, 1.0f );
			float elapsed = 0.0f;
			float duration = 0.2f;
			static float timer = 0.0f;

			g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ) - 1, ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) - 1, width + 2, height + 2, { 0, 0, 0, 255 } );
			g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ), ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ), width * ratio, height, { 215, 215, 70, 255 } );
			g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ) + 1, ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + 1, width * ratio - 2, height - 2, { 0, 0, 0, 150 } );

			if ( g_GlobalInfo.m_nWaitForShift != 0 )
			{
				g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ) - 1, ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + height + 3, width + 2, height + 2, { 0, 0, 0, 255 } );
				g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ), ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + height + 4, width * waitratio, height, { 215, 180, 70, 255 } );
				g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ) + 1, ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + height + 5, width * waitratio - 2, height - 2, { 0, 0, 0, 150 } );
				timer = I::EngineClient->Time( );
			}

			if ( !g_GlobalInfo.m_nWaitForShift )
			{
				elapsed = I::EngineClient->Time( ) - timer;

				if ( elapsed < duration )
				{
					g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ) - 1, ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + height + 3, width + 2, height + 2, { 0, 0, 0, 255 } );
					g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ), ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + height + 4, width * waitratio, height, { 215, 180, 70, 255 } );
					g_Draw.Rect( ( g_Draw.m_nScreenW / 2 ) - ( width / 2 ) + 1, ( g_Draw.m_nScreenH / 2 ) + ( g_Draw.m_nScreenH / 14 ) + height + 5, width * waitratio - 2, height - 2, { 0, 0, 0, 150 } );
				}
			}
			break;
		}

		case 5:
		{
			if ( g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_bRecharging )
			{
				g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + 100, { 250, 0, 0, 255 }, ALIGN_CENTERHORIZONTAL, L"[ ] Tick-Shifting Exploit" );
			}
			else if ( g_GlobalInfo.m_bRecharging )
			{
				g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + 100, { 150, 150, 150, 255 }, ALIGN_CENTERHORIZONTAL, L"[\\] Tick-Shifting Exploit" );
			}
			else if ( g_GlobalInfo.m_nWaitForShift )
			{
				g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + 100, { 250, 150, 0, 255 }, ALIGN_CENTERHORIZONTAL, L"[/] Tick-Shifting Exploit" );
			}
			else
			{
				g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + 100, { 0, 250, 0, 255 }, ALIGN_CENTERHORIZONTAL, L"[X] Tick-Shifting Exploit" );
			}
			break;
		}

		case 6:
		{
			int w = 155;
			int h = 12;
			int x = g_Draw.m_nScreenC - w / 2;
			int y = g_Draw.m_nScreenH / 2 + 100;

			int nShifted = g_GlobalInfo.m_nTicksToShift == 21 && g_GlobalInfo.m_nShifted > 18 ? g_GlobalInfo.m_nShifted + 2 : g_GlobalInfo.m_nShifted;
			const float ratioCurrent = std::clamp( ( g_GlobalInfo.m_nTicksToShift - (float)nShifted ) / g_GlobalInfo.m_nTicksToShift, 0.0f, 1.0f );
			static float ratioInterp = 0.00f; ratioInterp = g_Draw.EaseIn( ratioInterp, ratioCurrent, 0.95f ); Utils::Clamp( ratioInterp, 0.00f, 1.00f );

			g_Draw.Rect( x, y, w, h, { 20, 20, 20, 255 } );
			g_Draw.GradientRectWH( x, y, w, h, { 28, 28, 28, 255 }, { 20, 20, 20, 255 }, false );
			g_Draw.GradientRectWH( x, y, ratioInterp * w, h, { 50, 100, 255, 255 }, { 50, 200, 255, 255 }, true );
			g_Draw.OutlinedRect( x - 1, y - 1, w + 2, h + 2, { 0, 0, 0, 255 } );

			if ( g_GlobalInfo.m_bRecharging || g_GlobalInfo.m_nWaitForShift )
				g_Draw.Rect( x, y, w * ratioInterp, h, { 0, 0, 0, 150 } );

			byte alpha = g_GlobalInfo.m_nWaitForShift || g_GlobalInfo.m_nShifted ? 80 : 255;
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + w / 2, y - g_Fonts[ EFonts::NAME ].m_nTall - 2, { 255, 255, 255, alpha }, ALIGN_CENTERHORIZONTAL, L"CHARGE" );

			break;
		}
		default: break;
	}
}

void CIndicators::DrawCrits( )
{
	if ( !Vars::CritHack::Indicators.var ) { return; }
	if ( /*!g_Crits.IsEnabled( ) ||*/ !g_GlobalInfo.currentUserCmd ) { return; }

	const auto& pLocal = g_EntityCache.GetLocal( );
	
	const auto& pWeapon = g_EntityCache.GetWeapon( );

	const int x = g_Draw.m_nScreenC;
	int currentY = g_Draw.m_nScreenH / 2 + g_Draw.m_nScreenH / 4;

	const float bucket = *reinterpret_cast< float * >( reinterpret_cast< DWORD >( pWeapon ) + 0xA54 );
	const int seedRequests = *reinterpret_cast< int * >( pWeapon + 0xA5C );

	static auto tf_weapon_criticals_bucket_cap = g_ConVars.FindVar( "tf_weapon_criticals_bucket_cap" );
	const float bucketCap = tf_weapon_criticals_bucket_cap->GetFloat( );
	const float ratio = bucket / bucketCap;

	int longestW = 40;

	//g_Draw.String( g_Fonts[ EFonts::WEAPON ].m_dwFont, 150 / 2 + 20, 500 - g_Fonts[EFonts::WEAPON ].m_nTall, { 255, 255, 255, 255, }, ALIGN_CENTERHORIZONTAL, "%x", reinterpret_cast< float * >( pWeapon + 0xA54 ) );
	g_Draw.Rect( 20, 500, 150, 8, { 50, 50, 50, 255 } );
	//g_Draw.GradientRect( 20, 500 + 4, 20 + 150, 500 + 8, { 0, 0, 0, 0 }, { 255, 255, 255, 10 }, false );
	g_Draw.OutlinedRect( 20, 500, 150, 8, { 0, 0, 0, 255 } );
	g_Draw.Rect( 20 + 1, 500 + 1, ( 150 - 2 ) * ratio, 8 - 2, Vars::Colors::LocalPlayer );
	g_Draw.Line( ( 150 - 2 ) * ratio + 21, 500, ( 150 - 2 ) * ratio + 21, 500 + 7, { 0, 0, 0, 255 } );
	/*g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, x, currentY += 15, { 255, 255, 255, 255, }, ALIGN_CENTERHORIZONTAL, "%x", reinterpret_cast< float * >( pWeapon + 0xA54 ) );

	// Are we currently forcing crits?
	if ( ShouldCrit( ) )
	{
		g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, x, currentY += 15, { 70, 190, 50, 255 }, ALIGN_CENTERHORIZONTAL, "Forcing crits..." );
	}*/
	/*const std::wstring bucketstr = L"Bucket: " + std::to_wstring( static_cast< int >( bucket ) ) + L"/" + std::to_wstring( static_cast< int >( bucketCap ) );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, x, currentY += 15, { 181, 181, 181, 255 }, ALIGN_CENTERHORIZONTAL, bucketstr.c_str( ) );
	int w, h;
	I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, bucketstr.c_str( ), w, h );
	if ( w > longestW )
	{
		longestW = w;
	}

	const std::wstring seedText = L"m_nCritSeedRequests: " + std::to_wstring( seedRequests );
	const std::wstring FoundCrits = L"Found Crit Ticks: " + std::to_wstring( CritTicks.size( ) );
	const std::wstring commandNumber = L"cmdNumber: " + std::to_wstring( g_GlobalInfo.currentUserCmd->command_number );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, x, currentY += 15, { 181, 181, 181, 255 }, ALIGN_CENTERHORIZONTAL, seedText.c_str( ) );
	I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, seedText.c_str( ), w, h );
	if ( w > longestW )
	{
		longestW = w;
	}
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, x, currentY += 15, { 181, 181, 181, 255 }, ALIGN_CENTERHORIZONTAL, FoundCrits.c_str( ) );
	I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, FoundCrits.c_str( ), w, h );
	if ( w > longestW )
	{
		longestW = w;
	}
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, x, currentY += 15, { 181, 181, 181, 255 }, ALIGN_CENTERHORIZONTAL, commandNumber.c_str( ) );
	I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, commandNumber.c_str( ), w, h );
	if ( w > longestW )
	{
		longestW = w;
	}

	IndicatorW = longestW * 2;
	IndicatorH = currentY;*/
}

static void printStatus( bool bStatus, const wchar_t *basestr, int &x, int &y ) {

	/*I::Surface->DrawSetTextFont( g_Fonts[ EFonts::HARPOON ].m_dwFont );
	I::Surface->DrawSetTextColor( 150, 150, 250, 255 );
	const wchar_t *status = ( bStatus ? L"ON" : L"OFF" );
	std::wstring currtime = { std::wstring( status ) + L" - " + basestr };
	int w, h = 0;
	I::Surface->GetTextSize( 5, currtime.c_str( ), w, h );

	int w2, h2 = 0;
	currtime = { L" - " + std::wstring( basestr ) };
	I::Surface->GetTextSize( 5, currtime.c_str( ), w2, h2 );

	I::Surface->DrawSetTextPos( x - w2, y );
	I::Surface->DrawPrintText( currtime.c_str( ), wcslen(currtime.c_str( ) ) );
	if ( bStatus ) {
		I::Surface->DrawSetTextColor( 150, 255, 150, 255 );
	}
	else {
		I::Surface->DrawSetTextColor( 250, 150, 150, 255 );
	}
	I::Surface->DrawSetTextPos( x - w, y );
	y += h + 2;
	I::Surface->DrawPrintText( status, wcslen(status) );
	*/

	int w, h = 0;
	const wchar_t* status = ( bStatus ? L"ON" : L"OFF" );
	std::wstring str = { L" - " + std::wstring( basestr ) };
	Color_t status_t = ( bStatus ? Color_t{ 150, 255, 150, 255 } : Color_t{ 250, 150, 150, 255 } );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, x, y, { 150, 150, 250, 255 }, ALIGN_REVERSE, str.c_str( ) );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, str.c_str( ), w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, x - w, y, status_t, ALIGN_REVERSE, status );

	y += g_Fonts[ EFonts::HARPOON ].m_nTall;
}

void CIndicators::DrawInfo( )
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	const auto &pWeapon = g_EntityCache.GetWeapon( );

	if ( !Vars::Visuals::Debug.var )
		return;

	if ( g_GlobalInfo.console )
		g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 1355, 1024 - 1, { 230, 230, 180, 255 }, 0, L"i hate niggers" );

	int w, h = 0;
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 520, { 150, 150, 250, 255 }, 0, L"m_nTickBase:");
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"m_nTickBase: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 520, { 150, 255, 150, 255 }, 0, L"%i", pLocal->m_nTickBase( ) );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 540, { 150, 150, 250, 255 }, 0, L"m_nFinalTick:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"m_nFinalTick: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 540, { 250, 150, 150, 255 }, 0, L"%i", pLocal->m_nFinalTick( ) );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 570, { 150, 150, 250, 255 }, 0, L"pCmd->forwardmove:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"pCmd->forwardmove: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 570, { 150, 255, 150, 255 }, 0, L"%.1f", g_GlobalInfo.forwardmove );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 590, { 150, 150, 250, 255 }, 0, L"pCmd->sidemove:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"pCmd->sidemove: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 590, { 250, 150, 150, 255 }, 0, L"%.1f", g_GlobalInfo.sidemove );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 610, { 150, 150, 250, 255 }, 0, L"pCmd->tick_count:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"pCmd->tick_count: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 610, { 150, 255, 150, 255 }, 0, L"%i", g_GlobalInfo.tickCount );


	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 640, { 150, 150, 250, 255 }, 0, L"m_flSimulationTime:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"m_flSimulationTime: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 640, { 150, 255, 150, 255 }, 0, L"%.2f", pLocal->m_flSimulationTime( ) );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 660, { 150, 150, 250, 255 }, 0, L"m_flOldSimulationTime:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"m_flOldSimulationTime: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 660, { 250, 150, 150, 255 }, 0, L"%.2f", pLocal->GetOldSimulationTime( ) );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 680, { 150, 150, 250, 255 }, 0, L"m_flAnimTime:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"m_flAnimTime: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 680, { 150, 255, 150, 255 }, 0, L"%.2f", pLocal->m_flAnimTime( ) );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 700, { 150, 150, 250, 255 }, 0, L"m_flOldAnimTime:" );
	I::Surface->GetTextSize( g_Fonts[ EFonts::HARPOON ].m_dwFont, L"m_flOldAnimTime: ", w, h );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5 + w, 700, { 250, 150, 150, 255 }, 0, L"%.2f", pLocal->GetOldAnimTime( ) );

	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 720, { 150, 150, 250, 255 }, 0, L"%i", I::ClientState->chokedcommands );
	//g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 730, { 150, 150, 250, 255 }, 0, L"%i", pWeapon->m_iWeaponMode( ) );
	if ( pWeapon->GetWeaponID( ) == TF_WEAPON_MINIGUN )
		g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 730, { 150, 150, 250, 255 }, 0, L"%i", pWeapon->As<CTFMinigun>( )->m_iWeaponState( ) );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 750, { 150, 150, 250, 255 }, 0, L"Init: %i", Models.iInitCount / 9 );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 760, { 150, 150, 250, 255 }, 0, L"Validate: %i", Models.iValidCount / 9 );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 770, { 150, 150, 250, 255 }, 0, L"Delete: %i", Models.iDelCount );
	g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 790, { 150, 150, 250, 255 }, 0, L"lol: %x", pLocal->GetAnimState( ) );

	g_Draw.OutlinedString( g_Fonts[ EFonts::CONDS ].m_dwFont, 5, 810, { 150, 255, 150, 255 }, ALIGN_DEFAULT, L"%i", pWeapon->GetWeaponID( ) );

	/*static auto plugins = g_ConVars.FindVar( "sm plugins" );
	const char *info = plugins->GetString( );
	if ( strlen( info ) != 0 )
		g_Draw.String( g_Fonts[ EFonts::HARPOON ].m_dwFont, 5, 810, { 150, 150, 250, 255 }, 0, info );*/

	int x, y;
	x = g_Draw.m_nScreenW - 17;
	y = 23;
	bool bAimbot = ( Vars::Aimbot::Global::Active.var ) && ( Vars::Aimbot::Hitscan::Active.var || Vars::Aimbot::Projectile::Active.var );
	bool bDT = ( Vars::Misc::CL_Move::Enabled.var && Vars::Misc::CL_Move::Doubletap.var );

	printStatus( bAimbot, L"Aimbot", x, y );
	printStatus( Vars::Triggerbot::Active.var, L"Triggerbot", x, y );
	printStatus( bDT, L"Doubletap", x, y );
	printStatus( Vars::CritHack::Active.var, L"Crits", x, y );

	y += g_Fonts[ EFonts::HARPOON ].m_nTall;
}

void CIndicators::DrawBT( )
{
	if ( !Vars::Backtrack::Active.var || !Vars::Backtrack::Indicator.var || g_GlobalInfo.m_WeaponType == EWeaponType::PROJECTILE )
	{
		return;
	}

	const auto &pLocal = g_EntityCache.GetLocal( );
	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ENEMIES ) )
	{
		const auto &pEnemy = pEntity->As<CTFPlayer>( );
		if ( !pEnemy || pEnemy->deadflag( ) || pEnemy->IsDormant( ) || !Utils::IsOnScreen( pLocal, pEnemy ) )
			continue;

		const auto &vRecords = g_Backtrack.GetRecords( pEnemy );
		if ( vRecords )
		{
			int i = 0;
			for ( auto &record : *vRecords )
			{
				if ( &record )
				{
					i++;
					if ( Vars::Misc::DisableInterpolation.var )
					{
						if ( g_Backtrack.WithinRewind( record ) )
						{
							Vec3 vScreen;
							Utils::W2S( record.vHead, vScreen );
							g_Draw.BlackOutlinedRect( vScreen.x, vScreen.y, 3, 3, { 255, 255, 255, 255 } );
						}
					}
					else
					{
						if ( i > 2 )
						{
							if ( g_Backtrack.WithinRewind( record ) )
							{
								Vec3 vScreen;
								Utils::W2S( record.vHead, vScreen );
								g_Draw.BlackOutlinedRect( vScreen.x, vScreen.y, 3, 3, { 255, 255, 255, 255 } );
							}
						}
					}
				}
			}
		}
	}
}

bool CIndicators::GetSpectators( CTFPlayer* pLocal )
{
	m_Spectators.clear( );

	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
	{
		const auto &pPlayer = pEntity->As<CTFPlayer>( );
		const auto &pObservedPlayer = GET_ENT_H( pPlayer->m_hObserverTarget( ) )->As<CTFPlayer>( );

		if ( pPlayer && !pPlayer->IsDormant( ) && pPlayer->deadflag( ) && pObservedPlayer && !pObservedPlayer->IsDormant( ) && !pObservedPlayer->deadflag( ) )
		{
			Color_t color = { 255, 255, 255, 255 };
			switch ( pPlayer->m_iObserverMode( ) )
			{
				case OBS_MODE_FIRSTPERSON:
				{
					color = { 255, 50, 50, 255 };
					break;
				}
				case OBS_MODE_THIRDPERSON:
				{
					color = { 255, 255, 255, 255 };
					break;
				}
				default: continue;
			}

			if ( pPlayer->PlayerOnFriendsList( ) )
				color = { 150, 255, 150, 255 };

			player_info_t pi, pi1;
			if ( I::EngineClient->GetPlayerInfo( pPlayer->entindex( ), &pi ) && I::EngineClient->GetPlayerInfo( pObservedPlayer->entindex( ), &pi1 ) )
			{
				m_Spectators.push_back( { Utils::ConvertUtf8ToWide( pi.m_sName ).c_str( ), Utils::ConvertUtf8ToWide( pi1.m_sName ).c_str( ), color } );
			}
		}
	}

	return !m_Spectators.empty( );
}

void CIndicators::DrawSpectators( )
{
	if ( const auto &pLocal = g_EntityCache.GetLocal( ) )
	{
		if ( pLocal->deadflag( ) || !GetSpectators( pLocal ) ) { return; }

		int nTextOffset = 0;
		for ( size_t i = 0; i < m_Spectators.size( ); i++ )
		{
			if ( m_Spectators[ i ].observer.length( ) > 32 )
			{
				m_Spectators[ i ].observer.replace( 32, m_Spectators[ i ].observer.length( ), ( L"..." ) );
			}

			if ( m_Spectators[ i ].observed.length( ) > 32 )
			{
				m_Spectators[ i ].observed.replace( 32, m_Spectators[ i ].observed.length( ), ( L"..." ) );
			}

			int wide, tall;
			std::wstring name = m_Spectators[ i ].observer + L" -> " + m_Spectators[ i ].observed;
			I::Surface->GetTextSize( g_Fonts[ EFonts::NAME ].m_dwFont, name.c_str( ), wide, tall );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, 5, 200 + nTextOffset, m_Spectators[ i ].color, ALIGN_DEFAULT, name.c_str( ) );
			nTextOffset += g_Fonts[ EFonts::NAME ].m_nTall;
		}
	}
}

void CIndicators::DrawAA( )
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	
	int nTextOffset = 0;

	if ( ( Vars::AntiHack::AntiAim::Active.var || Vars::AntiHack::AntiAimBeta::Active.var ) && !pLocal->deadflag( ) )
	{
		static constexpr Color_t realColour = { 0, 255, 0, 255 };
		static constexpr Color_t fakeColour = { 255, 0, 0, 255 };

		const auto &vOrigin = pLocal->GetAbsOrigin( );

		Vec3 vScreen1, vScreen2;
		if ( Utils::W2S( vOrigin, vScreen1 ) && !g_GlobalInfo.m_bAvoidingBackstab && I::Input->CAM_IsThirdPerson( ) )
		{
			constexpr auto distance = 50.f;
			if ( Utils::W2S( Utils::GetRotatedPosition( vOrigin, g_GlobalInfo.m_vRealViewAngles.y, distance ), vScreen2 ) )
			{
				g_Draw.Line( vScreen1.x, vScreen1.y, vScreen2.x, vScreen2.y, realColour );
				g_Draw.OutlinedString( g_Fonts[ EFonts::CONDS ].m_dwFont, vScreen2.x, vScreen2.y - 4, { 255, 255, 255, 255 }, ALIGN_CENTER, L"REAL" );
			}

			if ( Utils::W2S( Utils::GetRotatedPosition( vOrigin, g_GlobalInfo.m_vFakeViewAngles.y, distance ), vScreen2 ) )
			{
				g_Draw.Line( vScreen1.x, vScreen1.y, vScreen2.x, vScreen2.y, fakeColour );
				g_Draw.OutlinedString( g_Fonts[ EFonts::CONDS ].m_dwFont, vScreen2.x, vScreen2.y - 4, { 255, 255, 255, 255 }, ALIGN_CENTER, L"FAKE" );
			}

		}

		/*Color_t fake = { 0, 0, 0, 255 };
		if ( Vars::AntiHack::AntiAim::Pitch.var == 0 )
		{
			fake = { 200, 5, 15, 255 };
		}
		else
		{
			if ( Vars::AntiHack::AntiAim::Pitch.var == 2 )
			{
				if ( !pLocal->IsOnGround( ) )
				{
					if ( pLocal->m_iClass( ) == SCOUT || pLocal->m_iClass( ) == SOLDIER )
						fake = { 128, 200, 0, 255 };
					else
						fake = { 200, 5, 15, 255 };
				}
				else
				{
					fake = { 128, 200, 0, 255 };
				}
			}
			else if ( Vars::AntiHack::AntiAim::Pitch.var == 1 )
			{
				fake = { 128, 200, 0, 255 };
			}
			else
			{
				if ( !pLocal->IsOnGround( ) )
				{
					fake = { 200, 5, 15, 255 };
				}
				else
				{
					fake = { 128, 200, 0, 255 };
				}
			}
		}

		g_Draw.String( g_Fonts[ EFonts::IND ].m_dwFont, 10, g_Draw.m_nScreenH / 2 + g_Draw.m_nScreenH / 4, fake, ALIGN_DEFAULT, L"PITCH" );
		nTextOffset += g_Fonts[ EFonts::IND ].m_nTall;*/

		/*static int y = m_nNextIndicatorHeight;
		I::Surface->DrawSetColor( { 0, 0, 0, 255 } );
		I::Surface->DrawFilledRectFade( 0, y + 2, 126, y + 2 + 51, 222, 0, true );

		I::Surface->DrawSetColor( { Vars::Menu::Colors::CustomTitle.r,
									Vars::Menu::Colors::CustomTitle.g,
									Vars::Menu::Colors::CustomTitle.b,
									255 } );

		I::Surface->DrawFilledRectFade( 0, y + 1, 126, y + 2, 255, 0, true );
		I::Surface->DrawFilledRectFade( 0, y + 1 + 51, 126, y + 2 + 51, 255, 0, true );

		static int realtexture = g_Draw.CreateTextureFromArray( g_RawData.rawData3, 82, 157 );
		static int faketexture = g_Draw.CreateTextureFromArray( g_RawData.rawData4, 82, 138 );
		g_Draw.DrawTextureRect( 40, y + 2 + 3, 24, 45, faketexture, { 45, 45, 45, 255 } );
		//g_Draw.DrawTextureRect( 20, y + 2 + 3, 45, 45, texture, { 90, 90, 90, 255 } );
		g_Draw.DrawTextureRect( 40, y + 2 + 3, 24, 45, realtexture, { 180, 180, 180, 255 } );
		m_nNextIndicatorHeight = y + 2 + 51;*/
	}

	/*if ( Vars::Misc::CL_Move::Enabled.var && Vars::Misc::CL_Move::Doubletap.var )
	{
		Color_t charge = { 0, 0, 0, 255 };

		if ( g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_bRecharging )
		{
			charge = { 200, 5, 15, 255 };
		}
		else if ( g_GlobalInfo.m_bRecharging || g_GlobalInfo.m_nWaitForShift )
		{
			charge = { 200, 128, 0, 255 };
		}
		else
		{
			charge = { 128, 200, 0, 255 };
		}

		g_Draw.String( g_Fonts[ EFonts::IND ].m_dwFont, 10, g_Draw.m_nScreenH / 2 + g_Draw.m_nScreenH / 4 + nTextOffset, charge, ALIGN_DEFAULT, L"DT" );
		nTextOffset += g_Fonts[ EFonts::IND ].m_nTall;
	}*/

	if ( Vars::Misc::CL_Move::Fakelag.var )
	{
		static int y = m_nNextIndicatorHeight;
		I::Surface->DrawSetColor( { 0, 0, 0, 255 } );
		I::Surface->DrawFilledRectFade( 0, y + 2, 126, y + 2 + 51, 222, 0, true );

		I::Surface->DrawSetColor( { Vars::Menu::Colors::CustomTitle.r,
									Vars::Menu::Colors::CustomTitle.g,
									Vars::Menu::Colors::CustomTitle.b,
									255 } );

		I::Surface->DrawFilledRectFade( 0, y + 1, 126, y + 2, 255, 0, true );
		I::Surface->DrawFilledRectFade( 0, y + 1 + 51, 126, y + 2 + 51, 255, 0, true );

		static int texture = g_Draw.CreateTextureFromArray( g_RawData.rawData2, 82, 82 );
		g_Draw.DrawTextureRect( 0, y + 2 + 3, 45, 45, texture, { 45, 45, 45, 255 } );
		g_Draw.DrawTextureRect( 20, y + 2 + 3, 45, 45, texture, { 90, 90, 90, 255 } );
		g_Draw.DrawTextureRect( 40, y + 2 + 3, 45, 45, texture, { 180, 180, 180, 255 } );
		m_nNextIndicatorHeight = y + 2 + 51;
		/*Color_t lag = { 0, 0, 0, 255 };
		if ( !pLocal->IsOnGround( ) && g_GlobalInfo.m_nShifted > 1 )
		{
			lag = { 128, 200, 0, 255 };
		}
		else
		{
			if ( Vars::Misc::CL_Move::FakelagValue.var < 2 || g_GlobalInfo.m_nShifted == 0 )
				lag = { 200, 5, 15, 255 };
			else if ( Vars::Misc::CL_Move::FakelagValue.var > 13 )
				lag = { 128, 200, 0, 255 };
			else
				lag = Utils::GetIndicatorColor( Vars::Misc::CL_Move::FakelagValue.var, Vars::Misc::CL_Move::FakelagSafe.var ? 21 : 28, 15 );
		}
		Color_t choke = Utils::GetIndicatorColor( I::ClientState->chokedcommands, Vars::Misc::CL_Move::FakelagSafe.var ? 21 : 28, 15 );
		g_Draw.String( g_Fonts[ EFonts::IND ].m_dwFont, 10, g_Draw.m_nScreenH / 2 + g_Draw.m_nScreenH / 4 + nTextOffset, lag, ALIGN_DEFAULT, L"FL" );
		nTextOffset += g_Fonts[ EFonts::IND ].m_nTall;
		g_Draw.String( g_Fonts[ EFonts::IND ].m_dwFont, 10, g_Draw.m_nScreenH / 2 + g_Draw.m_nScreenH / 4 + nTextOffset, lag, ALIGN_DEFAULT, L"%i", I::ClientState->chokedcommands );
		nTextOffset += g_Fonts[ EFonts::IND ].m_nTall;*/
	}
}

//cam
/*void CIndicators::DrawDT( )
{
	if ( !Vars::Misc::CL_Move::Indicator.var ) { return; }

	int w = 110;
	int h = 6;
	int x = g_Draw.m_nScreenC - w / 2;
	int y = g_Draw.m_nScreenH / 2 + 100;
	const float ratioCurrent = std::clamp( ( ( float )24 - ( float )g_GlobalInfo.m_nShifted / 24.f ), 0.0f, 1.0f );
	static float ratioInterp = 0.00f; ratioInterp = g_Draw.EaseIn( ratioInterp, ratioCurrent, 0.95f ); Utils::Clamp( ratioInterp, 0.00f, 1.00f );

	if ( g_GlobalInfo.m_nShifted > 19 && iAlpha > 0 )
	{
		if ( iAlpha )
			iAlpha -= 3;
	}

	else if ( g_GlobalInfo.m_nShifted < 20 && iAlpha < 256 )
	{
		if ( iAlpha != 255 )
			iAlpha += 3;
	}

	g_Draw.Rect( x + 1, y + 1, ( w - 2 ), h - 2, { 39, 35, 37, iAlpha } );
	g_Draw.OutlinedRect( x, y, w, h, { 0, 0, 0, iAlpha } );
	g_Draw.GradientRectWH( x + 1, y + 1, ratioInterp * ( w - 2 ), h - 2, { 255, 100, 0, iAlpha }, { 0, 0, 255, iAlpha }, true );

	if ( g_GlobalInfo.m_nWaitForShift && !g_GlobalInfo.m_bRecharging )
	{
		g_Draw.String( g_Fonts[ EFonts::WEAPON ].m_dwFont, g_Draw.m_nScreenC, y - 11, { 255, 255, 255, iAlpha }, ALIGN_CENTERHORIZONTAL, "%.2f", TICKS_TO_TIME( g_GlobalInfo.m_nWaitForShift ) );
	}
}*/

//deadflag
/*void CIndicators::DrawDT( )
{
	if ( !Vars::Misc::CL_Move::Indicator.var ) { return; }

	const auto &pLocal = GET_ENT_I( LOCAL_IDX )->As<CTFPlayer>( );
	if ( !pLocal || pLocal->deadflag( ) ) { return; }

	const auto &pWeapon = GET_ENT_H( pLocal->m_hActiveWeapon( ) )->As<CTFWeaponBase>( );
	if ( !pWeapon ) { return; }

	int w = 80;
	int h = 3;
	int offset = 100;//30;
	int x = g_Draw.m_nScreenC - w / 2;
	int y = g_Draw.m_nScreenH / 2 + offset;

	const float ratio = std::clamp( ( static_cast<float>( 24 - g_GlobalInfo.m_nShifted ) / 24.f ), 0.0f, 1.0f );

	g_Draw.GradientRectWH( x - 1, y - 1, w + 2, h + 2, { 35, 35, 35, 255 }, { 20, 20, 20, 255 }, false );
	g_Draw.OutlinedRect( x - 1, y - 1, w + 2, h + 2, { 20, 20, 20, 255 } );
	g_Draw.GradientRectWH( x, y, w * ratio, h, { 255, 255, 255, 255 }, Vars::Menu::Colors::CustomTitle, true );

	if ( g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_bRecharging )
	{
		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 191, 70, 70, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(RapidFire) Too Expensive %i < %i", 24 - g_GlobalInfo.m_nShifted, g_GlobalInfo.m_nTicksToShift );

		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall * 2, { 191, 70, 70, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(Teleport) Too Expensive %i < %i", 24 - g_GlobalInfo.m_nShifted, 24 );
	}
	else if ( g_GlobalInfo.m_bRecharging )
	{
		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 191, 121, 50, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(Recharging) %i/%i", 24 - g_GlobalInfo.m_nShifted, 24 );
	}
	else if ( g_GlobalInfo.m_nWaitForShift )
	{
		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 191, 121, 50, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(RapidFire) Wait %i/%i", DT_WAIT_CALLS - g_GlobalInfo.m_nWaitForShift, DT_WAIT_CALLS );

		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall * 2, { 60, 160, 110, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(Teleport) Ready" );
	}
	else
	{
		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall, { 60, 160, 110, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(RapidFire) Ready" );

		g_Draw.String( g_Fonts[ EFonts::DEBUG ].m_dwFont, g_Draw.m_nScreenC, g_Draw.m_nScreenH / 2 + offset + g_Fonts[ EFonts::DEBUG ].m_nTall * 2, { 60, 160, 110, 255 }, ALIGN_CENTERHORIZONTAL,
					   "(Teleport) Ready" );
	}

}*/