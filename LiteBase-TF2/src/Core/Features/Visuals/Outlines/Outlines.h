#pragma once
#include "../../../SDK/SDK.h"
#include "../Models/Models.h"
#include "../../Vars.h"

class COutlines
{
private:
	ITexture *m_pRtFullFrame;
	ITexture *m_pRenderBuffer1;
	ITexture *m_pRenderBuffer2;
	IMaterial *m_pMatBlurXwf;
	IMaterial *m_pMatBlurX;
	IMaterial *m_pMatBlurYwf;
	IMaterial *m_pMatBlurY;
	IMaterial *m_pMatHaloAddToScreen;

	struct GlowEnt_t
	{
		IClientEntity *m_pEntity;
		Color_t m_Color;
		float m_flAlpha;
		bool viewmodel;
	};

	std::vector<GlowEnt_t> m_vecGlowEntities;
	std::map<IClientEntity *, bool> m_DrawnEntities;

private:
	void DrawModel( IClientEntity *pEntity, int nFlags, bool bIsDrawingModels );
	void SetScale( int nScale, bool bReset );
	bool ShouldRun( );
	void DeleteMaterials( );

public:
	void Init( );
	void Outline( CTFPlayer* pLocal, IMatRenderContext* pRenderContext );
	void Render( );
	void CreateMaterials( );

	bool HasDrawn( IClientEntity *pEntity )
	{
		return m_DrawnEntities.find( pEntity ) != m_DrawnEntities.end( );
	}

	bool IsGlowMaterial( IMaterial *pMat )
	{
		return pMat == m_pMatGlowColor;
	}

public:
	IMaterial *m_pMatGlowColor;
	bool m_bDrawingGlow;
	bool m_bRendering;
	bool bDrawViewmodel = false;
	bool bColoringViewmodel = false;
	bool bSetup;
};

inline COutlines Outlines;