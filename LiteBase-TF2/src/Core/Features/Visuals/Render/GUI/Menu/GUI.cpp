#include "GUI.h"

#include "ConfigManager/ConfigManager.h"
#include "../../../AttributeChanger/AttributeChanger.h"
#include "../../../../Misc/Misc.h"

void CMenu::Separator( )
{
	int x = m_LastWidget.x + m_LastWidget.w + Vars::Menu::SpacingX;
	int y = Vars::Menu::Position.y;
	int w = 0;
	int h = 0;

	g_Draw.Line( x, y, x, ( y + Vars::Menu::Position.h - 1 ), Vars::Menu::Colors::OutlineMenu );

	m_LastWidget.x = x + Vars::Menu::SpacingX;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;
}

void CMenu::Separator( Color_t color )
{
	int x = m_LastWidget.x + m_LastWidget.w + Vars::Menu::SpacingX;
	int y = Vars::Menu::Position.y;
	int w = 0;
	int h = 0;

	g_Draw.Line( x - 2, y, x - 2, ( y + Vars::Menu::Position.h - 1 ), Vars::Menu::Colors::Outline );
	g_Draw.Line( x - 1, y - 2, x - 1, ( y + Vars::Menu::Position.h ), color );
	g_Draw.Line( x, y - 2, x, ( y + Vars::Menu::Position.h ), color );
	g_Draw.Line( x + 1, y - 2, x + 1, ( y + Vars::Menu::Position.h ), color );
	g_Draw.Line( x + 2, y - 2, x + 2, ( y + Vars::Menu::Position.h ), color );
	g_Draw.Line( x + 3, y, x + 3, ( y + Vars::Menu::Position.h - 1 ), Vars::Menu::Colors::Outline );

	m_LastWidget.x = x + Vars::Menu::SpacingX;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;
}

bool CMenu::CheckBox( CVar<bool> &Var )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::CheckBoxW;
	int h = Vars::Menu::CheckBoxH;
	/*if (Vars::Menu::LightMode)
		g_Draw.Rect(x, y, w, h, Vars::Menu::Colors::Outline);*/

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
			I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
			callback = true;
			Var.var = !Var.var;
		}

		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	if ( Var.var )
	{
		g_Draw.Rect( x + 2, y + 2, w - 4, h - 4, Vars::Menu::Colors::TitleBar );
	}

	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + Vars::Menu::SpacingText, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::Button( const wchar_t *Label, int WidthOverride, int HeightOverride, bool Active )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = WidthOverride ? WidthOverride : Vars::Menu::ButtonW;
	int h = HeightOverride ? HeightOverride : Vars::Menu::ButtonH;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( g_InputHelper.IsPressed( VK_LBUTTON ) )
		{
			if ( !Active )
			{
				I::Surface->PlaySound( "UI/buttonclick.wav" );
			}
			callback = true;
		}
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	if ( Active )
	{
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::TitleBar );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, Label );
	}

	if ( !Active )
	{
		I::Surface->DrawSetColor( { 255, 255, 255, 8 } );
		I::Surface->DrawFilledRectFade( x, y, x + w, y + ( h * 0.9f ), 255, 0, false );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, Label );
	}

	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	if ( callback )
		m_bReopened = true;

	return callback;
}

bool CMenu::ButtonXY( const wchar_t *Label, bool Active, int XOverride, int YOverride, int WidthOverride, int HeightOverride )
{
	bool callback = false;

	int x = XOverride ? XOverride : m_LastWidget.x;
	int y = YOverride ? YOverride : m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = WidthOverride ? WidthOverride : Vars::Menu::ButtonW;
	int h = HeightOverride ? HeightOverride : Vars::Menu::ButtonH;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( g_InputHelper.IsPressed( VK_LBUTTON ) )
		{
			if ( !Active )
				I::Surface->PlaySound( "UI/buttonclick.wav" );
			callback = true;
		}

		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	if ( Active )
	{
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::TitleBar );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ) + 1, Vars::Menu::Colors::Text, ALIGN_CENTER, Label );
	}

	if ( !Active )
	{
		I::Surface->DrawSetColor( { 255, 255, 255, 8 } );
		I::Surface->DrawFilledRectFade( x, y, x + w, y + ( h * 0.9f ), 255, 0, false );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ) + 1, Vars::Menu::Colors::Text, ALIGN_CENTER, Label );
	}

	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	if ( callback )
		m_bReopened = true;

	return callback;
}

bool CMenu::ComboBox( CVar<int> &Var, const std::vector<CVar<int>> &List )
{
	auto FindCurItemName = [ & ] ( ) -> const wchar_t *
	{
		for ( const auto &Item : List )
		{
			if ( Item.var == Var.var )
				return Item.szName;
		}

		return ( L"UNKNOWN_ITEM" );
	};

	auto FindCurItemIndex = [ & ] ( ) -> int
	{
		for ( size_t n = 0; n < List.size( ); n++ )
		{
			if ( List[ n ].var == Var.var )
				return n;
		}

		return 0;
	};

	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::ComboBoxW;
	int h = Vars::Menu::ComboBoxH;

	static std::map<CVar<int> *, int> indexes = {};
	static std::map<CVar<int> *, bool> inits = {};

	if ( !inits[ &Var ] || m_bReopened ) {
		indexes[ &Var ] = FindCurItemIndex( );
		inits[ &Var ] = true;
	}

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( indexes[ &Var ] > 0 )
		{
			if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
				I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
				Var.var = List[ --indexes[ &Var ] ].var;
				callback = true;
			}

			g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
		}
	}

	else if ( g_InputHelper.m_nMouseX > x + ( w / 2 ) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( indexes[ &Var ] < int( List.size( ) - 1 ) )
		{
			if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
				I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
				Var.var = List[ ++indexes[ &Var ] ].var;
				callback = true;
			}

			g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
		}
	}

	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, FindCurItemName( ) );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + Vars::Menu::SpacingText, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::PriorityCombo( int _x, int &Var, const std::vector<CVar<int>> &List )
{
	auto FindCurItemName = [ & ] ( ) -> const wchar_t *
	{
		for ( const auto &Item : List )
		{
			if ( Item.var == Var )
				return Item.szName;
		}

		return ( L"UNKNOWN_ITEM" );
	};

	auto FindCurItemIndex = [ & ] ( ) -> int
	{
		for ( size_t n = 0; n < List.size( ); n++ )
		{
			if ( List[ n ].var == Var )
				return n;
		}

		return 0;
	};

	bool callback = false;

	int x = _x - 1;
	int y = m_LastWidgetPL.y - 1;
	int w = Vars::Menu::ComboBoxW;
	int h = Vars::Menu::ComboBoxH + 1;

	static std::map<int *, int> indexes = {};
	static std::map<int *, bool> inits = {};

	if ( !inits[ &Var ] || m_bReopened ) {
		indexes[ &Var ] = FindCurItemIndex( );
		inits[ &Var ] = true;
	}

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( playerlist.m_nMouseX > x && playerlist.m_nMouseX < x + ( w / 2 ) && playerlist.m_nMouseY > y && playerlist.m_nMouseY < y + h )
	{
		if ( indexes[ &Var ] > 0 )
		{
			if ( playerlist.IsPressed( VK_LBUTTON ) ) {
				I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
				Var = List[ --indexes[ &Var ] ].var;
				callback = true;
			}

			//g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
		}
	}

	else if ( playerlist.m_nMouseX > x + ( w / 2 ) && playerlist.m_nMouseX < x + w && playerlist.m_nMouseY > y && playerlist.m_nMouseY < y + h )
	{
		if ( indexes[ &Var ] < int( List.size( ) - 1 ) )
		{
			if ( playerlist.IsPressed( VK_LBUTTON ) ) {
				I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
				Var = List[ ++indexes[ &Var ] ].var;
				callback = true;
			}

			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
		}
	}

	g_Draw.OutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, FindCurItemName( ) );
	//g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + Vars::Menu::SpacingText, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	//m_LastWidgetPL.x = x;
	m_LastWidgetPL.y = y;
	m_LastWidgetPL.w = w;
	m_LastWidgetPL.h = h;

	return callback;
}

