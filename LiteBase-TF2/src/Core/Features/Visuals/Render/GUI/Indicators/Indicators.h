#pragma once
#include "../../../../../SDK/SDK.h"
#include "../../../../../SDK/Include/TF2/globalinfo.h"
#include "../../../../Exploits/CritHack/CritHack.h"
#include "../../../../Exploits/Backtrack/Backtrack.h"
#include "RawData.h"

class CIndicators
{
private:
	struct Spectator_t
	{
		std::wstring observer;
		std::wstring observed;
		Color_t color;
	};

	int m_nNextIndicatorHeight = 488;
	std::vector<Spectator_t>m_Spectators;
	BYTE iAlpha = g_GlobalInfo.m_nShifted ? 255 : 0;
	float charge = 0;
	float amount = 0.1f;
public:
	

	void DrawDT( );
	void DrawCrits( );
	void DrawInfo( );
	void DrawBT( );
	void DrawAA( );
	bool GetSpectators( CTFPlayer* pLocal );
	void DrawSpectators( );
	void Run( );
};
inline CIndicators g_Indicators;