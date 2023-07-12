#include "Draw.h"
//#include "../../../Features/Vars.h"

void CDraw::ReloadFonts()
{
	g_Fonts[EFonts::DEBUG] = { "Small Fonts", 11, 0, FONTFLAG_OUTLINE };
	//g_Fonts[EFonts::NAME] = { "Small Fonts", 11, 0, FONTFLAG_OUTLINE };
	g_Fonts[EFonts::NAME] = { "Tahoma", 12, 600 };
	g_Fonts[EFonts::CHEAPNAME] = { "Tahoma", 12, 600, FONTFLAG_DROPSHADOW };
	g_Fonts[EFonts::MENU] = { "Tahoma", 12, 600, FONTFLAG_OUTLINE };
	g_Fonts[EFonts::WATERMARK] = { "Tahoma", 12, 0, FONTFLAG_OUTLINE };
	g_Fonts[EFonts::CONDS] = { "Small Fonts", 10, 0, };
	g_Fonts[EFonts::CHEAPCONDS] = { "Small Fonts", 10, 0, FONTFLAG_DROPSHADOW };
	g_Fonts[EFonts::WEAPON] = { "Small Fonts", 8, 0, };
	g_Fonts[EFonts::CHEAPWEAPON] = { "Small Fonts", 8, 0, FONTFLAG_DROPSHADOW };
	g_Fonts[EFonts::HARPOON] = { "Lucida Console", 10, 0, FONTFLAG_OUTLINE };
	//g_Fonts[EFonts::IND] = { "Verdana", 28, 700, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS };
	g_Fonts[EFonts::IND] = { "Tahoma", 14, 0, };

	for (auto &v : g_Fonts)
		I::Surface->SetFontGlyphSet(v.second.m_dwFont = I::Surface->CreateFont(), v.second.m_szName, v.second.m_nTall, v.second.m_nWeight, 0, 0, v.second.m_nFlags);
}

void CDraw::UpdateScreenSize()
{
	I::EngineClient->GetScreenSize( this->m_nScreenW, this->m_nScreenH );
	this->m_nScreenC = ( this->m_nScreenW / 2 );
}

void CDraw::UpdateW2SMatrix()
{
	CViewSetup ViewSetup = {};

	if (I::BaseClientDLL->GetPlayerView(ViewSetup)) {
		VMatrix WorldToView = {}, ViewToProjection = {}, WorldToPixels = {};
		I::RenderView->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &g_Draw.m_WorldToProjection, &WorldToPixels);
	}
}

void CDraw::String(const DWORD &font, int x, int y, Color_t clr, short align, const char *str, ...)
{
	if (str == 0)
		return;

	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%hs", cbuffer);

	switch (align)
	{
	case ALIGN_DEFAULT: break;
	case ALIGN_CENTER: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		x -= (w / 2);
		y -= (h / 2);
		break;
	}
	case ALIGN_CENTERVERTICAL: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		y -= (h / 2);
		break;
	}
	case ALIGN_CENTERHORIZONTAL: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		x -= (w / 2);
		break;
	}
	case ALIGN_REVERSE: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		x -= (w);
		break;
	}
	}

	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawPrintText(wstr, wcslen(wstr));
}

void CDraw::String(const DWORD &font, int x, int y, Color_t clr, short align, const wchar_t *str, ...)
{
	if (str == 0)
		return;

	va_list va_alist;
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vswprintf_s(wstr, str, va_alist);
	va_end(va_alist);

	switch (align)
	{
	case ALIGN_DEFAULT: break;
	case ALIGN_CENTER: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		x -= (w / 2);
		y -= (h / 2);
		break;
	}
	case ALIGN_CENTERVERTICAL: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		y -= (h / 2);
		break;
	}
	case ALIGN_CENTERHORIZONTAL: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		x -= (w / 2);
		break;
	}
	case ALIGN_REVERSE: {
		int w = 0, h = 0;
		I::Surface->GetTextSize(font, wstr, w, h);
		x -= (w);
		break;
	}
	}

	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawPrintText(wstr, wcslen(wstr));
}