bool CMenu::InputFloat( CVar<float> &Var, float Min, float Max, float Step, const wchar_t *Fmt )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( Var.var != Min )
	{
		if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
		{
			if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
				Var.var -= Step;
				Var.var = std::clamp( Var.var, Min, Max );
				callback = true;
				g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
			}
			else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
			{
				g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
			}
			else
			{
				g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
			}
		}
	}

	if ( Var.var != Max )
	{
		if ( g_InputHelper.m_nMouseX > x + ( w / 2 ) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
		{
			if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
				Var.var += Step;
				Var.var = std::clamp( Var.var, Min, Max );
				callback = true;
				g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
			}
			else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
			{
				g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
			}
			else
			{
				g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
			}
		}
	}

	g_Draw.OutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, Fmt, Var.var );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + Vars::Menu::SpacingText, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::SliderFloat( CVar<float> &Var, float Min, float Max, const wchar_t *Fmt )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY + Vars::Menu::SpacingText * 3;
	int w = Vars::Menu::InputBoxW * 2 - 5;
	int h = 7;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	const float size = Max - Min;
	const float ratio = ( w - 1 ) / size;
	float sliderx = x + ( Var.var - Min ) * ratio;
	if ( g_InputHelper.m_nMouseX >= x && g_InputHelper.m_nMouseX <= x + w - 1 && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
			I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
		else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w - 1 );
			Var.var = Min + ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )( w - 1 ) ) * ( Max - Min ) );
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
		else if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w - 1);
			Var.var = Min + ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )( w - 1 ) ) * ( Max - Min ) );
			callback = true;
			//sliderx = x + ( Var.var - Min ) * ratio;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
		}
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	/*if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY <= y )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w );
			Var.var = ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )w ) * ( Max - Min ) );
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY >= y + h )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w );
			Var.var = ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )w ) * ( Max - Min ) );
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}

	if ( g_InputHelper.m_nMouseX >= x + w )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = x + w;
			Var.var = Max;
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}

	if ( g_InputHelper.m_nMouseX <= x )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = x;
			Var.var = Min;
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}*/

	/*
	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.Line( sliderx, y, sliderx, y + h - 1, Vars::Menu::Colors::TitleBar );
	g_Draw.Line( sliderx - 1, y, sliderx - 1, y + h - 1, Vars::Menu::Colors::TitleBar );
	g_Draw.Line( sliderx + 1, y, sliderx + 1, y + h - 1, Vars::Menu::Colors::TitleBar );
	g_Draw.BlackOutlinedRect( sliderx - 2, y, 5, h, { 255, 255, 255, 100 } );
	*/

	const float rectratio = ( float )( Var.var - Min ) / ( Max - Min );
	g_Draw.Rect( x, y, w * rectratio, h, Vars::Menu::Colors::CustomTitle );
	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w, y - ( Vars::Menu::SpacingText * 3.f + 1 ), Vars::Menu::Colors::Text, ALIGN_REVERSE, Fmt, Var.var );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x, y - Vars::Menu::SpacingText * 1.5f, Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::InputInt( CVar<int> &Var, int Min, int Max, int Step )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( Var.var != Min )
	{
		if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
		{
			if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
				Var.var -= Step;
				Var.var = std::clamp( Var.var, Min, Max );
				callback = true;
				g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
			}
			else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
			{
				g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
			}
			else
			{
				g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
			}
		}
	}

	if ( Var.var != Max )
	{
		if ( g_InputHelper.m_nMouseX > x + ( w / 2 ) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
		{
			if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
				Var.var += Step;
				Var.var = std::clamp( Var.var, Min, Max );
				callback = true;
				g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
			}
			else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
			{
				g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
			}
			else
			{
				g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
			}
		}
	}

	g_Draw.OutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%d", Var );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + Vars::Menu::SpacingText, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::SliderInt( CVar<int> &Var, int Min, int Max )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY + Vars::Menu::SpacingText * 3;
	int w = Vars::Menu::InputBoxW * 2 - 5;
	int h = 7;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	const float size = Max - Min;
	const float ratio = ( w - 1 ) / size;
	float sliderx = x + ( Var.var - Min ) * ratio;
	if ( g_InputHelper.m_nMouseX >= x && g_InputHelper.m_nMouseX <= x + w - 1 && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
			I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
		else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w - 1 );
			Var.var = Min + ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )( w - 1 ) ) * ( ( Max - Min ) ) );
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
		else if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w - 1 );
			Var.var = Min + ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )( w - 1 ) ) * ( ( Max - Min ) ) );
			callback = true;
			//sliderx = x + ( Var.var - Min ) * ratio;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
		}
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	/*if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY <= y )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w );
			Var.var = ( int )( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )w ) * ( Max - Min ) );
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY >= y + h )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = std::clamp( g_InputHelper.m_nMouseX, x, x + w );
			Var.var = ( int )( ( ( ( float )g_InputHelper.m_nMouseX - ( float )x ) / ( float )w ) * ( Max - Min ) );
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}

	if ( g_InputHelper.m_nMouseX >= x + w )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = x + w;
			Var.var = Max;
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}

	if ( g_InputHelper.m_nMouseX <= x )
	{
		if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
		{
			sliderx = x;
			Var.var = Min;
			callback = true;
			//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
		}
	}*/
	
	/*
	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.Line( sliderx, y, sliderx, y + h - 1, Vars::Menu::Colors::TitleBar );
	g_Draw.Line( sliderx - 1, y, sliderx - 1, y + h - 1, Vars::Menu::Colors::TitleBar );
	g_Draw.Line( sliderx + 1, y, sliderx + 1, y + h - 1, Vars::Menu::Colors::TitleBar );
	g_Draw.BlackOutlinedRect( sliderx - 2, y, 5, h, { 255, 255, 255, 100 } );
	*/

	const float rectratio = ( float )( Var.var - Min ) / ( Max - Min );
	g_Draw.Rect( x, y, w * rectratio, h, Vars::Menu::Colors::CustomTitle );
	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w, y - ( Vars::Menu::SpacingText * 3.f + 1 ), Vars::Menu::Colors::Text, ALIGN_REVERSE, "%i", Var.var );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x, y - Vars::Menu::SpacingText * 1.5f, Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Var.szName );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::InputColor( Color_t &Var, const wchar_t *Label )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputColorBoxW;
	int h = Vars::Menu::InputBoxH;
	int _x = x;

	if ( wcslen( Label ) != 0 )
		y += g_Fonts[ EFonts::MENU ].m_nTall;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	int oldspacing = Vars::Menu::SpacingX;
	Vars::Menu::SpacingX /= 2;

	//red
	{
		if ( Var.r != 0 )
		{
			if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.r -= 1;
					Var.r = std::clamp( Var.r, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else
				{
					g_Draw.GradientRect( x, y, x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
				}
			}
		}

		if ( Var.r != 255 )
		{
			if ( g_InputHelper.m_nMouseX > x + ( w / 2 ) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.r += 1;
					Var.r = std::clamp( Var.r, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else
				{
					g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
		}

		g_Draw.OutlinedRect( _x, y, w, h, Vars::Menu::Colors::OutlineMenu );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, _x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%d", Var.r );
		_x += w + Vars::Menu::SpacingX;
	}

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( _x, y, w, h, Vars::Menu::Colors::Outline );

	//green
	{
		if ( Var.g != 0 )
		{
			if ( g_InputHelper.m_nMouseX > _x && g_InputHelper.m_nMouseX < _x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.g -= 1;
					Var.g = std::clamp( Var.g, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else
				{
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
				}
			}
		}

		if ( Var.g != 255 )
		{
			if ( g_InputHelper.m_nMouseX > _x + ( w / 2 ) && g_InputHelper.m_nMouseX < _x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.g += 1;
					Var.g = std::clamp( Var.g, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else
				{
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
		}

		g_Draw.OutlinedRect( _x, y, w, h, Vars::Menu::Colors::OutlineMenu );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, _x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%d", Var.g );
		_x += w + Vars::Menu::SpacingX;
	}

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( _x, y, w, h, Vars::Menu::Colors::Outline );

	//blue
	{
		if ( Var.b != 0 )
		{
			if ( g_InputHelper.m_nMouseX > _x && g_InputHelper.m_nMouseX < _x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.b -= 1;
					Var.b = std::clamp( Var.b, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else
				{
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
				}
			}
		}

		if ( Var.b != 255 )
		{
			if ( g_InputHelper.m_nMouseX > _x + ( w / 2 ) && g_InputHelper.m_nMouseX < _x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.b += 1;
					Var.b = std::clamp( Var.b, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else
				{
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
		}

		g_Draw.OutlinedRect( _x, y, w, h, Vars::Menu::Colors::OutlineMenu );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, _x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%d", Var.b );
		_x += w + Vars::Menu::SpacingX;
	}

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( _x, y, w, h, Vars::Menu::Colors::Outline );

	//alpha
	{
		if ( Var.a != 0 )
		{
			if ( g_InputHelper.m_nMouseX > _x && g_InputHelper.m_nMouseX < _x + ( w / 2 ) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.a -= 1;
					Var.a = std::clamp( Var.a, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
				}
				else
				{
					g_Draw.GradientRect( _x, y, _x + ( w / 2 ), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true );
				}
			}
		}

		if ( Var.a != 255 )
		{
			if ( g_InputHelper.m_nMouseX > _x + ( w / 2 ) && g_InputHelper.m_nMouseX < _x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			{
				if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) ) {
					Var.a += 1;
					Var.a = std::clamp( Var.a, static_cast< byte >( 0 ), static_cast< byte >( 255 ) );
					callback = true;
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else
				{
					g_Draw.GradientRect( _x + ( w / 2 ), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
		}

		g_Draw.OutlinedRect( _x, y, w, h, Vars::Menu::Colors::OutlineMenu );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, _x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%d", Var.a );
		_x += w + Vars::Menu::SpacingX;
	}

	//indicator
	{
		g_Draw.Rect( _x, y, w / 2, h, { Var.r, Var.g, Var.b, 255 } );
		g_Draw.OutlinedRect( _x, y, w / 2, h, Vars::Menu::Colors::OutlineMenu );
	}

	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x, y + ( h / 2.f ) - g_Fonts[ EFonts::MENU ].m_nTall * 1.2f, Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Label );

	Vars::Menu::SpacingX = oldspacing;

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::ColorPicker( Color_t &Var, const wchar_t *Label )
{
	bool callback = false;
	static bool bOpen = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;
	const float size = 255.0f;
	static float colHSV[ 3 ];
	static Color_t square = Var;
	float hueslidery = 0.f;
	float alphasliderx = 0.f;
	Vec2 picker = { };
	static float time = I::EngineClient->Time( );
	float elapsed = I::EngineClient->Time( ) - time;
	static Color_t *curr = nullptr, *prevv = curr;
	static Color_t preview = curr == nullptr ? ( prevv == nullptr ? Var : *prevv ) : *curr;

	if ( curr != prevv )
	{
		time = I::EngineClient->Time( );
		prevv = curr;
	}

	if ( wcslen( Label ) != 0 )
		y += g_Fonts[ EFonts::MENU ].m_nTall;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
	{
		if ( curr == nullptr && elapsed > 0.1f && g_InputHelper.IsPressed( VK_LBUTTON ) )
		{
			I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
			curr = &Var;
			Utils::CLR2HSV( *curr, colHSV );
			preview.a = ( *curr ).a;
			//callback = true;
		}
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	g_Draw.BlackOutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, L"Pick" );
	//g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + 4, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Label );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x, y + ( h / 2.f ) - g_Fonts[ EFonts::MENU ].m_nTall * 1.2f, Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, Label );

	//default texture
	{
		for ( int i = 0; i <= 2; i++ )
		{
			g_Draw.Rect( x + w + 5 + ( i * 8 ) + 2, y + 2, 4, 4, { 255, 255, 255, 255 } );
			g_Draw.Rect( x + w + 5 + ( i * 8 ) + 2, y + 4 + 2, 4, 4, { 180, 180, 180, 255 } );
			g_Draw.Rect( x + w + 5 + ( i * 8 ) + 2, y + 8 + 2, 4, 4, { 255, 255, 255, 255 } );
			//g_Draw.Rect( x + w + 5 + ( i * 8 ), y + 12 - 1, 4, 4, { 180, 180, 180, 255 } );
		}
		for ( int i = 0; i <= 2; i++ )
		{
			g_Draw.Rect( x + w + 5 + ( i * 8 ) + 6, y + 2, 4, 4, { 180, 180, 180, 255 } );
			g_Draw.Rect( x + w + 5 + ( i * 8 ) + 6, y + 4 + 2, 4, 4, { 255, 255, 255, 255 } );
			g_Draw.Rect( x + w + 5 + ( i * 8 ) + 6, y + 8 + 2, 4, 4, { 180, 180, 180, 255 } );
			//g_Draw.Rect( x + w + 5 + ( i * 8 ) + 4, y + 12 - 1, 4, 4, { 255, 255, 255, 255 } );
		}
	}

	//var rect
	{
		g_Draw.Rect( x + w + 5, y, Vars::Menu::CheckBoxW * 2, h, Var );
		g_Draw.BlackOutlinedRect( x + w + 5, y, Vars::Menu::CheckBoxW * 2, h, Vars::Menu::Colors::OutlineMenu );
	}

	if ( curr == &Var )
	{
		int _w = 239;
		int _h = 158;
		int _x = ( g_Draw.m_nScreenW / 2 ) - ( _w / 2 );
		int _y = ( g_Draw.m_nScreenH / 2 ) - ( _h / 2 );
		g_Draw.Rect( _x, _y, _w - 30, _h, Vars::Menu::Colors::WindowBackground );
		g_Draw.OutlinedRect( _x - 1, _y - 1, _w - 28, _h + 2, { 0, 0, 0, 255 } );

		if ( ButtonXY( L"X", false, _x + _w - ( Vars::Menu::ButtonWSmall * 0.3f ) - 30, _y + 1, Vars::Menu::ButtonWSmall * 0.3f, Vars::Menu::ButtonWSmall * 0.2f )/*Utils::IsGameWindowInFocus( ) && GetAsyncKeyState( VK_ESCAPE ) & 0x8000*/ )
		{
			curr = nullptr;
			//callback = false;
		}

		for ( int i = 0; i <= 3; i++ )
		{
			g_Draw.OutlinedRect( _x - 1 - i, _y - 1 - i, _w - 27 + ( i * 2 ), _h + 3 + ( i * 2 ), Vars::Menu::Colors::CustomTitle );
			if ( i == 3 )
			{
				g_Draw.OutlinedRect( _x - 1 - ( i + 1 ), _y - 1 - ( i + 1 ), _w - 27 + ( ( i * 2 ) + 2 ), _h + 3 + ( ( i * 2 ) + 2 ), { 0, 0, 0, 255 } );
			}
		}

		//g_Draw.GradientRectWH( _x + 10, _y + 10, 120, 120, { 0, 0, 0, 0 }, Var, true );
		//g_Draw.GradientRectWH( _x + 10, _y + 10, 120, 120, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, true );
		//g_Draw.GradientRectWH( _x + 10, _y + 10, 120, 120, { 0, 0, 0, 0 }, { 0, 0, 0, 255 }, false );

		//color square
		{
			I::Surface->DrawSetColor( { 255, 255, 255, 255 } );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 10, _x + 120, _y + 130, 255, 0, true );
			I::Surface->DrawSetColor( { square.r, square.g, square.b, 255 } );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 10, _x + 130, _y + 130, 0, 255, true );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 10, _x + 130, _y + 130, 0, 128, true );
			I::Surface->DrawSetColor( { 0, 0, 0, 255 } );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 10, _x + 130, _y + 130, 0, 255, false );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 10, _x + 130, _y + 130, 0, 255, false );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 10, _x + 130, _y + 130, 0, 128, false );

			int width = 119;
			int height = 119;
			const float ratiox = width / 100.f;
			const float ratioy = height / 100.f;
			picker.x = colHSV[ 1 ] * ratiox;
			picker.y = 120.f - ( colHSV[ 2 ] * ratioy );
			g_Draw.BlackOutlinedRect( _x + 10 + picker.x, _y + 10 + picker.y, 2, 2, { 255, 255, 255, 100 } );

			if ( g_InputHelper.m_nMouseX > _x + 10 && g_InputHelper.m_nMouseX < _x + 130 && g_InputHelper.m_nMouseY > _y + 10 && g_InputHelper.m_nMouseY < _y + 130 )
			{
				if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
					I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					picker.x = std::clamp( g_InputHelper.m_nMouseX, ( _x + 11 ), ( _x + 130 ) );
					picker.y = std::clamp( g_InputHelper.m_nMouseY, ( _y + 11 ), ( _y + 130 ) );
					colHSV[ 1 ] = ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )( _x + 11 ) ) / ( ( float )( 118 ) ) ) * ( 100.f ) );
					colHSV[ 2 ] = 100.f - ( ( ( ( float )g_InputHelper.m_nMouseY - ( float )( _y + 11 ) ) / ( ( float )( 118 ) ) ) * ( 100.f ) );
					//callback = true;
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) )
				{
					picker.x = std::clamp( g_InputHelper.m_nMouseX, ( _x + 11 ), ( _x + 130 ) );
					picker.y = std::clamp( g_InputHelper.m_nMouseY, ( _y + 11 ), ( _y + 130 ) );
					colHSV[ 1 ] = ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )( _x + 11 ) ) / ( ( float )( 118 ) ) ) * ( 100.f ) );
					colHSV[ 2 ] = 100.f - ( ( ( ( float )g_InputHelper.m_nMouseY - ( float )( _y + 11 ) ) / ( ( float )( 118 ) ) ) * ( 100.f ) );
					//callback = true;
					//sliderx = x + ( Var.var - Min ) * ratio;
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
		}

		//hue bar
		{
			static Color_t hueColors[ 7 ] =
			{
				{ 255, 0, 0, 255 },
				{ 255, 200, 0, 255 },
				{ 0, 255, 0, 255 },
				{ 0, 255, 255, 255 },
				{ 0, 0, 255, 255 },
				{ 255, 0, 255, 255 },
				{ 255, 0, 0, 255 }
			};

			for ( auto i = 0; i < 6; i++ )
			{
				g_Draw.GradientRectWH( _x + _w - 99, ( _y + 10 ) + i * ( 20.f ), 10, 20, hueColors[ i ], hueColors[ i + 1 ], false );
			}

			int height = 119;
			const float ratio = height / 359.f;
			hueslidery = colHSV[ 0 ] * ratio;
			g_Draw.BlackOutlinedRect( _x + _w - 101, _y + 8 + hueslidery, 14, 5, { 255, 255, 255, 100 } );
			if ( g_InputHelper.m_nMouseX > _x + _w - 99 && g_InputHelper.m_nMouseX < _x + _w - 89 && g_InputHelper.m_nMouseY > _y + 10 && g_InputHelper.m_nMouseY < _y + 130 )
			{
				if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
					I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					hueslidery = std::clamp( g_InputHelper.m_nMouseY, ( _y + 11 ), ( _y + 130 ) );
					colHSV[ 0 ] = ( ( ( ( float )g_InputHelper.m_nMouseY - ( float )( _y + 11 ) ) / ( ( float )( 118 ) ) ) * ( 359.f ) );
					//callback = true;
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) )
				{
					hueslidery = std::clamp( g_InputHelper.m_nMouseY, ( _y + 11 ), ( _y + 130 ) );
					colHSV[ 0 ] = ( ( ( ( float )g_InputHelper.m_nMouseY - ( float )( _y + 11 ) ) / ( ( float )( 118 ) ) ) * ( 359.f ) );
					//callback = true;
					//sliderx = x + ( Var.var - Min ) * ratio;
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
			preview.r = Utils::HSVTOCLR( colHSV[ 0 ], colHSV[ 1 ], colHSV[ 2 ] ).at( 0 );
			preview.g = Utils::HSVTOCLR( colHSV[ 0 ], colHSV[ 1 ], colHSV[ 2 ] ).at( 1 );
			preview.b = Utils::HSVTOCLR( colHSV[ 0 ], colHSV[ 1 ], colHSV[ 2 ] ).at( 2 );

			square.r = Utils::HSVTOCLR( colHSV[ 0 ], 100, 100 ).at( 0 );
			square.g = Utils::HSVTOCLR( colHSV[ 0 ], 100, 100 ).at( 1 );
			square.b = Utils::HSVTOCLR( colHSV[ 0 ], 100, 100 ).at( 2 );
		}

		//alpha bar
		{
			I::Surface->DrawSetColor( { 255, 255, 255, 255 } );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 140, _x + 130, _y + 150, 0, 255, true );
			I::Surface->DrawSetColor( { 0, 0, 0, 255 } );
			I::Surface->DrawFilledRectFade( _x + 10, _y + 140, _x + 130, _y + 150, 255, 0, true );
			int width = 119;
			const float ratio = width / size;
			alphasliderx = ( ( float )preview.a ) * ratio;
			g_Draw.BlackOutlinedRect( _x + 8 + alphasliderx, _y + 138, 5, 14, { 255, 255, 255, 100 } );
			if ( g_InputHelper.m_nMouseX > _x + 10 && g_InputHelper.m_nMouseX < _x + 131 && g_InputHelper.m_nMouseY > _y + 140 && g_InputHelper.m_nMouseY < _y + 150 )
			{
				if ( g_InputHelper.IsPressed( VK_LBUTTON ) ) {
					I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsHeld( VK_LBUTTON ) )
				{
					alphasliderx = std::clamp( g_InputHelper.m_nMouseX, ( _x + 11 ), ( _x + 131 ) );
					preview.a = ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )( _x + 11 ) ) / ( ( float )( 119 ) ) ) * ( 255.f ) );
					//callback = true;
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );
				}
				else if ( g_InputHelper.IsPressedAndHeld( VK_LBUTTON ) )
				{
					alphasliderx = std::clamp( g_InputHelper.m_nMouseX, ( _x + 11 ), ( _x + 131 ) );
					preview.a = ( ( ( ( float )g_InputHelper.m_nMouseX - ( float )( _x + 11 ) ) / ( ( float )( 119 ) ) ) * ( 255.f ) );
					//callback = true;
					//sliderx = x + ( Var.var - Min ) * ratio;
					//g_Draw.GradientRect( x + ( w / 2 ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true );
				}
			}
		}

		//default texture
		{
			int offsetx = 77;
			int offsety = 12 + 11;

			for ( int i = 0; i <= 4; i++ )
			{
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ), _y + offsety, 4, 4, { 255, 255, 255, 255 } );
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ), _y + offsety + 4, 4, 4, { 180, 180, 180, 255 } );
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ), _y + offsety + 8, 4, 4, { 255, 255, 255, 255 } );
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ), _y + offsety + 12, 4, 4, { 180, 180, 180, 255 } );
			}
			for ( int i = 0; i <= 3; i++ )
			{
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ) + 4, _y + offsety, 4, 4, { 180, 180, 180, 255 } );
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ) + 4, _y + offsety + 4, 4, 4, { 255, 255, 255, 255 } );
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ) + 4, _y + offsety + 8, 4, 4, { 180, 180, 180, 255 } );
				g_Draw.Rect( _x + _w - offsetx + ( i * 8 ) + 4, _y + offsety + 12, 4, 4, { 255, 255, 255, 255 } );
			}
		}

		//preview color
		{
			g_Draw.Rect( _x + _w - 78, _y + 11 + 11, 38, 18, preview );
			g_Draw.BlackOutlinedRect( _x + _w - 79, _y + 10 + 11, 40, 20, Vars::Menu::Colors::OutlineMenu );
		}

		//close button
		if ( ButtonXY( L"APPLY", false, _x + _w - 79, _y + 35 + 11, 40, 15 ) )
		{
			//Var = Utils::HSVTOCLR( colHSV[ 0 ], colHSV[ 1 ], colHSV[ 2 ] );
			//Var = preview;
			Var = preview;
			//square = preview;
			curr = nullptr;
			//callback = false;
		}
	}

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::InputString( const wchar_t *Label, std::wstring &output )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = m_LastWidget.w;
	int h = m_LastWidget.h;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	static bool active = false;
	static std::wstring active_str = {};

	if ( ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h ) && !active )
	{
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	if ( g_InputHelper.IsPressed( VK_LBUTTON ) )
	{
		if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			active = !active;

		else active = false;
	}

	if ( active )
	{
		m_bTyping = true;

		if ( g_InputHelper.IsPressed( VK_INSERT ) ) {
			active = false;
			return false;
		}

		if ( active_str.length( ) < 21 )
		{
			if ( g_InputHelper.IsPressed( VK_SPACE ) )
				active_str += char( VK_SPACE );

			for ( int16_t key = L'A'; key < L'Z' + 1; key++ )
			{
				if ( g_InputHelper.IsPressed( key ) )
					active_str += wchar_t( key );
			}
		}

		if ( g_InputHelper.IsPressedAndHeld( VK_BACK ) && !active_str.empty( ) )
			active_str.erase( active_str.end( ) - 1 );

		if ( g_InputHelper.IsPressed( VK_RETURN ) )
		{
			active = false;

			if ( !active_str.empty( ) )
			{
				output = active_str;
				callback = true;
			}
		}
		g_Draw.GradientRect( x, y, x + ( w / 8 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
		g_Draw.GradientRect( x + ( w - ( w / 8 ) ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );

		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%ws", active_str.empty( ) ? ( L"Enter a Name" ) : active_str.c_str( ) );
	}

	else
	{
		active_str = {};
		I::Surface->DrawSetColor( { 255, 255, 255, 8 } );
		I::Surface->DrawFilledRectFade( x, y, x + w, y + ( h * 0.9f ), 255, 0, false );
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%ws", Label );
	}

	g_Draw.OutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::InputStringC( const wchar_t *Label, std::string &output )
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = m_LastWidget.w;
	int h = m_LastWidget.h;

	static bool active = false;
	static std::string active_str = {};

	if ( ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h ) && !active )
	{
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	if ( g_InputHelper.IsPressed( VK_LBUTTON ) )
	{
		if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h )
			active = !active;

		else active = false;
	}

	if ( active )
	{
		m_bTyping = true;

		if ( g_InputHelper.IsPressed( VK_INSERT ) ) {
			active = false;
			return false;
		}

		if ( active_str.length( ) < 21 )
		{
			if ( g_InputHelper.IsPressed( VK_SPACE ) )
				active_str += char( VK_SPACE );

			for ( int16_t key = L'A'; key < L'Z' + 1; key++ )
			{
				if ( g_InputHelper.IsPressed( key ) )
					active_str += char( key );
			}
		}

		if ( g_InputHelper.IsPressedAndHeld( VK_BACK ) && !active_str.empty( ) )
			active_str.erase( active_str.end( ) - 1 );

		if ( g_InputHelper.IsPressed( VK_RETURN ) )
		{
			active = false;

			if ( !active_str.empty( ) )
			{
				output = active_str;
				callback = true;
			}
		}
		g_Draw.GradientRect( x, y, x + ( w / 8 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
		g_Draw.GradientRect( x + ( w - ( w / 8 ) ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );

		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%s", active_str.empty( ) ? ( "Enter text" ) : active_str.c_str( ) );
	}

	else
	{
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%s", active_str.empty( ) ? ( "Enter text" ) : active_str.c_str( ) );
	}

	g_Draw.OutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

bool CMenu::InputKey( CVar<int> &output, bool bAllowNone )
{
	auto VK2STR = [ & ] ( const short key ) -> std::wstring
	{
		switch ( key ) {
		case VK_LBUTTON: return ( L"Left Mouse" );
		case VK_RBUTTON: return ( L"Right Mouse" );
		case VK_MBUTTON: return ( L"Middle Mouse" );
		case VK_XBUTTON1: return ( L"Mouse4" );
		case VK_XBUTTON2: return ( L"Mouse5" );
		case 0x0: return ( L"None" );
		default: break;
		}

		WCHAR output[ 16 ] = { L"\0" };

		if ( const int result = GetKeyNameTextW( MapVirtualKeyW( key, MAPVK_VK_TO_VSC ) << 16, output, 16 ) )
			return output;

		return ( L"VK2STR_FAILED" );
	};

	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::Outline );

	//static bool active = false, old_active = active;
	static float time = I::EngineClient->Time( );
	float elapsed = I::EngineClient->Time( ) - time;

	/*if ( old_active != active ) {
		time = I::EngineClient->Time( );
		old_active = active;
	*/

	static int *curr = nullptr, *prevv = curr;
	if ( curr != prevv )
	{
		time = I::EngineClient->Time( );
		prevv = curr;
	}

	if ( curr == nullptr && elapsed > 0.1f && g_InputHelper.IsPressed( VK_LBUTTON ) )
	{
		if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h ) {
			I::Surface->PlaySound( "UI/buttonclickrelease.wav" );
			curr = &output.var;
			g_InputHelper.NullKey( VK_LBUTTON );
		}
	}

	static float time_notactive = 0.0f;

	if ( g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h && curr == nullptr ) {
		g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WidgetActive );
	}

	if ( curr == &output.var )
	{
		m_bTyping = true;

		if ( I::EngineClient->Time( ) - time_notactive > 0.1f )
		{
			for ( short n = 0; n < 256; n++ )
			{
				if ( ( n > 0x0 && n < 0x7 )
					|| ( n > L'A' - 1 && n < L'Z' + 1 )
					|| n == VK_LSHIFT || n == VK_RSHIFT || n == VK_SHIFT
					|| n == VK_ESCAPE || n == VK_INSERT )
				{
					if ( g_InputHelper.IsPressed( n ) )
					{
						if ( n == VK_INSERT ) {
							curr = nullptr;
							break;
						}

						if ( n == VK_ESCAPE && bAllowNone ) {
							output.var = 0x0;
							curr = nullptr;
							break;
						}

						output.var = n;
						curr = nullptr;
						break;
					}
				}
			}
		}

		g_Draw.GradientRect( x, y, x + ( w / 8 ), y + h, Vars::Menu::Colors::TitleBar, Vars::Menu::Colors::Widget, true );
		g_Draw.GradientRect( x + ( w - ( w / 8 ) ), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::TitleBar, true );

		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%ws", ( L"Press a Key" ) );

	}

	else
	{
		if ( curr == nullptr )
		{
			time_notactive = I::EngineClient->Time( );
		}
		g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + ( w / 2 ), y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTER, "%ws", VK2STR( output.var ).c_str( ) );
	}

	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w + Vars::Menu::SpacingText, y + ( h / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, output.szName );
	g_Draw.OutlinedRect( x, y, w, h, Vars::Menu::Colors::OutlineMenu );

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.w = w;
	m_LastWidget.h = h;

	return callback;
}

void CMenu::GroupBoxStart( )
{
	m_LastGroupBox.x = m_LastWidget.x;
	m_LastGroupBox.y = m_LastWidget.y + m_LastWidget.h + Vars::Menu::SpacingY;

	m_LastWidget.x += Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
}

void CMenu::GroupBoxEnd( const wchar_t *Label, int Width )
{
	int h = m_LastWidget.y - m_LastGroupBox.y + m_LastWidget.h + Vars::Menu::SpacingY;

	int label_w, label_h;
	I::Surface->GetTextSize( g_Fonts[ EFonts::MENU ].m_dwFont, Label, label_w, label_h );

	int label_x = m_LastGroupBox.x + Vars::Menu::SpacingText;
	int label_y = m_LastGroupBox.y - ( label_h / 2 );

	g_Draw.Line( m_LastGroupBox.x, m_LastGroupBox.y, label_x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu );
	g_Draw.Line( label_x + label_w, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu );
	g_Draw.Line( m_LastGroupBox.x + Width, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.Line( m_LastGroupBox.x + Width, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu );
	g_Draw.Line( m_LastGroupBox.x, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu );

	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, label_x, label_y, Vars::Menu::Colors::Text, ALIGN_DEFAULT, Label );

	m_LastWidget.x -= Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
	m_LastGroupBox.h = h;
}

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WDATE WIDE1(__DATE__)
#define WTIME WIDE1(__TIME__)

void DrawWatermark( )
{
	if ( !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) ) { return; }
	/*if ( g_Menu.m_bOpen )
		g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 150, 400, Vars::Menu::Colors::CustomTitle, ALIGN_CENTER, L"Menu key pressed" );*/
	g_Draw.Rect( 0, g_Draw.m_nScreenH - 55, 4, 55, Vars::Menu::Colors::CustomTitle );
	I::Surface->DrawSetColor( 0, 0, 0, 255 );
	I::Surface->DrawFilledRectFade( 4, g_Draw.m_nScreenH - 55, 175, g_Draw.m_nScreenH, 222, 0, true );
	//I::Surface->DrawFilledRectFade( 4, g_Draw.m_nScreenH - 55, 135, g_Draw.m_nScreenH, 0, 222, false );
	int nTextOffset = 0;
	std::wstring build;

	//build = { L"[ALPHA]" };

	build = { L"[DEV]" };

	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9, g_Draw.m_nScreenH - 55 + 3, Vars::Menu::Colors::Text, ALIGN_DEFAULT, build.c_str( ) );
	int wide, tall; I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, build.c_str( ), wide, tall );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9 + wide + 3, g_Draw.m_nScreenH - 55 + 3, 
				   { Vars::Menu::Colors::CustomTitle.r, Vars::Menu::Colors::CustomTitle.g, Vars::Menu::Colors::CustomTitle.b, 255 },
				   ALIGN_DEFAULT, L"ateris." ); //+40
	int wide1, tall1; I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, L"ateris.", wide1, tall1 );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9 + wide + wide1 + 4, g_Draw.m_nScreenH - 55 + 3, Vars::Menu::Colors::Text, ALIGN_DEFAULT, L"technology" );
	nTextOffset += g_Fonts[ EFonts::WATERMARK ].m_nTall;

	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, L"Build: " );
	int wide2, tall2; I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, L"Build: ", wide2, tall2 );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9 + wide2, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, WDATE );
	int wide3, tall3; I::Surface->GetTextSize( g_Fonts[ EFonts::WATERMARK ].m_dwFont, WDATE, wide3, tall3 );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9 + wide2 + wide3 + 3, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, WTIME );
	/*time_t now = time( 0 );
	char buf[ 26 ];
	ctime_s( buf, sizeof( buf ), &now );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9, 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, "Time: %s", buf );*/
	nTextOffset += g_Fonts[ EFonts::WATERMARK ].m_nTall;

	INetChannelInfo *nci = I::EngineClient->GetNetChannelInfo( );

	float ping = 0;
	float choke = 0;
	float loss = 0;

	if ( nci )
	{
		float flow_incoming = nci->GetAvgLatency( FLOW_INCOMING );
		float flow_outgoing = nci->GetAvgLatency( FLOW_OUTGOING );
		float choke_incoming = nci->GetAvgChoke( FLOW_INCOMING );
		float choke_outgoing = nci->GetAvgChoke( FLOW_OUTGOING );
		float loss_incoming = nci->GetAvgLoss( FLOW_INCOMING );
		float loss_outgoing = nci->GetAvgLoss( FLOW_OUTGOING );
		ping = flow_incoming + flow_outgoing;
		choke = choke_incoming + choke_outgoing;
		loss = loss_incoming + loss_outgoing;
	}

	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, L"Ping: %i", ( int )( ping * 1000.0f ) );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 59, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, L"Choke: %i", ( int )( choke * 1000.0f ) );
	nTextOffset += g_Fonts[ EFonts::WATERMARK ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 59, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, L"Loss: %i", ( int )( loss * 1000.0f ) );

	int fps = Utils::GetFPS( );
	g_Draw.String( g_Fonts[ EFonts::WATERMARK ].m_dwFont, 9, g_Draw.m_nScreenH - 55 + 3 + nTextOffset, Vars::Menu::Colors::Text, ALIGN_DEFAULT, L"FPS: %i", fps );
	nTextOffset += g_Fonts[ EFonts::WATERMARK ].m_nTall;
}

