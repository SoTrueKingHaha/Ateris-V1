#pragma once
#include "../../../Utils/Color/Color.h"

typedef struct wrect_s
{
	int	left; //[0]
	int right; //[1]
	int top; //[2]
	int bottom; //[3]
} wrect_t;


class CHudTexture
{
public:
	CHudTexture();
	CHudTexture& operator =(const CHudTexture& src);
	virtual ~CHudTexture();

	int Width() const
	{
		return rc.right - rc.left;
	}

	int Height() const
	{
		return rc.bottom - rc.top;
	}

	// causes the font manager to generate the glyph, prevents run time hitches on platforms that have slow font managers
	void Precache(void);

	// returns width & height of icon with scale applied (scale is ignored if font is used to render)
	int EffectiveWidth(float flScale) const;
	int EffectiveHeight(float flScale) const;

	void DrawSelf(int x, int y, const Color_t& clr) const;
	void DrawSelf(int x, int y, int w, int h, const Color_t& clr) const;
	void DrawSelfCropped(int x, int y, int cropx, int cropy, int cropw, int croph, Color_t clr) const;
	// new version to scale the texture over a finalWidth and finalHeight passed in
	void DrawSelfCropped(int x, int y, int cropx, int cropy, int cropw, int croph, int finalWidth, int finalHeight, Color_t clr) const;

	char		szShortName[64];
	char		szTextureFile[64];

	bool		bRenderUsingFont;
	bool		bPrecached;
	char		cCharacterInFont;
	unsigned long		hFont;

	// vgui texture Id assigned to this item
	int			textureId;
	// s0, t0, s1, t1
	float		texCoords[4];

	// Original bounds
	wrect_t		rc;
};