void CDraw::OutlinedString( const DWORD &font, int x, int y, Color_t clr, short align, const char *str, ... )
{
	if ( str == 0 )
		return;

	va_list va_alist;
	char cbuffer[ 1024 ] = { '\0' };
	wchar_t wstr[ 1024 ] = { '\0' };

	va_start( va_alist, str );
	vsprintf_s( cbuffer, str, va_alist );
	va_end( va_alist );

	wsprintfW( wstr, L"%hs", cbuffer );

	switch ( align )
	{
		case ALIGN_DEFAULT: break;
		case ALIGN_CENTER: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			x -= ( w / 2 );
			y -= ( h / 2 );
			break;
		}
		case ALIGN_CENTERVERTICAL: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			y -= ( h / 2 );
			break;
		}
		case ALIGN_CENTERHORIZONTAL: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			x -= ( w / 2 );
			break;
		}
		case ALIGN_REVERSE: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			x -= ( w );
			break;
		}
	}

	I::Surface->DrawSetTextFont( font );
	//outline
	{
		//left
		I::Surface->DrawSetTextPos( x - 1, y );
		I::Surface->DrawSetTextColor( 0, 0, 0, 255 );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
		//right
		I::Surface->DrawSetTextPos( x + 1, y );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
		//top
		I::Surface->DrawSetTextPos( x, y - 1 );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
		//bottom
		I::Surface->DrawSetTextPos( x, y + 1 );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
	}
	//text
	{
		I::Surface->DrawSetTextPos( x, y );
		I::Surface->DrawSetTextColor( clr.r, clr.g, clr.b, clr.a );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
	}
}

void CDraw::OutlinedString( const DWORD &font, int x, int y, Color_t clr, short align, const wchar_t *str, ... )
{
	if ( str == 0 )
		return;

	va_list va_alist;
	wchar_t wstr[ 1024 ] = { '\0' };

	va_start( va_alist, str );
	vswprintf_s( wstr, str, va_alist );
	va_end( va_alist );

	switch ( align )
	{
		case ALIGN_DEFAULT: break;
		case ALIGN_CENTER: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			x -= ( w / 2 );
			y -= ( h / 2 );
			break;
		}
		case ALIGN_CENTERVERTICAL: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			y -= ( h / 2 );
			break;
		}
		case ALIGN_CENTERHORIZONTAL: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			x -= ( w / 2 );
			break;
		}
		case ALIGN_REVERSE: {
			int w = 0, h = 0;
			I::Surface->GetTextSize( font, wstr, w, h );
			x -= ( w );
			break;
		}
	}

	I::Surface->DrawSetTextFont( font );
	//outline
	{
		//left
		I::Surface->DrawSetTextPos( x - 1, y );
		I::Surface->DrawSetTextColor( 0, 0, 0, 255 );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
		//right
		I::Surface->DrawSetTextPos( x + 1, y );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
		//top
		I::Surface->DrawSetTextPos( x, y - 1 );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
		//bottom
		I::Surface->DrawSetTextPos( x, y + 1 );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
	}
	//text
	{
		I::Surface->DrawSetTextPos( x, y );
		I::Surface->DrawSetTextColor( clr.r, clr.g, clr.b, clr.a );
		I::Surface->DrawPrintText( wstr, wcslen( wstr ) );
	}
}

void CDraw::Line(int x, int y, int x1, int y1, Color_t clr)
{
	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawLine(x, y, x1, y1);
}

void CDraw::DrawTexturedPolygon( int count, Vertex_t *vertices, const Color_t &clr )
{
	static int iTexture = I::Surface->CreateNewTextureID( true );

	I::Surface->DrawSetColor( clr.r, clr.g, clr.b, clr.a );
	I::Surface->DrawSetTexture( iTexture );

	I::Surface->DrawTexturedPolygon( count, vertices );
}

void CDraw::DottedLine(int x, int y, int x1, int y1, Color_t clr)
{
	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawLine(x, y, x1, y1);
	I::Surface->DrawLine(x, y, x - 20, y + 10);
}