void CMenu::DrawFirstWindow( )
{
	enum struct Window { Window, Close };
	static Window Tab = Window::Window;
	Vars::Menu::Colors::TitleBar = Vars::Menu::Colors::CustomTitle;

	if ( Tab == Window::Close )
		return;

	int w = 300;
	int h = 230;
	int x = g_Draw.m_nScreenW / 2 - w / 2;
	int y = g_Draw.m_nScreenH / 2 - h / 2;
	int nTextOffset = 0;

	g_Draw.Rect( x, y, w, h, Vars::Menu::Colors::WindowBackground );
	g_Draw.OutlinedRect( x, y, w, h, {0, 0, 0, 255 } );
	{
		g_Draw.Rect( x - 4, y - 4, w + 8, 4, Vars::Menu::Colors::TitleBar ); //top
		g_Draw.Rect( x - 4, y + h, w + 8, 4, Vars::Menu::Colors::TitleBar ); //bottom

		g_Draw.Rect( x - 4, y - 4, 4, h + 4, Vars::Menu::Colors::TitleBar ); //left
		g_Draw.Rect( x + w, y - 4, 4, h + 4, Vars::Menu::Colors::TitleBar ); //right
	}
	g_Draw.OutlinedRect( x - 5, y - 5, w + 10, h + 10, {0, 0, 0, 255 } );
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 4, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"THIS MESSAGE WILL APPEAR" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 4 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"ONLY ONE TIME" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall * 2.5;

	g_Draw.Line( x, y + 32, x + w - 1, y + 32, { 0, 0, 0, 255 } );
	g_Draw.Rect( x, y + 33, w, 4, Vars::Menu::Colors::TitleBar );
	g_Draw.Line( x, y + 37, x + w - 1, y + 37, { 0, 0, 0, 255 } );


	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"THIS COPY OF ATERIS SHOULD" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"BE USED ONLY BY AN ALPHA MEMBER" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall * 1.75;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"DO NOT REDISTRIBUTE/SHARE THIS FILE" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"WITH ANY FRIENDS/RELATIVES" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall * 1.75;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"IF YOU AGREE WITH THE MESSAGE" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"THEN CLICK OK AND" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"FEEL FREE TO ENJOY THE CHEAT" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall * 1.75;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"OTHERWISE IF YOU DON'T AGREE" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"AND/OR ARE NOT A LEGITIMATE ALPHA MEMBER" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"THEN UNINJECT IMMEDIATELY" );
	nTextOffset += g_Fonts[ EFonts::MENU ].m_nTall;
	g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + w / 2, y + 3 + nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTERHORIZONTAL, L"BY PRESSING THE [DEL] OR [F8] KEY" );

	if ( ButtonXY( L"OK", Tab == Window::Window, x + w / 2 - Vars::Menu::ButtonWSmall / 2, y + h - Vars::Menu::ButtonHSmall - 3, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall ) )
	{
		g_GlobalInfo.firsttime = false;
		Tab = Window::Close;
	}
}

