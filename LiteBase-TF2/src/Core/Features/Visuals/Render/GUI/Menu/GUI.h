#pragma once

#include "../../../../../SDK/SDK.h"
#include "InputHelper/InputHelper.h"
#include "PlayerList/PlayerList.h"
//#include <ctime>
struct Player_t;

class CMenu
{
private:
	Rect_t m_LastWidget = {};
	Rect_t m_LastWidgetPL = {};
	Rect_t m_LastGroupBox = {};
	Color_t PastColor = {};
	bool m_bHovered = false;
	inline static CInputHelper playerlist;

	std::vector<std::wstring> configs = {};

	void Separator();
	void Separator(Color_t color);
	bool CheckBox(CVar<bool>& Var);
	bool Button(const wchar_t* Label, int WidthOverride = 0, int HeightOverride = 0, bool Active = false);
	bool ButtonXY(const wchar_t* Label, bool Active = false, int XOverride = 0, int YOverride = 0, int WidthOverride = 0, int HeightOverride = 0);
	bool ComboBox(CVar<int>& Var, const std::vector<CVar<int>>& List);
	bool InputFloat(CVar<float>& Var, float Min, float Max, float Step = 1.0f, const wchar_t* Fmt = L"%f");
	bool InputInt(CVar<int>& Var, int Min, int Max, int Step = 1);
	bool InputColor(Color_t& Var, const wchar_t* Label);
	bool InputString(const wchar_t* Label, std::wstring& output);
	bool InputStringC( const wchar_t *Label, std::string &output );
	bool InputKey( CVar<int> &output, bool bAllowNone = true );
	bool InputKey1( CVar<int> &output, bool bAllowNone = true );
	bool InputKey2( CVar<int> &output, bool bAllowNone = true );
	bool InputKey3( CVar<int> &output, bool bAllowNone = true );
	bool InputKey4( CVar<int> &output, bool bAllowNone = true );
	bool InputKey5( CVar<int> &output, bool bAllowNone = true );
	bool InputKey6( CVar<int> &output, bool bAllowNone = true );
	void InputPlayer( Player_t player );
	void GroupBoxStart();
	void GroupBoxEnd(const wchar_t* Label, int Width);
	bool SliderInt(CVar<int>& Var, int Min, int Max );
	bool SliderFloat(CVar<float>& Var, float Min, float Max, const wchar_t* Fmt = L"%f" );
	bool ColorPicker( Color_t &Var, const wchar_t *Label );
	bool ColorPicker1( Color_t &Var, const wchar_t *Label );
	bool ColorPicker2( Color_t &Var, const wchar_t *Label );
	bool ColorPicker3( Color_t &Var, const wchar_t *Label );
	bool ColorPicker4( Color_t &Var, const wchar_t *Label );
	bool ColorPicker5( Color_t &Var, const wchar_t *Label );
	bool ColorPicker6( Color_t &Var, const wchar_t *Label );
	bool ColorPicker7( Color_t &Var, const wchar_t *Label );
	bool ColorPicker8( Color_t &Var, const wchar_t *Label );
	bool ColorPicker9( Color_t &Var, const wchar_t *Label );
	bool ColorPicker0( Color_t &Var, const wchar_t *Label );
	bool ColorPicker10( Color_t &Var, const wchar_t *Label );
	bool ColorPicker11( Color_t &Var, const wchar_t *Label );
	bool ColorPicker12( Color_t &Var, const wchar_t *Label );
	bool ColorPicker13( Color_t &Var, const wchar_t *Label );
	bool ColorPicker14( Color_t &Var, const wchar_t *Label );
	bool ColorPicker15( Color_t &Var, const wchar_t *Label );
	bool ColorPicker16( Color_t &Var, const wchar_t *Label );
	bool PriorityCombo( int _x, int &Var, const std::vector<CVar<int>> &List );
	void DrawFirstWindow( );

private:
	bool m_bReopened = false;
	std::wstring m_szCurTip = L"";

public:
	float m_flFadeAlpha = 0.0f;
	float m_flFadeElapsed = 0.0f;
	float m_flFadeDuration = 0.1f;

public:
	bool m_bOpen = false;
	bool m_bTyping = false;
	void Render();
	void PlayerList();
};

inline CMenu g_Menu;