void CDraw::Triangle(int x, int y, int range, int y1, Color_t clr)
{
	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawLine(x, y, x + range, y1);
	I::Surface->DrawLine(x, y, x - range, y1);
	I::Surface->DrawLine(x - range, y1, x + range + 1, y1);
}

void CDraw::Rect(int x, int y, int w, int h, Color_t clr)
{
	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void CDraw::OutlinedRect(int x, int y, int w, int h, Color_t clr)
{
	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void CDraw::BlackOutlinedRect(int x, int y, int w, int h, Color_t clr)
{
	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
	I::Surface->DrawSetColor({0, 0, 0, 255});
	I::Surface->DrawOutlinedRect(x - 1, y - 1, x + w + 1, y + h + 1);
	I::Surface->DrawOutlinedRect(x + 1, y + 1, x + w - 1, y + h - 1);
}

void CDraw::GradientRect(int x, int y, int x1, int y1, Color_t top_clr, Color_t bottom_clr, bool horizontal)
{
	I::Surface->DrawSetColor(top_clr.r, top_clr.g, top_clr.b, top_clr.a);
	I::Surface->DrawFilledRectFade(x, y, x1, y1, 255, 255, horizontal);

	I::Surface->DrawSetColor(bottom_clr.r, bottom_clr.g, bottom_clr.b, bottom_clr.a);
	I::Surface->DrawFilledRectFade(x, y, x1, y1, 0, 255, horizontal);
}

void CDraw::GradientRectWH( int x, int y, int w, int h, Color_t top_clr, Color_t bottom_clr, bool horizontal )
{
	I::Surface->DrawSetColor( top_clr.r, top_clr.g, top_clr.b, top_clr.a );
	I::Surface->DrawFilledRectFade( x, y, x + w, y + h, 255, 255, horizontal );
	I::Surface->DrawSetColor( bottom_clr.r, bottom_clr.g, bottom_clr.b, bottom_clr.a );
	I::Surface->DrawFilledRectFade( x, y, x + w, y + h, 0, 255, horizontal );
}

void CDraw::OutlinedCircle( int x, int y, float radius, int segments, const Color_t &clr )
{
	float Step = PI * 2.0 / segments;

	for ( float a = 0; a < ( PI * 2.0 ); a += Step )
	{
		float x1 = radius * cos( a ) + x;
		float y1 = radius * sin( a ) + y;
		float x2 = radius * cos( a + Step ) + x;
		float y2 = radius * sin( a + Step ) + y;
		Line( x1, y1, x2, y2, clr );
	}
}

void CDraw::FilledCircle( const int &x, const int &y, const float &radius, const int &segments, const Color_t &clr )
{
	CUtlVector<Vertex_t> vecVertixes;

	const float flStep = ( 6.28318530718f / static_cast< float >( segments ) );

	for ( float n = 0.0f; n < 6.28318530718f; n += flStep )
		vecVertixes.AddToTail( Vertex_t( { ( static_cast< float >( radius ) * cos( n ) + x ), ( static_cast< float >( radius ) * sinf( n ) + y ) }, { 0.0f, 0.0f } ) );

	if ( vecVertixes.Count( ) > 0 )
	{
		I::Surface->DrawSetColor( clr.r, clr.g, clr.b, clr.a );
		I::Surface->DrawTexturedPolygon( segments, vecVertixes.Base( ) );
	}
}

void CDraw::Texture(int x, int y, int w, int h, const Color_t& clr, int nIndex)
{
	static int nTexture = 0;

	if (ICONS::ID[nIndex] != ICONS::UNDEFINED)
	{
		nTexture = ICONS::ID[nIndex];
	}
	else
	{
		nTexture = I::Surface->CreateNewTextureID();
		I::Surface->DrawSetTextureFile(nTexture, ICONS::TEXTURE[nIndex].c_str(), false, true);
		ICONS::ID[nIndex] = nTexture;
	}

	I::Surface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::Surface->DrawSetTexture(nTexture);
	I::Surface->DrawTexturedRect(x, y, w, h);
}

CHudTexture* CDraw::GetIcon(const char* szIcon, int eIconFormat /* = 0*/)
{
	using fn = CHudTexture * (__stdcall*)(const char*, int);
	static fn GetIconFn = reinterpret_cast<fn>(g_Pattern.Find(L"client.dll", L"55 8B EC 81 EC ? ? ? ? 83 7D 0C ? 56"));
	return GetIconFn(szIcon, eIconFormat);
}

void CDraw::DrawHudTexture(float x0, float y0, float s0, CHudTexture* texture, Color_t col0)
{
	if (!texture)
		return;

	if (texture->bRenderUsingFont)
	{

		I::Surface->DrawSetTextFont(texture->hFont);
		I::Surface->DrawSetTextColor(col0.r, col0.g, col0.b, col0.a);
		I::Surface->DrawSetTextPos(x0, y0);
		I::Surface->DrawUnicodeChar(texture->cCharacterInFont);
	}
	else if (texture->textureId != -1)
	{
		I::Surface->DrawSetTexture(texture->textureId);
		I::Surface->DrawSetColor(col0.r, col0.g, col0.b, col0.a);
		I::Surface->DrawTexturedSubRect(x0, y0, x0 + (texture->rc.right - texture->rc.left) * s0, y0 + (texture->rc.bottom - texture->rc.top) * s0, texture->texCoords[0], texture->texCoords[1], texture->texCoords[2], texture->texCoords[3]);
	}
}

void CDraw::DrawHudTextureByName(float x0, float y0, float s0, const char* textureName, Color_t col0)
{
	CHudTexture* pIcon = GetIcon(textureName, 0);

	if (!pIcon)
		return;

	if (pIcon->bRenderUsingFont)
	{

		I::Surface->DrawSetTextFont(pIcon->hFont);
		I::Surface->DrawSetTextColor(col0.r, col0.g, col0.b, col0.a);
		I::Surface->DrawSetTextPos(x0, y0);
		I::Surface->DrawUnicodeChar(pIcon->cCharacterInFont);
	}
	else if (pIcon->textureId != -1)
	{
		I::Surface->DrawSetTexture(pIcon->textureId);
		I::Surface->DrawSetColor(col0.r, col0.g, col0.b, col0.a);
		I::Surface->DrawTexturedSubRect(x0, y0, x0 + (pIcon->rc.right - pIcon->rc.left) * s0, y0 + (pIcon->rc.bottom - pIcon->rc.top) * s0, pIcon->texCoords[0], pIcon->texCoords[1], pIcon->texCoords[2], pIcon->texCoords[3]);
	}
}

int CDraw::CreateTextureFromArray( const unsigned char *array, int width, int height )
{
	int iTextureOut = I::Surface->CreateNewTextureID( true );
	I::Surface->DrawSetTextureRGBAEx( iTextureOut, array, width, height, IMAGE_FORMAT_BGRA8888 );
	m_vecCreatedTextures.push_back( iTextureOut );
	return iTextureOut;
}

void CDraw::DrawTextureRect( int x, int y, int w, int h, int texture, Color_t clr )
{
	I::Surface->DrawSetColor( { clr.r, clr.g, clr.b, clr.a } );
	I::Surface->DrawSetTexture( texture );
	I::Surface->DrawTexturedRect( x, y, x + w, y + h );
}

void CDraw::DeleteTexturesFromMemory( )
{
	for ( const auto &texture : m_vecCreatedTextures )
	{
		I::Surface->DeleteTextureByID( texture );
	}
}

float CDraw::EaseOut(float start, float end, float speed)
{
	if (start > end || speed <= 1)
		return end;

	if (Timer())
	{
		if (start < end)
			return start * speed;
		return end;
	}
	return start;
}

float CDraw::EaseIn(float start, float end, float speed)
{
	if (start < end || speed >= 1)
		return end;

	if (Timer())
	{
		if (start > end)
			return start * speed;
		return end;
	}
	return start;
}

float CDraw::Linear(float start, float end, float speed)
{
	if (start < end)
		return start + speed;
	return end;
}

bool CDraw::Timer() // This is to make sure that the animations don't get calculated in a split-second
{
	int t = clock();
	static int i = 0;

	if (t > i)
	{
		i += 1;
		return true;
	}
	return false;
}