void DrawAimFOV( )
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	if ( !pLocal || pLocal->deadflag( ) || pLocal->IsDormant( ) ) { return; }
	const auto& pWeapon = g_EntityCache.GetWeapon( );
	if ( !pWeapon || pWeapon->IsDormant( ) ) { return; }

	if ( !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) )
		return;

	const auto EWeaponType = Utils::GetWeaponType( pWeapon );
	float fov = 0.f;
	switch ( EWeaponType )
	{
		case EWeaponType::HITSCAN:
		{
			fov = Vars::Aimbot::Hitscan::AimFOV.var;
			break;
		}

		case EWeaponType::PROJECTILE:
		{
			fov = Vars::Aimbot::Projectile::AimFOV.var;
			break;
		}

		default: { fov = 0.00f; break; }
	}

	const float flFOV = static_cast< float >( Vars::Visuals::fov.var );
	const float flR = tanf( DEG2RAD( fov ) / 2.0f ) / tanf( DEG2RAD( ( pLocal->InCond( TF_COND_ZOOMED ) && !Vars::Visuals::RemoveScope.var ) ? 30.0f : flFOV ) / 2.0f ) * g_Draw.m_nScreenW;

	const Color_t clr = { 255, 255, 255, 150 };
	g_Draw.OutlinedCircle( g_Draw.m_nScreenW / 2, g_Draw.m_nScreenH / 2, flR, 68, clr );
}

CVar<int> unuPrimary = { 0, L"Unusual effect 1" };
CVar<int> unuSecondary = { 0, L"Unusual effect 2" };

