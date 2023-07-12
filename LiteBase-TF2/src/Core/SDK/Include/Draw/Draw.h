#pragma once
#include "../TF2/TF2.h"
#include "../TF2/icons.h"
#include "../TF2/chudtexture.h"

#define TXT_DEFAULT		(1 << 0)
#define TXT_LEFT		(1 << 1)
#define TXT_TOP			(1 << 2)
#define TXT_CENTERX		(1 << 3)
#define TXT_CENTERY		(1 << 4)
#define TXT_CENTERXY	TXT_CENTERX | TXT_CENTERY

class CFont
{
public:
	const char *m_szName;
	int m_nTall, m_nWeight, m_nFlags;
	DWORD m_dwFont;
};

enum class EFonts {
	DEBUG,
	MENU,
	HARPOON,
	WATERMARK,
	CONDS,
	WEAPON,
	IND,
	NAME,
	CHEAPNAME,
	CHEAPCONDS,
	CHEAPWEAPON,
	RIJIN,
	RIJIN2
};

enum EStringAlign
{
	ALIGN_DEFAULT,
	ALIGN_CENTER,
	ALIGN_CENTERVERTICAL,
	ALIGN_CENTERHORIZONTAL,
	ALIGN_REVERSE
};

inline std::map<EFonts, CFont> g_Fonts;

class CDraw
{
public:
	void ReloadFonts();
	void UpdateScreenSize();
	void UpdateW2SMatrix();
	std::vector<int> m_vecCreatedTextures = { };

public:
	int m_nScreenW = 0, m_nScreenH = 0, m_nScreenC = 0;
	VMatrix m_WorldToProjection;

public:
	void String(const DWORD &font, int x, int y, Color_t clr, short align, const char *str, ...);
	void String(const DWORD &font, int x, int y, Color_t clr, short align, const wchar_t *str, ...);
	void OutlinedString( const DWORD &font, int x, int y, Color_t clr, short align, const char *str, ... );
	void OutlinedString(const DWORD &font, int x, int y, Color_t clr, short align, const wchar_t *str, ...);
	void Line(int x, int y, int x1, int y1, Color_t clr);
	void DrawTexturedPolygon( int count, Vertex_t *vertices, const Color_t &clr );
	void Triangle(int x, int y, int range, int y1, Color_t clr);
	void DottedLine(int x, int y, int x1, int y1, Color_t clr);
	void Rect(int x, int y, int w, int h, Color_t clr);
	void OutlinedRect(int x, int y, int w, int h, Color_t clr);
	void BlackOutlinedRect(int x, int y, int w, int h, Color_t clr);
	void GradientRect(int x, int y, int x1, int y1, Color_t top_clr, Color_t bottom_clr, bool horizontal);
	void GradientRectWH(int x, int y, int w, int h, Color_t top_clr, Color_t bottom_clr, bool horizontal);
	void OutlinedCircle( int x, int y, float radius, int segments, const Color_t &clr );
	void FilledCircle( const int &x, const int &y, const float &radius, const int &segments, const Color_t &clr );
	void Texture(int x, int y, int w, int h, const Color_t& clr, int nIndex);
	void DrawHudTextureByName(float x0, float y0, float s0, const char* textureName, Color_t col0);
	void DrawHudTexture(float x0, float y0, float s0, CHudTexture* texture, Color_t col0);
	int CreateTextureFromArray( const unsigned char *array, int width, int height );
	void DrawTextureRect( int x, int y, int w, int h, int texture, Color_t clr );
	void DeleteTexturesFromMemory( );
	float EaseOut(float start, float end, float speed);
	float EaseIn(float start, float end, float speed);
	float Linear(float start, float end, float speed);
	CHudTexture* GetIcon(const char* szIcon, int eIconFormat = 0);
	bool Timer();
};

inline CDraw g_Draw;