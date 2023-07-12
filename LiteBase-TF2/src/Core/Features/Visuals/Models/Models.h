#pragma once
#include "../../Vars.h"
#include "../Colors.h"
#include "../../../SDK/SDK.h"
#include "../../../Utils/Utils.h"
#include "../../../Utils/Hash/Hash.h"

class IModels
{
private:
	virtual bool ShouldRun( ) = 0;

	virtual void DrawModel( IClientEntity *pEntity, bool overlayMat, Color_t color, float flAlpha ) = 0;
	virtual void Override( CTFPlayer *pLocal , IMatRenderContext *pRenderContext ) = 0;

public:
	virtual void Init( ) = 0;
	virtual void Render( ) = 0;
	virtual bool HasDrawn( IClientEntity *pEntity ) = 0;
	virtual bool IsChamsMaterial( IMaterial *pMat ) = 0;
};

struct ChamInfo {
	int $flags = 0;
	int $flags_defined = 0;
	int $flags2 = 0;
	int $flags_defined2 = 0;
	int $frame = 0;
};

class __Models : public IModels
{
private:
	IMaterial *m_pMatFlat;
	IMaterial *m_pMatShaded;
	IMaterial *m_pMatFresnelHDR0;
	IMaterial *m_pMatFresnelHDR1;
	IMaterial *m_pMatFresnel;
	IMaterial *m_pMatOverlay;

	IMaterial *m_pMatPlayer;
	IMaterial *m_pMatBuilding;
	IMaterial *m_pMatWorld;
	std::map<IClientEntity * , bool> m_DrawnEntities;

	virtual bool ShouldRun( ) override;

	virtual void DrawModel( IClientEntity *pEntity, bool overlayMat, Color_t color, float flAlpha ) override;
	virtual void Override( CTFPlayer *pLocal , IMatRenderContext *pRenderContext ) override;

public:
	//IMaterial *overlaymaterial;
	virtual void Init( ) override;
	IMaterial *CreateNRef( char const *szName, void *pKV, bool bSave = true );
	virtual void Render( ) override;
	void ValidateMaterial( IMaterial *mTarget );
	void DeleteMaterials( );

	virtual bool HasDrawn( IClientEntity *pEntity ) override
	{
		return m_DrawnEntities.find( pEntity ) != m_DrawnEntities.end( );
	}
	virtual bool IsChamsMaterial( IMaterial *pMat ) override
	{
		return ( pMat == m_pMatFlat || pMat == m_pMatOverlay );
	}

	bool m_bRendering;
	bool m_bHasSetStencil;
	bool bSetup = false;
	int iInitCount, iValidCount, iDelCount;
	std::vector<IMaterial *>matList = { };
	std::vector<IMaterial *>matListGlobal = { };
	std::unordered_map<IMaterial *, ChamInfo> backupInformation;
};

inline __Models Models;