void CMenu::Render( )
{
	DrawWatermark( );

	m_bReopened = false;
	m_bTyping = false;

	static bool bOldOpen = m_bOpen;

	if ( bOldOpen != m_bOpen )
	{
		bOldOpen = m_bOpen;

		if ( m_bOpen )
			m_bReopened = true;
	}

	if ( I::EngineClient->IsDrawingLoadingImage( ) ) {
		m_bOpen = false;
		return;
	}

	static float flTimeOnChange = 0.0f;

	if ( Utils::IsGameWindowInFocus( ) && ( ( GetAsyncKeyState( VK_INSERT ) & 1 ) || ( GetAsyncKeyState( VK_F3 ) & 1 ) ) ) {
		I::Surface->SetCursorAlwaysVisible( m_bOpen = !m_bOpen );
		flTimeOnChange = I::EngineClient->Time( );
		if ( m_bOpen )
		{
			I::Surface->PlaySound( "UI/slide_down.wav" );
		}
		else
		{
			I::Surface->PlaySound( "UI/slide_up.wav" );
		}
	}

	m_flFadeElapsed = I::EngineClient->Time( ) - flTimeOnChange;

	if ( m_flFadeElapsed < m_flFadeDuration ) {
		m_flFadeAlpha = Utils::RemapValClamped( m_flFadeElapsed, 0.0f, m_flFadeDuration, !m_bOpen ? 1.0f : 0.0f, m_bOpen ? 1.0f : 0.0f );
		I::Surface->DrawSetAlphaMultiplier( m_flFadeAlpha );
	}

	if ( m_bOpen || m_flFadeElapsed < m_flFadeDuration )
	{
		g_InputHelper.Update( );

		if ( !g_GlobalInfo.firsttime )
		{
			DrawAimFOV( );

			if ( !Vars::Menu::CustomTheme.var )
			{
				if ( Vars::Menu::LightMode.var && !Vars::Menu::CustomTheme.var )
				{
					Vars::Menu::Colors::WindowBackground = Vars::Menu::Colors::Text;
					Vars::Menu::Colors::OutlineMenu = { 0, 0, 0, 255 };
					Vars::Menu::Colors::WidgetActive = { 105, 105, 105, 255 };
				}

				if ( Vars::Menu::DarkMode.var )
				{
					Vars::Menu::Colors::WindowBackground = { 24, 24, 24, 255 };
					Vars::Menu::Colors::OutlineMenu = { 83, 83, 83, 255 };
					Vars::Menu::Colors::WidgetActive = { 70, 70, 70, 255 };
				}
			}

			const auto& pLocal = g_EntityCache.GetLocal( );

			if ( !Vars::Menu::CustomMode.var )
			{
				if ( I::EngineClient->IsConnected( ) || I::EngineClient->IsInGame( ) )
				{
					if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
					{
						if ( pLocal->m_iTeamNum( ) == TEAM_RED )
						{
							Vars::Menu::Colors::TitleBar = { 255, 70, 80, 255 };
						}
						else if ( pLocal->m_iTeamNum( ) == TEAM_BLU )
						{
							Vars::Menu::Colors::TitleBar = { 80, 140, 255, 255 };
						}
						else
						{
							Vars::Menu::Colors::TitleBar = { 90, 90, 90, 255 };
						}
					}
					else
					{
						Vars::Menu::Colors::TitleBar = { 90, 90, 90, 255 };
					}
				}
				else
				{
					Vars::Menu::Colors::TitleBar = { 90, 90, 90, 255 };
				}
			}
			else
			{
				Vars::Menu::Colors::TitleBar = Vars::Menu::Colors::CustomTitle;
			}

			//Window
			
			g_InputHelper.Drag(
				g_InputHelper.m_nMouseX,
				g_InputHelper.m_nMouseY,
				Vars::Menu::Position.x,
				Vars::Menu::Position.y,
				Vars::Menu::Position.w,
				Vars::Menu::TitleBarH,
				Vars::Menu::TitleBarH );
			{
				/*g_Draw.Rect(
					Vars::Menu::Position.x - 5,
					Vars::Menu::Position.y - 16,
					Vars::Menu::Position.w + 10,
					Vars::Menu::TitleBarH,
					Vars::Menu::Colors::TitleBar);*/

				g_Draw.Rect(
					Vars::Menu::Position.x - 5,
					Vars::Menu::Position.y - 16,
					Vars::Menu::Position.w + 9,
					15,
					Vars::Menu::Colors::TitleBar ); //topbar

				g_Draw.Rect(
					Vars::Menu::Position.x - 5,
					Vars::Menu::Position.y + Vars::Menu::TitleBarH - 21,
					Vars::Menu::Position.w + 9,
					4,
					Vars::Menu::Colors::TitleBar ); //bottombar

				g_Draw.Rect(
					Vars::Menu::Position.x - 5,
					Vars::Menu::Position.y - 1,
					4,
					Vars::Menu::TitleBarH - 20,
					Vars::Menu::Colors::TitleBar ); //leftbar

				g_Draw.Rect(
					Vars::Menu::Position.x + Vars::Menu::Position.w,
					Vars::Menu::Position.y - 1,
					4,
					Vars::Menu::TitleBarH - 20,
					Vars::Menu::Colors::TitleBar ); //rightbar

				g_Draw.Rect(
					Vars::Menu::Position.x,
					Vars::Menu::Position.y,
					Vars::Menu::Position.w,
					Vars::Menu::Position.h,
					Vars::Menu::Colors::WindowBackground ); //background

				g_Draw.OutlinedRect(
					Vars::Menu::Position.x - 6,
					Vars::Menu::Position.y - 17,
					Vars::Menu::Position.w + 11,
					Vars::Menu::TitleBarH + 1,
					Vars::Menu::Colors::Outline //outline
				);

				g_Draw.OutlinedRect(
					Vars::Menu::Position.x - 1,
					Vars::Menu::Position.y - 1,
					Vars::Menu::Position.w + 1,
					Vars::Menu::Position.h + 1,
					Vars::Menu::Colors::Outline //interior outline
				);

				g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont,
							   Vars::Menu::Position.x + 5,
							   Vars::Menu::Position.y - ( 16 / 2 ),
							   Vars::Menu::Colors::Text,
							   ALIGN_CENTERVERTICAL,
							   "%ls", ( L"Ateris II (ALPHA BUILD)" ) );



				int wide, tall;
				std::wstring name;

				//name = L"Harley Quinn";

				//name = L"macedoniaN";

				//name = L"BIALY";

				//name = L"DAFO";

				name = L"DEV";

				std::wstring user = { L"Build registered to: " + name };
				I::Surface->GetTextSize( g_Fonts[ EFonts::MENU ].m_dwFont, user.c_str( ), wide, tall );
				g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont,
							   Vars::Menu::Position.x + Vars::Menu::Position.w - wide,
							   Vars::Menu::Position.y - ( 16 / 2 ),
							   Vars::Menu::Colors::Text,
							   ALIGN_CENTERVERTICAL,
							   "%ls", user.c_str( ) );
			}

			//Components
			{
				static std::wstring selected = {};
				int config_count = 0;

				for ( const auto &entry : std::filesystem::directory_iterator( g_CFG.m_sConfigPath ) )
				{
					if ( std::string( std::filesystem::path( entry ).filename( ).string( ) ).find( ( ".cfg" ) ) == std::string_view::npos )
						continue;

					config_count++;
				}

				enum struct EMainTabs { TAB_AIM, TAB_TRIGGER, TAB_VISUALS, TAB_MISC, TAB_CONFIGS };
				enum struct EAimTabs { TAB_HITSCAN, TAB_PROJECTILE, TAB_MELEE };
				enum struct EVisualsTabs { TAB_INGAME, TAB_GUI };
				enum struct EESPTabs { TAB_PLAYERS, TAB_BUILDINGS, TAB_WORLD };
				enum struct EModelsTabs { TAB_PLAYERS, TAB_BUILDINGS, TAB_WORLD };
				enum struct EMiscTabs { TAB_GENERAL1, TAB_GENERAL2, TAB_TICKBASE };
				enum struct EOutlinesTabs { TAB_PLAYERS, TAB_BUILDINGS, TAB_WORLD };
				enum struct EColorsTabs { TAB_PLAYERS, TAB_BUILDINGS, TAB_WORLD };

				m_LastWidget = { Vars::Menu::Position.x + Vars::Menu::SpacingX, Vars::Menu::Position.y, 0, 0 };

				static EMainTabs Tab = EMainTabs::TAB_TRIGGER;
				{
					if ( Button( L"Aim", 0, 0, Tab == EMainTabs::TAB_AIM ) )
						Tab = EMainTabs::TAB_AIM;

					if ( Button( L"Trigger", 0, 0, Tab == EMainTabs::TAB_TRIGGER ) )
						Tab = EMainTabs::TAB_TRIGGER;

					if ( Button( L"Visuals", 0, 0, Tab == EMainTabs::TAB_VISUALS ) )
						Tab = EMainTabs::TAB_VISUALS;

					if ( Button( L"Misc", 0, 0, Tab == EMainTabs::TAB_MISC ) )
						Tab = EMainTabs::TAB_MISC;

					m_LastWidget = {
						Vars::Menu::Position.x + Vars::Menu::SpacingX,
						Vars::Menu::Position.y + Vars::Menu::Position.h - ( Vars::Menu::ButtonH * 2 + ( Vars::Menu::SpacingY * 2 ) + 15 ),
						0, 0 };

					/*if ( Button( L"Configs", 0, 0, Tab == EMainTabs::TAB_CONFIGS ) )
						Tab = EMainTabs::TAB_CONFIGS;*/
					if ( !selected.empty( ) )
					{
						m_LastWidget.y -= Vars::Menu::ButtonHSmall + 4;
					}
					m_LastWidget.y -= config_count * ( Vars::Menu::ButtonHSmall + 4 ) - 1;

					GroupBoxStart( );
					{
						for ( const auto &entry : std::filesystem::directory_iterator( g_CFG.m_sConfigPath ) )
						{
							if ( std::string( std::filesystem::path( entry ).filename( ).string( ) ).find( ( ".cfg" ) ) == std::string_view::npos )
								continue;

							std::wstring s = entry.path( ).filename( ).wstring( );
							s.erase( s.end( ) - 4, s.end( ) );

							if ( Button( s.c_str( ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, s == selected ) )
								selected = s;

							CVar<int> item{};
							for ( int i = 0; i < config_count; i++ )
							{
								item.var = i;
								item.szName = s.c_str( );
							}
						}

						if ( config_count < 1 )
						{
							m_LastWidget.y -= Vars::Menu::SpacingY * 4;
							m_LastWidget.w += Vars::Menu::ButtonWSmall;
							m_LastWidget.h += Vars::Menu::ButtonHSmall;
						}

						if ( config_count < 12 )
						{
							std::wstring output = {};

							if ( InputString( ( L"+ Add" ), output ) )
							{
								if ( !std::filesystem::exists( g_CFG.m_sConfigPath + L"\\" + output ) )
									g_CFG.Save( output.c_str( ) );
							}
						}
						enum struct Remove { Remove, Remove2 };
						static Remove Tab = Remove::Remove;

						switch ( Tab )
						{
							case Remove::Remove:
							{
								if ( !selected.empty( ) )
								{
									if ( Button( ( L"Remove" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall ) )
									{
										Tab = Remove::Remove2;
									}
								}
								break;
							}

							case Remove::Remove2:
							{
								if ( GetAsyncKeyState( VK_ESCAPE ) & 1 )
									Tab = Remove::Remove;

								if ( Button( ( L"..." ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall ) )
								{
									g_CFG.Remove( selected.c_str( ) );
									selected.clear( );
									Tab = Remove::Remove;
								}
								break;
							}
						}
					}
					GroupBoxEnd( /*std::wstring( ( L"Configs(" ) + std::to_wstring( config_count ) + ( L")" ) ).c_str( )*/ L"", Vars::Menu::ButtonWSmall + ( Vars::Menu::SpacingX * 2 ) );

					m_LastWidget.x += Vars::Menu::SpacingX * 2 + 3;
					Rect_t checkpoint = m_LastWidget;
					m_LastWidget.x -= Vars::Menu::SpacingX * 2 + 3;
					enum struct Save { Save, Save2 };
					static Save Tab = Save::Save;

					switch ( Tab )
					{
						case Save::Save:
						{
							if ( Button( L"Save", 41, Vars::Menu::ButtonHSmall ) )
							{
								if ( !selected.empty( ) )
								{
									Tab = Save::Save2;
								}
							}
							break;
						}

						case Save::Save2:
						{
							if ( Tab == Save::Save2 )
							{
								if ( GetAsyncKeyState( VK_ESCAPE ) & 1 )
									Tab = Save::Save;

								if ( Button( L"...", 41, Vars::Menu::ButtonHSmall ) )
								{
									g_CFG.Save( selected.c_str( ) );
									selected.clear( );
									Tab = Save::Save;
								}
							}
							break;
						}
					}

					m_LastWidget.x += 46;
					m_LastWidget.y -= Vars::Menu::ButtonHSmall + Vars::Menu::SpacingY;

					if ( Button( L"Load", 41, Vars::Menu::ButtonHSmall ) )
					{
						if ( !selected.empty( ) )
						{
							g_CFG.Load( selected.c_str( ) );
							selected.clear( );
						}
					}

					m_LastWidget.x += Vars::Menu::Position.x + Vars::Menu::SpacingX * 2;

					m_LastWidget = checkpoint;
				}

				Separator( Vars::Menu::Colors::TitleBar );

				switch ( Tab )
				{
					case EMainTabs::TAB_AIM:
					{
						/* {
							Rect_t checkpoint_line = m_LastWidget;
							checkpoint_line.x -= Vars::Menu::SpacingX;
							checkpoint_line.y += Vars::Menu::ButtonHSmall + ( Vars::Menu::SpacingY * 2 );
							Rect_t checkpoint_move = m_LastWidget;

							if ( Button( ( L"Hitscan" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EAimTabs::TAB_HITSCAN ) )
								Tab = EAimTabs::TAB_HITSCAN;

							checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint_move;

							if ( Button( ( L"Projectile" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EAimTabs::TAB_PROJECTILE ) )
								Tab = EAimTabs::TAB_PROJECTILE;

							checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint_move;

							if ( Button( ( L"Melee" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EAimTabs::TAB_MELEE ) )
								Tab = EAimTabs::TAB_MELEE;

							m_LastWidget = checkpoint_line;
							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y, Vars::Menu::Colors::OutlineMenu );

							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y, Vars::Menu::Colors::Outline );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 1, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 1, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 2, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 2, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 3, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 3, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 4, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 4, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y + 5, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y + 5, Vars::Menu::Colors::Outline );

							checkpoint_line.x += Vars::Menu::SpacingX;
							checkpoint_line.y += Vars::Menu::SpacingY;
							m_LastWidget = checkpoint_line;
						}
						*/

						Rect_t checkpoint = m_LastWidget;
						checkpoint.y += Vars::Menu::SpacingY;
						m_LastWidget = checkpoint;

						GroupBoxStart( );
						{
							CheckBox( Vars::Aimbot::Global::Active );

							if ( Vars::Aimbot::Global::Active.var )
							{
								InputKey( Vars::Aimbot::Global::AimKey );
								CheckBox( Vars::Aimbot::Global::AutoShoot );
								CheckBox( Vars::Aimbot::Global::AimPlayers );
								CheckBox( Vars::Aimbot::Global::AimBuildings );
								CheckBox( Vars::Aimbot::Global::AimNPCs );
								CheckBox( Vars::Aimbot::Global::AimStickies );
								//CheckBox( Vars::Aimbot::Global::AmmoCheck );
								//SliderInt( Vars::Aimbot::Global::AmmoCount, 0, 100 );
								CheckBox( Vars::Aimbot::Global::IgnoreInvulnerable );
								CheckBox( Vars::Aimbot::Global::IgnoreCloaked );
								CheckBox( Vars::Aimbot::Global::IgnoreFriends );
								CheckBox( Vars::Aimbot::Global::IgnoreTaunting );
							}
						}
						GroupBoxEnd( ( L"Global" ), 190 );

						GroupBoxStart( );
						{
							CheckBox( Vars::Aimbot::Projectile::Active );

							if ( Vars::Aimbot::Projectile::Active.var )
							{
								ComboBox( Vars::Aimbot::Projectile::SortMethod, { {0, L"FOV" }, {1, L"Distance" } } );	//0 - FOV,		1 - Distance
								if ( Vars::Aimbot::Projectile::SortMethod.var == 1 )
								{
									CheckBox( Vars::Aimbot::Projectile::RespectFOV );
								}
								ComboBox( Vars::Aimbot::Projectile::AimMethod, { {0, L"Normal" }, {1, L"Silent" } } );	//0 - Normal,	1 - Silent
								ComboBox( Vars::Aimbot::Projectile::AimPosition, { {0, L"Head" }, { 1, L"Body" }, { 2, L"Feet" } , { 3, L"Auto" } } );	//0 - Body,		1 - Feet,	2 - Auto
								SliderFloat( Vars::Aimbot::Projectile::AimFOV, 0.f, 180.f, L"%.1f" );
								CheckBox( Vars::Aimbot::Projectile::HeadAllowed );
								CheckBox( Vars::Aimbot::Projectile::BodyAllowed );
								CheckBox( Vars::Aimbot::Projectile::FeetAllowed );
								CheckBox( Vars::Aimbot::Projectile::FeetAimIfOnGround );
								CheckBox( Vars::Aimbot::Projectile::ChargeLooseCannon );
								/*CheckBox( Vars::Aimbot::Projectile::StrafePredictionAir );
								CheckBox( Vars::Aimbot::Projectile::StrafePredictionGround );*/
								CheckBox( Vars::Aimbot::Projectile::PredictedPath );
								if ( Vars::Aimbot::Projectile::PredictedPath.var )
								{
									SliderFloat( Vars::Aimbot::Projectile::PathDuration, 0.f, 10.f, L"%.1f" );
									ColorPicker( Vars::Colors::PredictedPath, L"" );
								}
								CheckBox( Vars::Aimbot::Projectile::TargetBBox );
								if ( Vars::Aimbot::Projectile::TargetBBox.var )
								{
									SliderFloat( Vars::Aimbot::Projectile::BBoxDuration, 0.f, 10.f, L"%.1f" );
									ColorPicker( Vars::Colors::TargetBBox, L"" );
								}
								//InputFloat( Vars::Aimbot::Projectile::PredictionTime, 0.1f, 10.f, 0.1f, L"%.1f" );
								//InputInt( Vars::Aimbot::Projectile::VisTestPoints, 3, 15 );
								//InputInt( Vars::Aimbot::Projectile::ScanPoints, 3, Vars::Aimbot::Projectile::VisTestPoints.var );
							}
						}
						GroupBoxEnd( L"Projectile", 190 );

						checkpoint.x += 190 + Vars::Menu::SpacingX;
						m_LastWidget = checkpoint;

						GroupBoxStart( );
						{
							CheckBox( Vars::Aimbot::Hitscan::Active );

							if ( Vars::Aimbot::Hitscan::Active.var )
							{
								ComboBox( Vars::Aimbot::Hitscan::SortMethod, { {0, L"FOV" }, {1, L"Distance" } } );	//0 - FOV,		1 - Distance
								ComboBox( Vars::Aimbot::Hitscan::AimMethod, { {0, L"Normal" }, {1, L"Smooth" }, {2, L"Silent" } } );	//0 - Normal,	1 - Smooth, 2 - Silent
								ComboBox( Vars::Aimbot::Hitscan::AimHitbox, { {0, L"Head" }, {1, L"Body" }, {2, L"Auto" } } );	//0 - Head,		1 - Body,	2 - Auto
								SliderFloat( Vars::Aimbot::Hitscan::AimFOV, 0.0f, 180.f, L"%.1f" );
								SliderFloat( Vars::Aimbot::Hitscan::SmoothingAmount, 1.0f, 10.0f, L"%.1f" );
								ComboBox( Vars::Aimbot::Hitscan::TapFire, { {0, L"Off" }, {1, L"Distance" }, {2, L"Always" } } );	//0 - Off, 1 - Distance, 2 - Always
								CheckBox( Vars::Backtrack::Active );
								if ( Vars::Backtrack::Active.var )
									CheckBox( Vars::Backtrack::Indicator );
								CheckBox( Vars::Aimbot::Hitscan::ScanHitboxes );
								CheckBox( Vars::Aimbot::Hitscan::ScanHead );
								CheckBox( Vars::Aimbot::Hitscan::ScanBuildings );
								CheckBox( Vars::Aimbot::Hitscan::WaitForHeadshot );
								CheckBox( Vars::Aimbot::Hitscan::WaitForCharge );
								CheckBox( Vars::Aimbot::Hitscan::SpectatedSmooth );
								CheckBox( Vars::Aimbot::Hitscan::ScopedOnly );
								CheckBox( Vars::Aimbot::Hitscan::AutoScope );
								CheckBox( Vars::Aimbot::Hitscan::AutoRev );
								CheckBox( Vars::Aimbot::Hitscan::ShowHitboxes );
								if ( Vars::Aimbot::Hitscan::ShowHitboxes.var )
								{
									SliderFloat( Vars::Aimbot::Hitscan::HitboxDuration, 0.f, 10.f, L"%.1f" );
									ColorPicker( Vars::Colors::ShowHitboxes, L"" );
								}
							}
						}
						GroupBoxEnd( ( L"Hitscan" ), 190 );

						/*checkpoint.x += 160 + Vars::Menu::SpacingX;
						m_LastWidget = checkpoint;

						Rect_t checkpoint = m_LastWidget;
						checkpoint.y += Vars::Menu::SpacingY;
						m_LastWidget = checkpoint;*/

						/*checkpoint.y += 30 + Vars::Menu::SpacingY;
						if ( Vars::Aimbot::Global::Active.var )
						{
							checkpoint.y += 140 + Vars::Menu::SpacingY;
						}
						checkpoint.x -= 190 + Vars::Menu::SpacingX;
						m_LastWidget = checkpoint;*/
						break;
					}

					case EMainTabs::TAB_TRIGGER:
					{
						Rect_t checkpoint = m_LastWidget;
						checkpoint.y += Vars::Menu::SpacingY;
						m_LastWidget = checkpoint;

						GroupBoxStart( );
						{
							CheckBox( Vars::Triggerbot::Active );
							if ( Vars::Triggerbot::Active.var )
							{
								InputKey( Vars::Triggerbot::Key, Vars::Triggerbot::Key.szName );
								CheckBox( Vars::Triggerbot::TriggerPlayers );
								CheckBox( Vars::Triggerbot::TriggerBuildings );
								CheckBox( Vars::Triggerbot::HeadOnly );
								CheckBox( Vars::Triggerbot::WaitForHeadshot );
								CheckBox( Vars::Triggerbot::ScopeOnly );
								SliderFloat( Vars::Triggerbot::HeadScale, 0.0f, 1.0f, L"%.2f" );
							}
						}
						GroupBoxEnd( L"Global", 190 );
						break;
					}

					case EMainTabs::TAB_VISUALS:
					{
						static EVisualsTabs Tab = EVisualsTabs::TAB_INGAME;
						{
							Rect_t checkpoint_line = m_LastWidget;
							checkpoint_line.x -= Vars::Menu::SpacingX;
							checkpoint_line.y += Vars::Menu::ButtonHSmall + ( Vars::Menu::SpacingY * 2 );
							Rect_t checkpoint_move = m_LastWidget;

							if ( Button( ( L"In-game" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EVisualsTabs::TAB_INGAME ) )
								Tab = EVisualsTabs::TAB_INGAME;

							checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint_move;

							if ( Button( ( L"GUI" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EVisualsTabs::TAB_GUI ) )
								Tab = EVisualsTabs::TAB_GUI;

							m_LastWidget = checkpoint_line;
							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y, Vars::Menu::Colors::OutlineMenu );

							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y, Vars::Menu::Colors::Outline );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 1, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 1, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 2, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 2, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 3, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 3, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 4, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 4, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y + 5, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y + 5, Vars::Menu::Colors::Outline );

							checkpoint_line.x += Vars::Menu::SpacingX;
							checkpoint_line.y += Vars::Menu::SpacingY;
							m_LastWidget = checkpoint_line;
						}

						switch ( Tab )
						{
							case EVisualsTabs::TAB_INGAME:
							{
								Rect_t checkpoint = m_LastWidget;
								checkpoint.y += Vars::Menu::SpacingY;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									static EESPTabs Tab = EESPTabs::TAB_PLAYERS;

									if ( Button( L"Players", 55, Vars::Menu::ButtonHSmall, Tab == EESPTabs::TAB_PLAYERS ) )
										Tab = EESPTabs::TAB_PLAYERS;

									if ( ButtonXY( L"Buildings", Tab == EESPTabs::TAB_BUILDINGS, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EESPTabs::TAB_BUILDINGS;

									if ( ButtonXY( L"World", Tab == EESPTabs::TAB_WORLD, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EESPTabs::TAB_WORLD;
									m_LastWidget.x -= 60 * 2;

									CheckBox( Vars::ESP::Main::esp_global );
									if ( Vars::ESP::Main::esp_global.var )
									{
										CheckBox( Vars::ESP::Main::esp_dormant );
										CheckBox( Vars::ESP::Main::esp_cheaptext );

										if ( Tab == EESPTabs::TAB_PLAYERS || Tab == EESPTabs::TAB_BUILDINGS )
											CheckBox( Vars::ESP::Main::esp_enemies );

										if ( Tab == EESPTabs::TAB_PLAYERS )
										{
											CheckBox( Vars::ESP::Players::esp_players );
											if ( Vars::ESP::Players::esp_players.var )
											{
												CheckBox( Vars::ESP::Players::name );
												CheckBox( Vars::ESP::Players::box );
												CheckBox( Vars::ESP::Players::healthbar );
												CheckBox( Vars::ESP::Players::gradient );
												CheckBox( Vars::ESP::Players::conds );
												CheckBox( Vars::ESP::Players::skeleton );
												CheckBox( Vars::ESP::Players::weapons );
												CheckBox( Vars::ESP::Players::distance );
												SliderInt( Vars::ESP::Players::lines, 0, 10 );
											}
										}

										if ( Tab == EESPTabs::TAB_BUILDINGS )
										{
											CheckBox( Vars::ESP::Buildings::esp_buildings );
											if ( Vars::ESP::Buildings::esp_buildings.var )
											{
												CheckBox( Vars::ESP::Buildings::name );
												CheckBox( Vars::ESP::Buildings::box );
												CheckBox( Vars::ESP::Buildings::healthbar );
												CheckBox( Vars::ESP::Buildings::gradient );
												CheckBox( Vars::ESP::Buildings::distance );
												CheckBox( Vars::ESP::Buildings::conds );
												SliderInt( Vars::ESP::Buildings::lines, 0, 10 );
											}
										}

										if ( Tab == EESPTabs::TAB_WORLD )
										{
											CheckBox( Vars::ESP::World::esp_world );
											if ( Vars::ESP::World::esp_world.var )
											{
												CheckBox( Vars::ESP::World::name );
												CheckBox( Vars::ESP::World::box );
												CheckBox( Vars::ESP::World::distance );
											}
										}
									}
								}
								GroupBoxEnd( ( L"ESP" ), 190 );

								GroupBoxStart( );
								{
									static EModelsTabs Tab = EModelsTabs::TAB_PLAYERS;

									if ( Button( L"Players", 55, Vars::Menu::ButtonHSmall, Tab == EModelsTabs::TAB_PLAYERS ) )
										Tab = EModelsTabs::TAB_PLAYERS;

									if ( ButtonXY( L"Buildings", Tab == EModelsTabs::TAB_BUILDINGS, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EModelsTabs::TAB_BUILDINGS;

									if ( ButtonXY( L"World", Tab == EModelsTabs::TAB_WORLD, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EModelsTabs::TAB_WORLD;
									m_LastWidget.x -= 60 * 2;

									CheckBox( Vars::Models::Main::Enabled );
									ComboBox( Vars::Models::Main::ModelOrder, { { 0, L"Models First" }, { 1, L"Outlines First" } } );

									if ( Tab == EModelsTabs::TAB_PLAYERS || Tab == EModelsTabs::TAB_BUILDINGS )
										CheckBox( Vars::Models::Main::Enemies );

									if ( Vars::Models::Main::Enabled.var )
									{
										if ( Tab == EModelsTabs::TAB_PLAYERS )
										{
											CheckBox( Vars::Models::Players::Enabled );
											if ( Vars::Models::Players::Enabled.var )
											{
												CheckBox( Vars::Models::Players::IgnoreZ );
												CheckBox( Vars::Models::Players::Overlay );
												ComboBox( Vars::Models::Players::Model, { {Vars::Models::NONE, L"None" }, {Vars::Models::FLAT, L"Flat" }, {Vars::Models::SHADED, L"Shaded" }, { Vars::Models::FRESNEL, L"Fresnel" } } );
												if ( Vars::Models::Players::Model.var == Vars::Models::FRESNEL )
													ColorPicker( Vars::Colors::Fresnel, L"Base Color" );
												if ( Vars::Models::Players::Overlay.var )
													SliderFloat( Vars::Models::Players::OverlayAlpha, 0.0f, 1.0f, L"%.1f" );
												//InputFloat( Vars::Models::Players::Alpha, 0.0f, 1.0f, 0.01f, L"%.2f" );
											}
										}

										if ( Tab == EModelsTabs::TAB_BUILDINGS )
										{
											CheckBox( Vars::Models::Buildings::Enabled );
											if ( Vars::Models::Buildings::Enabled.var )
											{
												CheckBox( Vars::Models::Buildings::IgnoreZ );
												CheckBox( Vars::Models::Buildings::Overlay );
												ComboBox( Vars::Models::Buildings::Model, { {Vars::Models::NONE, L"None" }, {Vars::Models::FLAT, L"Flat" }, {Vars::Models::SHADED, L"Shaded" }, { Vars::Models::FRESNEL, L"Fresnel" } } );
												if ( Vars::Models::Buildings::Model.var == Vars::Models::FRESNEL )
													ColorPicker( Vars::Colors::Fresnel, L"Base Color" );
												if ( Vars::Models::Buildings::Overlay.var )
													SliderFloat( Vars::Models::Buildings::OverlayAlpha, 0.0f, 1.0f, L"%.1f" );
												//InputFloat( Vars::Models::Buildings::Alpha, 0.0f, 1.0f, 0.01f, L"%.2f" );
											}
										}

										if ( Tab == EModelsTabs::TAB_WORLD )
										{
											CheckBox( Vars::Models::World::Enabled );
											if ( Vars::Models::World::Enabled.var )
											{
												CheckBox( Vars::Models::World::IgnoreZ );
												CheckBox( Vars::Models::World::Overlay );
												ComboBox( Vars::Models::World::Model, { {Vars::Models::NONE, L"None" }, {Vars::Models::FLAT, L"Flat" }, {Vars::Models::SHADED, L"Shaded" }, { Vars::Models::FRESNEL, L"Fresnel" } } );
												if ( Vars::Models::World::Model.var == Vars::Models::FRESNEL )
													ColorPicker( Vars::Colors::Fresnel, L"Base Color" );
												if ( Vars::Models::World::Overlay.var )
													SliderFloat( Vars::Models::World::OverlayAlpha, 0.0f, 1.0f, L"%.1f" );
												//InputFloat( Vars::Models::World::Alpha, 0.0f, 1.0f, 0.01f, L"%.2f" );
											}
										}
									}
								}
								GroupBoxEnd( ( L"Models" ), 190 );

								checkpoint.x += 190 + Vars::Menu::SpacingX;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									static EOutlinesTabs Tab = EOutlinesTabs::TAB_PLAYERS;

									if ( Button( L"Players", 55, Vars::Menu::ButtonHSmall, Tab == EOutlinesTabs::TAB_PLAYERS ) )
										Tab = EOutlinesTabs::TAB_PLAYERS;

									if ( ButtonXY( L"Buildings", Tab == EOutlinesTabs::TAB_BUILDINGS, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EOutlinesTabs::TAB_BUILDINGS;

									if ( ButtonXY( L"World", Tab == EOutlinesTabs::TAB_WORLD, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EOutlinesTabs::TAB_WORLD;
									m_LastWidget.x -= 60 * 2;

									CheckBox( Vars::Outline::Main::Enabled );

									if ( Tab == EOutlinesTabs::TAB_PLAYERS || Tab == EOutlinesTabs::TAB_BUILDINGS )
										CheckBox( Vars::Outline::Main::Enemies );

									if ( Vars::Outline::Main::Enabled.var )
									{
										CheckBox( Vars::Outline::Main::BlurOutline );
										CheckBox( Vars::Outline::Main::WireframeOutline );
										CheckBox( Vars::Outline::Main::CheapOutline );


										if ( Tab == EOutlinesTabs::TAB_PLAYERS )
										{
											CheckBox( Vars::Outline::Players::Enabled );
											if ( Vars::Outline::Players::Enabled.var )
											{
												CheckBox( Vars::Outline::Players::ViewmodelOutline );
												CheckBox( Vars::Outline::Players::AttachmentsOutline );
												SliderInt( Vars::Outline::Players::OutlineScale, 0, 10 );
												SliderFloat( Vars::Outline::Players::OutlineAlpha, 0.0f, 1.0f, L"%.2f" );
											}
										}

										if ( Tab == EOutlinesTabs::TAB_BUILDINGS )
										{
											CheckBox( Vars::Outline::Buildings::Enabled );
											if ( Vars::Outline::Buildings::Enabled.var )
											{
												SliderInt( Vars::Outline::Buildings::OutlineScale, 0, 10 );
												SliderFloat( Vars::Outline::Buildings::OutlineAlpha, 0.0f, 1.0f, L"%.2f" );
											}
										}

										if ( Tab == EOutlinesTabs::TAB_WORLD )
										{
											CheckBox( Vars::Outline::World::Enabled );
											if ( Vars::Outline::World::Enabled.var )
											{
												SliderInt( Vars::Outline::World::OutlineScale, 0, 10 );
												SliderFloat( Vars::Outline::World::OutlineAlpha, 0.0f, 1.0f, L"%.2f" );
											}
										}
									}
								}
								GroupBoxEnd( L"Outlines", 190 );

								GroupBoxStart( );
								{
									static EColorsTabs Tab = EColorsTabs::TAB_PLAYERS;

									if ( Button( L"Players", 55, Vars::Menu::ButtonHSmall, Tab == EColorsTabs::TAB_PLAYERS ) )
										Tab = EColorsTabs::TAB_PLAYERS;

									if ( ButtonXY( L"Buildings", Tab == EColorsTabs::TAB_BUILDINGS, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EColorsTabs::TAB_BUILDINGS;

									if ( ButtonXY( L"World", Tab == EColorsTabs::TAB_WORLD, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 55, Vars::Menu::ButtonHSmall ) )
										Tab = EColorsTabs::TAB_WORLD;
									m_LastWidget.x -= 60 * 2;

									if ( Tab == EColorsTabs::TAB_PLAYERS )
									{
										ComboBox( Vars::Colors::Scheme, { { 0, L"Enemy/Team" }, { 1, L"Red/Blu" } } );
										ColorPicker( Vars::Colors::LocalPlayer, L"Local" );
										if ( Vars::Colors::Scheme.var == 0 )
										{
											ColorPicker( Vars::Colors::Enemy, L"Enemy" );
											ColorPicker( Vars::Colors::Teammate, L"Teammate" );
										}

										if ( Vars::Colors::Scheme.var == 1 )
										{
											ColorPicker( Vars::Colors::Red, L"Red" );
											ColorPicker( Vars::Colors::Blu, L"Blu" );
											ColorPicker( Vars::Colors::Cloaked, L"Cloaked" );
										}
									}

									if ( Tab == EColorsTabs::TAB_BUILDINGS )
									{
										ComboBox( Vars::Colors::Scheme, { { 0, L"Enemy/Team" }, { 1, L"Red/Blu" } } );
										ColorPicker( Vars::Colors::LocalBuilding, L"Local" );
										if ( Vars::Colors::Scheme.var == 0 )
										{
											ColorPicker( Vars::Colors::EnemyBuilding, L"Enemy" );
											ColorPicker( Vars::Colors::TeammateBuilding, L"Teammate" );
										}

										if ( Vars::Colors::Scheme.var == 1 )
										{
											ColorPicker( Vars::Colors::RedBuilding, L"Red" );
											ColorPicker( Vars::Colors::BluBuilding, L"Blu" );
										}
									}

									if ( Tab == EColorsTabs::TAB_WORLD )
									{
										ColorPicker( Vars::Colors::Health, L"Healthpacks" );
										ColorPicker( Vars::Colors::Ammo, L"Ammopacks" );
										ColorPicker( Vars::Colors::Bomb, L"Bombs" );
										ColorPicker( Vars::Colors::HeadlessHatman, L"Horseman" );
										ColorPicker( Vars::Colors::Merasmus, L"Merasmus" );
										ColorPicker( Vars::Colors::Monoculus, L"Monoculus" );
										ColorPicker( Vars::Colors::Zombie, L"Zombie" );
										ColorPicker( Vars::Colors::Tank, L"MvM Tank" );
									}
								}
								GroupBoxEnd( L"Colors", 190 );

								checkpoint.x += 190 + Vars::Menu::SpacingX;
								m_LastWidget = checkpoint;
								break;
							}

							case EVisualsTabs::TAB_GUI:
							{
								Rect_t checkpoint = m_LastWidget;
								checkpoint.y += Vars::Menu::SpacingY;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									CheckBox( Vars::Menu::CustomMode );
									CheckBox( Vars::Menu::CustomTheme );

									if ( !Vars::Menu::CustomTheme.var )
									{
										if ( Button( L"Light Mode", 85, Vars::Menu::ButtonHSmall, Vars::Menu::LightMode.var == true ) || Vars::Menu::LightMode.var == true && Vars::Menu::DarkMode.var == false )
										{
											Vars::Menu::DarkMode.var = false;
											Vars::Menu::LightMode.var = true;
										}

										if ( ButtonXY( L"Dark Mode", Vars::Menu::DarkMode.var == true, m_LastWidget.x + m_LastWidget.w + 5, m_LastWidget.y, 85, Vars::Menu::ButtonHSmall )
											 || Vars::Menu::LightMode.var == false && Vars::Menu::DarkMode.var == true )
										{
											Vars::Menu::LightMode.var = false;
											Vars::Menu::DarkMode.var = true;
										}

										m_LastWidget.x -= m_LastWidget.w + 5;

									}

									else//if ( Vars::Menu::CustomTheme.var )
									{
										ColorPicker( Vars::Menu::Colors::WindowBackground, L"Background" );
										ColorPicker( Vars::Menu::Colors::Outline, L"Menu Outline" );
										ColorPicker( Vars::Menu::Colors::OutlineMenu, L"Group Outline" );
										ColorPicker( Vars::Menu::Colors::Widget, L"Widget" );
										ColorPicker( Vars::Menu::Colors::WidgetActive, L"Widget Active" );
										ColorPicker( Vars::Menu::Colors::Text, L"Text" );
									}
									if ( Vars::Menu::CustomMode.var )
										ColorPicker( Vars::Menu::Colors::CustomTitle, L"Menu Color" );
								}
								GroupBoxEnd( L"Menu", 190 );

								checkpoint.x += 190 + Vars::Menu::SpacingX;
								m_LastWidget = checkpoint;

								break;
							}
						}
						break;
					}

					case EMainTabs::TAB_MISC:
					{
						static EMiscTabs Tab = EMiscTabs::TAB_GENERAL1;
						{
							Rect_t checkpoint_line = m_LastWidget;
							checkpoint_line.x -= Vars::Menu::SpacingX;
							checkpoint_line.y += Vars::Menu::ButtonHSmall + ( Vars::Menu::SpacingY * 2 );
							Rect_t checkpoint_move = m_LastWidget;

							if ( Button( ( L"General 1" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EMiscTabs::TAB_GENERAL1 ) )
								Tab = EMiscTabs::TAB_GENERAL1;

							checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint_move;

							if ( Button( ( L"General 2" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EMiscTabs::TAB_GENERAL2 ) )
								Tab = EMiscTabs::TAB_GENERAL2;

							checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint_move;

							if ( Button( ( L"Tickbase" ), Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall, Tab == EMiscTabs::TAB_TICKBASE ) )
								Tab = EMiscTabs::TAB_TICKBASE;

							m_LastWidget = checkpoint_line;
							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y, Vars::Menu::Colors::OutlineMenu );

							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y, Vars::Menu::Colors::Outline );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 1, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 1, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 2, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 2, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 3, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 3, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 2, checkpoint_line.y + 4, Vars::Menu::Position.x + Vars::Menu::Position.w, checkpoint_line.y + 4, Vars::Menu::Colors::TitleBar );
							g_Draw.Line( checkpoint_line.x + 3, checkpoint_line.y + 5, Vars::Menu::Position.x + Vars::Menu::Position.w - 1, checkpoint_line.y + 5, Vars::Menu::Colors::Outline );

							checkpoint_line.x += Vars::Menu::SpacingX;
							checkpoint_line.y += Vars::Menu::SpacingY;
							m_LastWidget = checkpoint_line;
						}

						std::wstring szTickBase = Vars::Misc::StacBypass.var ? L"Tick Shifting (Anti-Cheat)" : L"Tick Shifting";

						switch ( Tab )
						{
							case EMiscTabs::TAB_GENERAL1:
							{
								Rect_t checkpoint = m_LastWidget;
								checkpoint.y += Vars::Menu::SpacingY;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::AutoJump );
									CheckBox( Vars::Misc::BreakJump );
									CheckBox( Vars::Misc::ScoutJump );
									if ( Vars::Misc::ScoutJump.var )
									{
										InputKey( Vars::Misc::ScoutJumpKey );
									}
									CheckBox( Vars::Misc::AccurateMovement );
									CheckBox( Vars::Misc::TomislavRev );
									ComboBox( Vars::Misc::AutoStrafe, { { 0, L"None"}, { 1, L"Normal" }, { 2, L"Directional" } } );
									CheckBox( Vars::Misc::DisableInterpolation );
									CheckBox( Vars::Misc::DisableTaunts );
									CheckBox( Vars::Misc::AnimFix );
									CheckBox( Vars::Misc::NoPush );
								}
								GroupBoxEnd( ( L"Main" ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Visuals::RagdollEffects::Active );
									if ( Vars::Visuals::RagdollEffects::Active.var )
									{
										CheckBox( Vars::Visuals::RagdollEffects::EnemyOnly );
										CheckBox( Vars::Visuals::RagdollEffects::Burning );
										CheckBox( Vars::Visuals::RagdollEffects::Electrocuted );
										CheckBox( Vars::Visuals::RagdollEffects::BecomeAsh );
										CheckBox( Vars::Visuals::RagdollEffects::Dissolve );
										ComboBox( Vars::Visuals::RagdollEffects::RagdollType, { {0, L"None" }, {1, L"Gold" }, {2, L"Ice" } } );
									}
								}
								GroupBoxEnd( L"Ragdolls", 190 );

								GroupBoxStart( );
								{
									SliderInt( Vars::Visuals::ViewmodelFOV, 0, 160 );
									CheckBox( Vars::Visuals::ViewmodelPos );
									if ( Vars::Visuals::ViewmodelPos.var )
									{
										CheckBox( Vars::Visuals::ViewmodelSway );
										CheckBox( Vars::Visuals::ViewmodelFlip );
										if ( Vars::Visuals::ViewmodelPos.var )
										{
											SliderInt( Vars::Visuals::ViewX, -45, 45 );
											SliderInt( Vars::Visuals::ViewY, -45, 45 );
											SliderInt( Vars::Visuals::ViewZ, -45, 45 );
											SliderInt( Vars::Visuals::ViewRoll, -180, 180 );
										}
									}
								}
								GroupBoxEnd( ( L"Viewmodel" ), 190 );

								checkpoint.x += 190 + Vars::Menu::SpacingX;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									CheckBox( Vars::Visuals::RemoveScope );
									SliderInt( Vars::Visuals::fov, 50, 140 );
									SliderFloat( Vars::Visuals::AspectRatio, 0.f, 2.f, L"%.2f" );
									ComboBox( Vars::Visuals::ParticleTracer, { {0, L"None" }, {1, L"Machina" }, {2, L"C.A.P.P.E.R" }, {3, L"Short circuit" }, {4, L"Merasmus" }, {5, L"Merasmus Beam" }, {6, L"Big Nasty" } } );
									CheckBox( Vars::Visuals::WorldModulation );
									ColorPicker( Vars::Colors::WorldModulation, L"World Color" );
									ColorPicker( Vars::Colors::StaticPropModulation, L"Props Color" );
									ColorPicker( Vars::Colors::ParticleModulation, L"Particles Color" );
									CheckBox( Vars::Visuals::ParticlesTeamColor );
								}
								GroupBoxEnd( ( L"Visuals" ), 190 );

								/*GroupBoxStart( );
								{
									CheckBox( Vars::Visuals::Skins::Override ); //australium
									ComboBox( Vars::Visuals::Skins::Sheen, { {0, L"None" }, {1, L"Team Colored" }, {2, L"Yellow" }, {3, L"Orange"},
																		   {4, L"Lime"}, {5, L"Emerald"}, {6, L"Violet"}, {7, L"Pink"} } );
									ComboBox( unuPrimary, { {0, L"None" }, {1, L"Hot" }, {2, L"Isotope" }, {3, L"Cool"}, {4, L"Energy Orb"} } );
									switch ( unuPrimary.var )
									{
										case 0:
											Vars::Visuals::Skins::Particle.var = 0;
											break;
										case 1:
											Vars::Visuals::Skins::Particle.var = 701;
											break;
										case 2:
											Vars::Visuals::Skins::Particle.var = 702;
											break;
										case 3:
											Vars::Visuals::Skins::Particle.var = 703;
											break;
										case 4:
											Vars::Visuals::Skins::Particle.var = 704;
											break;
										default:
											break;
									}
									ComboBox( unuSecondary, { {0, L"None" }, {1, L"Hot" }, {2, L"Isotope" }, {3, L"Cool"}, {4, L"Energy Orb"} } );
									switch ( unuSecondary.var )
									{
										case 0:
											Vars::Visuals::Skins::Effect.var = 0;
											break;
										case 1:
											Vars::Visuals::Skins::Effect.var = 701;
											break;
										case 2:
											Vars::Visuals::Skins::Effect.var = 702;
											break;
										case 3:
											Vars::Visuals::Skins::Effect.var = 703;
											break;
										case 4:
											Vars::Visuals::Skins::Effect.var = 704;
											break;
										default:
											break;
									}

									if ( Button( L"Apply", 45, 20 ) )
										g_AttributeChanger.SetAttribute( );

									m_LastWidget.x += 45 + Vars::Menu::SpacingX / 2;
									m_LastWidget.y -= 20 + Vars::Menu::SpacingY;

									if ( Button( L"Save", 45, 20 ) )
										g_AttributeChanger.SaveConfig( );

									m_LastWidget.x += 45 + Vars::Menu::SpacingX / 2;
									m_LastWidget.y -= 20 + Vars::Menu::SpacingY;

									if ( Button( L"Load", 45, 20 ) )
										g_AttributeChanger.LoadConfig( );

									m_LastWidget.x -= 90 + Vars::Menu::SpacingX;
								}
								GroupBoxEnd( ( L"Weapon" ), 190 );*/

								GroupBoxStart( );
								{
									CheckBox( Vars::CritHack::Active );
									if ( Vars::CritHack::Active.var )
									{
										CheckBox( Vars::CritHack::AvoidRandom );
										CheckBox( Vars::CritHack::Indicators );
										InputKey( Vars::CritHack::CritKey, Vars::CritHack::CritKey.szName );
									}
								}
								GroupBoxEnd( ( L"CritHack" ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::Steam::EnableRPC );
									if ( Vars::Misc::Steam::EnableRPC.var )
									{
										ComboBox( Vars::Misc::Steam::MatchGroup, { {0, L"Special Event" }, {1, L"Mann Up" }, {2, L"Competitive" }, {3, L"Casual" }, {4, L"Boot Camp" } } );
										CheckBox( Vars::Misc::Steam::OverrideMenu ); // Override matchgroup when in main menu
										SliderInt( Vars::Misc::Steam::GroupSize, 0, 10000 );
										ComboBox( Vars::Misc::Steam::MapText, { {0, L"Custom" }, {1, L"ateris.tech" }, {2, L"ateris" }, {3, L"Ateris V2" }, {4, L"rijin.solutions" }, {5, L"(Visuals Only)" }, {6, L"(Visuals Only) 2" } } );
										if ( !Vars::Misc::Steam::MapText.var )
											InputStringC( Vars::Misc::Steam::CustomText.szName, Vars::Misc::Steam::CustomText.var );
									}
								}
								GroupBoxEnd( ( L"Steam RPC" ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::CheatsBypass );
									CheckBox( Vars::Misc::PureBypass );
								}
								GroupBoxEnd( ( L"Bypasses" ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::StacBypass );
								}
								GroupBoxEnd( ( L"Anti-Cheat" ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Visuals::OutOfFOVArrows );
									if ( Vars::Visuals::OutOfFOVArrows.var )
									{
										CheckBox( Vars::Visuals::ArrowsIgnoreFriends );
										CheckBox( Vars::Visuals::ArrowsEnemyOnly );
										CheckBox( Vars::Visuals::ArrowsDormant );
										CheckBox( Vars::Visuals::ArrowsDistanceAlpha );
									}
									/*InputFloat( Vars::Visuals::ArrowLength, 5.f, 50.f, 0.5f, L"%.1f" );
									InputFloat( Vars::Visuals::ArrowAngle, 5.f, 180.f, 0.5f, L"%.1f" );
									InputFloat( Vars::Visuals::FovArrowsDist, 0.01f, 0.2f, 0.01f, L"%.2f" );
									InputFloat( Vars::Visuals::MaxDist, 5.f, 4000.f, 5.f, L"%.1f" );
									InputFloat( Vars::Visuals::MinDist, 5.f, 4000.f, 5.f, L"%.1f" );*/
								}
								GroupBoxEnd( ( L"Arrows" ), 190 );
								break;
							}

							case EMiscTabs::TAB_GENERAL2:
							{
								Rect_t checkpoint = m_LastWidget;
								checkpoint.y += Vars::Menu::SpacingY;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									InputKey( Vars::Visuals::ThirdPersonKey, Vars::Visuals::ThirdPersonKey.szName );
									CheckBox( Vars::Visuals::ThirdpersonOffset );
									if ( Vars::Visuals::ThirdpersonOffset.var )
									{
										SliderFloat( Vars::Visuals::ThirdpersonDist, -300.f, 300.f, L"%.1f" );
										SliderFloat( Vars::Visuals::ThirdpersonRight, -300.f, 300.f, L"%.1f" );
										SliderFloat( Vars::Visuals::ThirdpersonUp, -300.f, 300.f, L"%.1f" );
									}
								}
								GroupBoxEnd( ( L"ThirdPerson" ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::Regions::RegionSelector );

									if ( Vars::Misc::Regions::RegionSelector.var )
									{
										CheckBox( Vars::Misc::Regions::EUEast );		//EU East		- Frankfurt, Stockholm, Warsaw, Vienna
										CheckBox( Vars::Misc::Regions::EUWest );		//EU West		- Amsterdam, London, Luxembourg, Madrid, Paris
										CheckBox( Vars::Misc::Regions::Asia );			//Asia			- Manila, Singapore
										CheckBox( Vars::Misc::Regions::MiddleEast );	//Middle East	- Dubai
										CheckBox( Vars::Misc::Regions::HongKong );		//Hong Kong		- Hong Kong
										CheckBox( Vars::Misc::Regions::Japan );			//Japan			- Tokyo (GNRT)
										CheckBox( Vars::Misc::Regions::India );			//India			- Mumbai, Chennai
										CheckBox( Vars::Misc::Regions::Australia );		//Australia		- Sydney
										CheckBox( Vars::Misc::Regions::Africa );		//Africa		- Johannesburg
										CheckBox( Vars::Misc::Regions::Chile );			//Chile			- Santiago
										CheckBox( Vars::Misc::Regions::Peru );			//Peru			- Lima
										CheckBox( Vars::Misc::Regions::Brazil );		//Brazil		- Sao Paulo
										CheckBox( Vars::Misc::Regions::USEast );		//US East		- Atlanta, Virginia, Chicago
										CheckBox( Vars::Misc::Regions::USNorthWest );	//US North West - Moses Lake, Washington, Seattle
										CheckBox( Vars::Misc::Regions::USSouthWest );	//US South West - Los Angeles, Oklahoma City
									}
								}
								GroupBoxEnd( ( L"Region Selector" ), 190 );

								checkpoint.x += 190 + Vars::Menu::SpacingX;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::TauntSlide );
									CheckBox( Vars::Misc::TauntSpin );
									if ( Vars::Misc::TauntSlide.var )
									{
										CheckBox( Vars::Misc::TauntControl );
										CheckBox( Vars::Misc::TauntFollowsCamera );
									}
									if ( Vars::Misc::TauntSpin.var )
									{
										InputKey( Vars::Misc::TauntSpinKey );
										SliderFloat( Vars::Misc::TauntSpinSpeed, 0.1f, 100.f, L"%.1f" );
									}
								}
								GroupBoxEnd( ( L"Taunt Movement" ), 190 );

								GroupBoxStart( );
								{
									if ( Button( L"Unlock", 42, Vars::Menu::ButtonHSmall ) )
									{
										g_Misc.UnlockAchievements( );
									}

									m_LastWidget.x += 48;
									m_LastWidget.y -= Vars::Menu::ButtonHSmall + Vars::Menu::SpacingY;

									if ( Button( L"Lock", 42, Vars::Menu::ButtonHSmall ) )
									{
										g_Misc.LockAchievements( );
									}
								}
								GroupBoxEnd( L"Achievements", Vars::Menu::ButtonWSmall + ( Vars::Menu::SpacingX * 4 ) + 3 );

								break;
							}

							case EMiscTabs::TAB_TICKBASE:
							{
								Rect_t checkpoint = m_LastWidget;
								checkpoint.y += Vars::Menu::SpacingY;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::CL_Move::Enabled );

									if ( Vars::Misc::CL_Move::Enabled.var )
									{
										CheckBox( Vars::Misc::CL_Move::Doubletap );
										CheckBox( Vars::Misc::CL_Move::AntiWarp );
										CheckBox( Vars::Misc::CL_Move::AutoPeek );
										if ( Vars::Misc::CL_Move::AutoPeek.var )
											CheckBox( Vars::Misc::CL_Move::AutoPeekFree );
										//ComboBox( Vars::Misc::CL_Move::TeleportMode, { {0, L"Normal" }, {1, L"Smooth" } } );
										InputKey( Vars::Misc::CL_Move::RechargeKey ); //H
										InputKey( Vars::Misc::CL_Move::TeleportKey ); //R
										if ( Vars::Misc::CL_Move::AutoPeek.var )
											InputKey( Vars::Misc::CL_Move::AutoPeekKey );
										ComboBox( Vars::Misc::CL_Move::Indicator, { { 0, L"None" }, { 1, L"ateris.tech" }, { 2, L"ChadAlphaMales" }, { 3, L"deadflag" },
												  { 4, L"deadflag 2" }, { 5, L"Harpoon" }, { 6, L"intellect" } } );
										SliderInt( Vars::Misc::CL_Move::DTTicksStanding, 14, 22 );
										SliderInt( Vars::Misc::CL_Move::DTTicksMoving, 14, 22 );
										CheckBox( Vars::Misc::CL_Move::PassiveRecharge );
										if ( Vars::Misc::CL_Move::PassiveRecharge.var )
										{
											SliderInt( Vars::Misc::CL_Move::TickFrequency, 4, 48 );
										}
									}
								}
								GroupBoxEnd( szTickBase.c_str( ), 190 );

								GroupBoxStart( );
								{
									CheckBox( Vars::Misc::CL_Move::Fakelag );
									if ( Vars::Misc::CL_Move::Fakelag.var )
									{
										CheckBox( Vars::Misc::CL_Move::FakelagSafe );

										ComboBox( Vars::Misc::CL_Move::FakelagMode, { {0, L"Plain" }, {1, L"Random" }, {2, L"Speed Based" } } ); // 0 - plain, 1 - random, 2 - vel based

										if ( Vars::Misc::CL_Move::FakelagMode.var == 1 )
										{
											SliderInt( Vars::Misc::CL_Move::FakelagMin, 1, Vars::Misc::CL_Move::FakelagMax.var ); //	only show when FakelagMode=2
											SliderInt( Vars::Misc::CL_Move::FakelagMax, Vars::Misc::CL_Move::FakelagMin.var, Vars::Misc::CL_Move::FakelagSafe.var ? 14 : 21 );
										}

										if ( Vars::Misc::CL_Move::FakelagMode.var != 1 )
											SliderInt( Vars::Misc::CL_Move::FakelagValue, 1, Vars::Misc::CL_Move::FakelagSafe.var ? 14 : 21 ); // dont show when fakelagmode=2

										if ( Vars::Misc::CL_Move::FakelagOnKey.var )
											InputKey( Vars::Misc::CL_Move::FakelagKey ); //T

										if ( Vars::Misc::CL_Move::FakelagMode.var == 0 )
											CheckBox( Vars::Misc::CL_Move::FakelagOnKey ); // dont show when fakelagmode=2|3
									}
								}
								GroupBoxEnd( L"Fakelag", 190 );

								checkpoint.x += 190 + Vars::Menu::SpacingX;
								m_LastWidget = checkpoint;

								GroupBoxStart( );
								{
									CheckBox( Vars::AntiHack::AntiAimBeta::Active );
									if ( Vars::AntiHack::AntiAimBeta::Active.var )
									{
										CheckBox( Vars::AntiHack::AntiAimBeta::Rage );
										InputKey( Vars::AntiHack::AntiAimBeta::ToggleKey );
										if ( Vars::AntiHack::AntiAimBeta::Rage.var )
										{
											//pitch
											ComboBox( Vars::AntiHack::AntiAimBeta::Pitch, { { 0, L"None" }, { 1, L"Automatic" }, { 2, L"Manual" } } );
											if ( Vars::AntiHack::AntiAimBeta::Pitch.var == 2 )
											{
												CheckBox( Vars::AntiHack::AntiAimBeta::FakePitch );
												ComboBox( Vars::AntiHack::AntiAimBeta::RealPitch, { { 0, L"Up" }, { 1, L"Down" }, { 2, L"Jitter" } } );
											}

											//real yaw
											int rmode = Vars::AntiHack::AntiAimBeta::RealMode.var;
											ComboBox( Vars::AntiHack::AntiAimBeta::RealMode, { { 0, L"Static" }, { 1, L"Edge" }, { 2, L"Jitter" }, { 3, L"Spin" }, { 4, L"Rotate" } } );
											if ( rmode == 0 || rmode == 2 || rmode == 4 )
											{
												InputKey( Vars::AntiHack::AntiAimBeta::CycleRealKey );
												SliderInt( Vars::AntiHack::AntiAimBeta::RealYaw, -180, 180 );
											}
											if ( rmode == 4 )
											{
												SliderInt( Vars::AntiHack::AntiAimBeta::RealRotate, 1, 15 );
												SliderInt( Vars::AntiHack::AntiAimBeta::RealAngle1, -180, Vars::AntiHack::AntiAimBeta::RealAngle2.var );
												SliderInt( Vars::AntiHack::AntiAimBeta::RealAngle2, Vars::AntiHack::AntiAimBeta::RealAngle1.var, 180 );
											}
											if ( rmode == 2 )
											{
												CheckBox( Vars::AntiHack::AntiAimBeta::CycleRealJitter );
												SliderInt( Vars::AntiHack::AntiAimBeta::RealDelta, 0, 180 );
											}

											//fake yaw
											int fmode = Vars::AntiHack::AntiAimBeta::FakeMode.var;
											ComboBox( Vars::AntiHack::AntiAimBeta::FakeMode, { { 0, L"Static" }, { 1, L"Edge" }, { 2, L"Jitter" }, { 3, L"Spin" } } );
											if ( fmode == 0 || fmode == 2 )
											{
												InputKey( Vars::AntiHack::AntiAimBeta::CycleFakeKey );
												SliderInt( Vars::AntiHack::AntiAimBeta::FakeYaw, -180, 180 );
											}
											if ( fmode == 2 )
											{
												CheckBox( Vars::AntiHack::AntiAimBeta::CycleFakeJitter );
												SliderInt( Vars::AntiHack::AntiAimBeta::FakeDelta, 0, 180 );
											}

											if ( fmode == 3 )
											{
												SliderInt( Vars::AntiHack::AntiAimBeta::FakeSpin, -30, 30 );
											}
										}
										else
										{
											ComboBox( Vars::AntiHack::AntiAimBeta::LegitYawMode, { { 0, L"Static" }, { 1, L"Edge" } } );
											if ( Vars::AntiHack::AntiAimBeta::LegitYawMode.var == 0 )
											{
												InputKey( Vars::AntiHack::AntiAimBeta::CycleKey );
												SliderInt( Vars::AntiHack::AntiAimBeta::LegitYaw, -180, 180 );
											}
										}
									}
									/*CheckBox( Vars::AntiHack::AntiAim::Active );
									if ( Vars::AntiHack::AntiAim::Active.var )
									{
										CheckBox( Vars::AntiHack::AntiAim::AntiOverlap );
										CheckBox( Vars::AntiHack::AntiAim::AntiBackstab );
										ComboBox( Vars::AntiHack::AntiAim::Pitch, { { 0, ( L"None" ) },{ 1, ( L"Zero" ) }, { 2, ( L"Up" ) }, { 3, ( L"Down" ) }, { 4, ( L"Fake Up" ) },
																					{ 5, ( L"Fake Down" ) }, { 6, ( L"Random" ) }, { 7, ( L"Half Up" ) }, { 8, ( L"Jitter" ) } } );

										ComboBox( Vars::AntiHack::AntiAim::YawReal, { { 0, ( L"None" ) }, { 1, ( L"Forwards" ) }, { 2, ( L"Left" ) }, { 3, ( L"Right" ) }, { 4, ( L"Backwards" ) },
																					  { 5, ( L"Random" ) }, { 6, ( L"Spin" ) }, { 7, ( L"Edge" ) }, { 8, ( L"On Hurt" ) } } );

										ComboBox( Vars::AntiHack::AntiAim::YawFake, { { 0, ( L"None" ) }, { 1, ( L"Forwards" ) }, { 2, ( L"Left" ) }, { 3, ( L"Right" ) }, { 4, ( L"Backwards" ) },
																					  { 5, ( L"Random" ) }, { 6, ( L"Spin" ) }, { 7, ( L"Edge" ) }, { 8, ( L"On Hurt" ) } } );

										if ( Vars::AntiHack::AntiAim::YawFake.var == 6 || Vars::AntiHack::AntiAim::YawReal.var == 6 )
										{
											SliderFloat( Vars::AntiHack::AntiAim::SpinSpeed, -30.f, 30.f, L"%.1f" );
										}

										if ( Vars::AntiHack::AntiAim::Pitch.var == 6 || Vars::AntiHack::AntiAim::YawFake.var == 5 || Vars::AntiHack::AntiAim::YawReal.var == 5 )
										{
											SliderInt( Vars::AntiHack::AntiAim::RandInterval, 0, 100 );
										}
									}*/
								}
								GroupBoxEnd( ( L"AntiAim" ), 190 );

								/*GroupBoxStart( );
								{
									CheckBox( Vars::Visuals::ThirdPersonServerHitbox );
									CheckBox( Vars::Visuals::Debug );*/
									/*SliderFloat( Vars::Aimbot::Hitscan::SmoothingAmount, 0.f, 9.f, L"%.1f" );
									SliderInt( Vars::ESP::Players::lines, -30, 30 );
									ColorPicker( Vars::Colors::Bomb, L"Bomb" );*/
									/*}
									GroupBoxEnd( L"Debug", 190 ); */

								break;
							}
							default: break;
						}
						break;
					}

					case EMainTabs::TAB_CONFIGS:
					{
						Rect_t checkpoint = m_LastWidget;
						checkpoint.y += Vars::Menu::SpacingY;
						m_LastWidget = checkpoint;

						break;
					}

					default: break;

				}
			}
		}

		else
		{
			DrawFirstWindow( );
		}
	}

	I::Surface->DrawSetAlphaMultiplier( 1.0f );
}

/*void CMenu::InputPlayer( Player_t player )
{
	int x = m_LastWidgetPL.x;
	int y = m_LastWidgetPL.y;
	const int _x = x;
	const auto &pLocal = g_EntityCache.GetLocal( );
	Color_t teamcolor = player.m_iIndex == I::EngineClient->GetLocalPlayer( ) ? Vars::Colors::LocalPlayer : ( player.m_iTeam == pLocal->m_iTeamNum( )
		? Color_t{ Vars::Colors::Teammate.r, Vars::Colors::Teammate.g, Vars::Colors::Teammate.b, 255 } 
		: Color_t{ Vars::Colors::Enemy.r, Vars::Colors::Enemy.g, Vars::Colors::Enemy.b, 255 } );
	g_Draw.Rect( x - 1, y - 1, 731, 15, teamcolor );
	g_Draw.OutlinedRect( x - 1, y - 1, 206, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), { 255, 255, 255, 255 }, ALIGN_CENTERVERTICAL, Utils::ConvertUtf8ToWide( player.m_szName ).c_str( ) );
	x += 205;

	std::wstring alive = player.m_bAlive ? L"Alive" : L"Dead";
	/*Color_t colalive = player.m_bAlive ? Color_t{ 255, 255, 255, 255 } : Color_t{ 170, 170, 170, 255 };*//*
	g_Draw.OutlinedRect( x - 1, y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), { 255, 255, 255, 255 }, ALIGN_CENTERVERTICAL, alive.c_str( ) );
	x += 44;

	/*std::wstring bfriend = player.m_bFriend ? L"Friend" : L"Not Friend";
	Color_t friendcolor = player.m_bFriend ? Vars::Colors::LocalPlayer : Color_t{ 255, 255, 255, 255 };
	g_Draw.OutlinedRect( x - 1, y - 1, 65, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), friendcolor, ALIGN_CENTERVERTICAL, bfriend.c_str( ) );*//*
	x += 64;

	g_Draw.OutlinedRect( x - 1, y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"%i", player.m_iIndex );
	x += 44;

	g_Draw.OutlinedRect( x - 1, y - 1, 55, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"%i", player.m_iUserID );
	x += 54;

	g_Draw.OutlinedRect( x - 1, y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"%i", player.m_iScore );
	x += 44;

	int kills = player.m_iKills == 0 ? 1 : player.m_iKills;
	int deaths = player.m_iDeaths == 0 ? 1 : player.m_iDeaths;
	Color_t killscol = ( kills / deaths ) >= 10 ? Color_t{ 255, 128, 0, 255 } : Color_t{ 255, 255, 255, 255 };
	g_Draw.OutlinedRect( x - 1, y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), killscol, ALIGN_CENTERVERTICAL, L"%i", player.m_iKills );
	x += 44;

	Color_t deathscol = ( deaths / kills ) >= 10 ? Color_t{ 255, 0, 0, 255 } : Color_t{ 255, 255, 255, 255 };
	g_Draw.OutlinedRect( x - 1, y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), deathscol, ALIGN_CENTERVERTICAL, L"%i", player.m_iDeaths );
	x += 44;

	Color_t pingcol = ( player.m_iPing != 0 ) ? ( ( player.m_iPing > 100 ) ? Color_t{ 255, 178, 0, 255 } : Color_t{ 150, 255, 150, 255 } ) : Color_t{ 255, 255, 255, 255 };
	const wchar_t* ping = player.m_iPing == 0 ? L"BOT" : L"%i";
	g_Draw.OutlinedRect( x - 1, y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
	g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + 3, y + ( 12 / 2 ), pingcol, ALIGN_CENTERVERTICAL, ping, player.m_iPing );
	x += 44;

	g_Draw.OutlinedRect( x - 1, y - 1, 90, 15, { Vars::Menu::Colors::OutlineMenu } );
	if ( player.m_iPing != 0 )
		PriorityCombo( x, g_GlobalInfo.PlayerPriority[ player.m_iFriendID ].Mode, { { 0, L"Friend" }, { 1, L"Ignore" }, { 2, L"Default" }, { 3, L"Rage" }, { 4, L"Cheater" } } );
	else
		g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + ( 66 * 0.5f ), y + ( 12 / 2 ), { 255, 255, 255, 255 }, ALIGN_CENTERVERTICAL, L"BOT" );
	/*g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + 3, y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Priority" );*//*
	x += 89;

	g_Draw.OutlinedRect( x - 1, y - 1, 55, 15, { Vars::Menu::Colors::OutlineMenu } );
	/*g_Draw.String( g_Fonts[ EFonts::MENU ].m_dwFont, x + 3, y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Ateris UID" );*//*
	x += 54;

	if ( ( playerlist.m_nMouseX > _x - 1 && playerlist.m_nMouseX < (_x - 1 ) + 731 && playerlist.m_nMouseY > y - 1 && playerlist.m_nMouseY < ( y - 1 ) + 15 ) )
	{
		g_Draw.Rect( _x - 1, y - 1, 731, 15, { 255, 255, 255, 15 } );
		/*if ( playerlist.IsPressed( VK_RBUTTON ) )
		{
			g_Draw.Rect( _x + 1, y + 1, 50, 15, { 0, 0, 0, 255 } );
		}*//*
	}
	
	m_LastWidgetPL.y += 14;
}

void CMenu::PlayerList( )
{
	if ( m_flFadeElapsed < m_flFadeDuration )
	{
		I::Surface->DrawSetAlphaMultiplier( m_flFadeAlpha );
	}

	if ( m_bOpen || m_flFadeElapsed < m_flFadeDuration )
	{
		playerlist.Update( );

		playerlist.Drag1(
			playerlist.m_nMouseX,
			playerlist.m_nMouseY,
			Vars::Playerlist::Position.x,
			Vars::Playerlist::Position.y,
			Vars::Playerlist::Position.w,
			Vars::Playerlist::TitleBarH,
			Vars::Playerlist::TitleBarH );

		g_Draw.Rect(
			Vars::Playerlist::Position.x - 5,
			Vars::Playerlist::Position.y - 16,
			Vars::Playerlist::Position.w + 9,
			15,
			Vars::Menu::Colors::TitleBar ); //topbar

		g_Draw.Rect(
			Vars::Playerlist::Position.x - 5,
			Vars::Playerlist::Position.y + Vars::Playerlist::TitleBarH - 21,
			Vars::Playerlist::Position.w + 9,
			4,
			Vars::Menu::Colors::TitleBar ); //bottombar

		g_Draw.Rect(
			Vars::Playerlist::Position.x - 5,
			Vars::Playerlist::Position.y - 1,
			4,
			Vars::Playerlist::TitleBarH - 20,
			Vars::Menu::Colors::TitleBar ); //leftbar

		g_Draw.Rect(
			Vars::Playerlist::Position.x + Vars::Playerlist::Position.w,
			Vars::Playerlist::Position.y - 1,
			4,
			Vars::Playerlist::TitleBarH - 20,
			Vars::Menu::Colors::TitleBar ); //rightbar

		g_Draw.Rect(
			Vars::Playerlist::Position.x,
			Vars::Playerlist::Position.y,
			Vars::Playerlist::Position.w,
			Vars::Playerlist::Position.h,
			Vars::Menu::Colors::WindowBackground ); //background

		g_Draw.OutlinedRect(
			Vars::Playerlist::Position.x - 6,
			Vars::Playerlist::Position.y - 17,
			Vars::Playerlist::Position.w + 11,
			Vars::Playerlist::TitleBarH + 1,
			Vars::Menu::Colors::Outline //outline
		);

		g_Draw.OutlinedRect(
			Vars::Playerlist::Position.x - 1,
			Vars::Playerlist::Position.y - 1,
			Vars::Playerlist::Position.w + 1,
			Vars::Playerlist::Position.h + 1,
			Vars::Menu::Colors::Outline //interior outline
		);

		g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont,
					   Vars::Playerlist::Position.x + 5,
					   Vars::Playerlist::Position.y - ( 16 / 2 ),
					   Vars::Menu::Colors::Text,
					   ALIGN_CENTERVERTICAL ,
					   "Player List" );

		{
			m_LastWidgetPL = { Vars::Playerlist::Position.x + 1, Vars::Playerlist::Position.y + 1, 0, 0 };

			g_Draw.BlackOutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, Vars::Playerlist::Position.w - 1, 15, { Vars::Menu::Colors::OutlineMenu } );

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 206, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Name" );
			m_LastWidgetPL.x += 205;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Status" );
			m_LastWidgetPL.x += 44;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 65, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Friend" );
			m_LastWidgetPL.x += 64;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Index" );
			m_LastWidgetPL.x += 44;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 55, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"User ID" );
			m_LastWidgetPL.x += 54;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Score" );
			m_LastWidgetPL.x += 44;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Kills" );
			m_LastWidgetPL.x += 44;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Deaths" );
			m_LastWidgetPL.x += 44;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 45, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Ping" );
			m_LastWidgetPL.x += 44;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 90, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Priority" );
			m_LastWidgetPL.x += 89;

			g_Draw.OutlinedRect( m_LastWidgetPL.x - 1, m_LastWidgetPL.y - 1, 55, 15, { Vars::Menu::Colors::OutlineMenu } );
			g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, m_LastWidgetPL.x + 3, m_LastWidgetPL.y + ( 15 / 2 ), Vars::Menu::Colors::Text, ALIGN_CENTERVERTICAL, L"Ateris ID" );

			m_LastWidgetPL.x -= 676;
			m_LastWidgetPL.y += 14;

			/*Player_t nigger = { L"nigger", false, false, 2, 2, 0, 0, 100, 215 };
			InputPlayer( nigger );
			Player_t chad = { L"chad", true, true, 1, 1, 100, 100, 0, 15 };
			InputPlayer( chad );*//*

			//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "%i\n", g_PlayerList.PlayerCache.size( ) );

			for ( const auto &[Team, Player] : g_PlayerList.PlayerCache )
			{
				InputPlayer( Player );
			}
		}
	}

	I::Surface->DrawSetAlphaMultiplier( 1.0f );
}*/