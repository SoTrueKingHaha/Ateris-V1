#include "Outlines.h"
#include "../../../SDK/Include/EntityCache/EntityCache.h"

bool COutlines::ShouldRun( )
{
	if ( I::EngineVGui->IsGameUIVisible( ) || !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) )
		return false;

	return true;
}

void COutlines::DrawModel( IClientEntity *pEntity, int nFlags, bool bIsDrawingModels )
{
	m_bRendering = true;

	if ( !bIsDrawingModels )
		m_bDrawingGlow = true;

	pEntity->DrawModel( nFlags );

	if ( bIsDrawingModels )
		m_DrawnEntities[ pEntity ] = true;

	if ( !bIsDrawingModels )
		m_bDrawingGlow = false;

	m_bRendering = false;
}

void COutlines::SetScale( int nScale, bool bReset = false )
{
	static IMaterialVar *pVar = nullptr;
	static bool bFound = false;

	if ( bReset ) { pVar = nullptr; bFound = false; return; }

	if ( !bFound && m_pMatBlurY )
	{
		pVar = m_pMatBlurY->FindVar( ( "$bloomamount" ), &bFound );
	}

	else if ( pVar )
	{
		pVar->SetIntValue( nScale );
	}
}

void COutlines::Init( )
{
	//CreateMaterials( );
	m_pMatGlowColor = I::MaterialSystem->FindMaterial( ( "dev/glow_color" ), "Other textures" );
	m_pMatGlowColor->IncrementReferenceCount( );

	m_pRtFullFrame = I::MaterialSystem->FindTexture( ( "_rt_FullFrameFB" ), "RenderTargets" );
	m_pRtFullFrame->IncrementReferenceCount( );

	m_pRenderBuffer1 = I::MaterialSystem->CreateNamedRenderTargetTextureEx(
		( "glow_buffer_1" ), m_pRtFullFrame->GetActualWidth( ), m_pRtFullFrame->GetActualHeight( ),
		RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED,
		TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA, 0x1 );
	m_pRenderBuffer1->IncrementReferenceCount( );

	// This can cause a crash on inject for some reason

	m_pRenderBuffer2 = I::MaterialSystem->CreateNamedRenderTargetTextureEx(
		( "glow_buffer_2" ), m_pRtFullFrame->GetActualWidth( ), m_pRtFullFrame->GetActualHeight( ),
		RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED,
		TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA, 0x1 );
	m_pRenderBuffer2->IncrementReferenceCount( );

	/*static auto m_pMatBlurXkv = new KeyValues( "BlurFilterX" );
	static auto m_pMatBlurXwfkv = new KeyValues( "BlurFilterX" );
	static auto m_pMatBlurYkv = new KeyValues( "BlurFilterY" );
	static auto m_pMatBlurYwfkv = new KeyValues( "BlurFilterY" );
	static auto m_pMatHaloAddToScreenkv = new KeyValues( "UnlitGeneric" );

	m_pMatBlurXkv->SetString( "$basetexture", "glow_buffer_1" );
	m_pMatBlurXwfkv->SetString( "$basetexture", "glow_buffer_1" );
	m_pMatBlurXwfkv->SetBool( "$wireframe", true );

	m_pMatBlurYkv->SetString( "$basetexture", "glow_buffer_2" );
	m_pMatBlurYwfkv->SetString( "$basetexture", "glow_buffer_2" );
	m_pMatBlurYwfkv->SetBool( "$wireframe", true );

	m_pMatHaloAddToScreenkv->SetString( "$basetexture", "glow_buffer_1" );
	m_pMatHaloAddToScreenkv->SetString( "$additive", "1" );

	m_pMatBlurX = Models.CreateNRef( "m_pMatBlurX", m_pMatBlurXkv, false );
	m_pMatBlurXwf = Models.CreateNRef( "m_pMatBlurXwf", m_pMatBlurXwfkv, false );
	m_pMatBlurY = Models.CreateNRef( "m_pMatBlurY", m_pMatBlurYkv, false );
	m_pMatBlurYwf = Models.CreateNRef( "m_pMatBlurYwf", m_pMatBlurYwfkv, false );
	m_pMatHaloAddToScreen = Models.CreateNRef( "m_pMatHaloAddToScreen", m_pMatHaloAddToScreenkv, false );*/

	{
		static auto m_pMatBlurXkv = new KeyValues( "BlurFilterX" );
		m_pMatBlurXkv->SetString( "$basetexture", "glow_buffer_1" );
		m_pMatBlurXkv->SetString( "$wireframe", "0" );
		m_pMatBlurX = Models.CreateNRef( "m_pMatBlurX", m_pMatBlurXkv );
	}

	{
		static auto m_pMatBlurXwfkv = new KeyValues( "BlurFilterX" );
		m_pMatBlurXwfkv->SetString( "$basetexture", "glow_buffer_1" );
		m_pMatBlurXwfkv->SetString( "$wireframe", "1" );
		m_pMatBlurXwf = Models.CreateNRef( "m_pMatBlurXwf", m_pMatBlurXwfkv );
	}

	{
		static auto m_pMatBlurYkv = new KeyValues( "BlurFilterY" );
		m_pMatBlurYkv->SetString( "$basetexture", "glow_buffer_2" );
		m_pMatBlurYkv->SetString( "$wireframe", "0" );
		m_pMatBlurY = Models.CreateNRef( "m_pMatBlurY", m_pMatBlurYkv );
	}

	{
		static auto m_pMatBlurYwfkv = new KeyValues( "BlurFilterY" );
		m_pMatBlurYwfkv->SetString( "$basetexture", "glow_buffer_2" );
		m_pMatBlurYwfkv->SetString( "$wireframe", "1" );
		m_pMatBlurYwf = Models.CreateNRef( "m_pMatBlurYwf", m_pMatBlurYwfkv );
	}

	{
		static auto m_pMatHaloAddToScreenkv = new KeyValues( "UnlitGeneric" );
		m_pMatHaloAddToScreenkv->SetString( "$basetexture", "glow_buffer_1" );
		m_pMatHaloAddToScreenkv->SetString( "$wireframe", "0" );
		m_pMatHaloAddToScreenkv->SetString( "$additive", "1" );
		m_pMatHaloAddToScreen = Models.CreateNRef( "m_pMatHaloAddToScreen", m_pMatHaloAddToScreenkv );
	}
}

void COutlines::CreateMaterials( ) {
	DeleteMaterials( );
	m_pMatGlowColor = I::MaterialSystem->FindMaterial( "dev/glow_color", "Other textures" );
	m_pMatGlowColor->IncrementReferenceCount( );
	KeyValues *m_pMatBlurXkv = new KeyValues( "BlurFilterX" );
	KeyValues *m_pMatBlurXwfkv = new KeyValues( "BlurFilterX" );
	KeyValues *m_pMatBlurYkv = new KeyValues( "BlurFilterY" );
	KeyValues *m_pMatBlurYwfkv = new KeyValues( "BlurFilterY" );
	KeyValues *m_pMatHaloAddToScreenkv = new KeyValues( "UnlitGeneric" );

	m_pMatBlurXkv->SetString( "$basetexture", "glow_buffer_1" );
	m_pMatBlurXwfkv->SetString( "$basetexture", "glow_buffer_1" );
	m_pMatBlurXwfkv->SetString( "$wireframe", "1" );

	m_pMatBlurYkv->SetString( "$basetexture", "glow_buffer_2" );
	m_pMatBlurYwfkv->SetString( "$basetexture", "glow_buffer_2" );
	m_pMatBlurYwfkv->SetString( "$wireframe", "1" );


	m_pMatHaloAddToScreenkv->SetString( "$basetexture", "glow_buffer_1" );
	m_pMatHaloAddToScreenkv->SetString( "$additive", "1" );

	m_pMatBlurX = Models.CreateNRef( "m_pMatBlurX", m_pMatBlurXkv, false );
	m_pMatBlurXwf = Models.CreateNRef( "m_pMatBlurXwf", m_pMatBlurXwfkv, false );
	m_pMatBlurY = Models.CreateNRef( "m_pMatBlurY", m_pMatBlurYkv, false );
	m_pMatBlurYwf = Models.CreateNRef( "m_pMatBlurYwf", m_pMatBlurYwfkv, false );
	m_pMatHaloAddToScreen = Models.CreateNRef( "m_pMatHaloAddToScreen", m_pMatHaloAddToScreenkv, false );

	SetScale( 1.f, true );
}

void COutlines::DeleteMaterials( ) {
	std::vector<IMaterial *> scanMats = {
		m_pMatGlowColor,
		m_pMatBlurXwf,
		m_pMatBlurX,
		m_pMatBlurYwf,
		m_pMatBlurY,
		m_pMatHaloAddToScreen,
	};

	for ( IMaterial *material : scanMats ) {
		if ( !material ) { continue; }
		material->DecrementReferenceCount( );
		material->DeleteIfUnreferenced( );
		material = nullptr;
	}
}

void COutlines::Render( )
{
	if ( !m_vecGlowEntities.empty( ) )
		m_vecGlowEntities.clear( );

	if ( !m_DrawnEntities.empty( ) )
		m_DrawnEntities.clear( );

	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		if ( !ShouldRun( ) || !pLocal )
			return;

		if ( auto pRenderContext = I::MaterialSystem->GetRenderContext( ) )
		{
			Outline( pLocal, pRenderContext );
		}
	}
}

void COutlines::Outline( CTFPlayer *pLocal, IMatRenderContext *pRenderContext )
{
	static ConVar *r_drawviewmodel = g_ConVars.FindVar( "r_drawviewmodel" );

	if ( !Vars::Outline::Main::Enabled.var )
	{
		r_drawviewmodel->SetValue( 1 );
		return;
	}

	if ( !Vars::Outline::Players::ViewmodelOutline.var )
	{
		r_drawviewmodel->SetValue( 1 );
	}

	if ( !ShouldRun( ) )
		return;

	int w = g_Draw.m_nScreenW;
	int h = g_Draw.m_nScreenH;

	if ( w < 1 || h < 1 || w > 4096 || h > 2160 )
		return;

	SetScale( Vars::Outline::Players::OutlineScale.var );

	ShaderStencilState_t StencilStateDisable = {};
	StencilStateDisable.m_bEnable = false;

	float flOriginalColor[ 3 ] = {};
	I::RenderView->GetColorModulation( flOriginalColor );
	float flOriginalBlend = I::RenderView->GetBlend( );

	if ( !Models.m_bHasSetStencil )
	{
		ShaderStencilState_t StencilState = {};
		StencilState.m_bEnable = true;
		StencilState.m_nReferenceValue = 1;
		StencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		StencilState.m_PassOp = STENCILOPERATION_REPLACE;
		StencilState.m_FailOp = STENCILOPERATION_KEEP;
		StencilState.m_ZFailOp = STENCILOPERATION_REPLACE;
		StencilState.m_nTestMask = 0x0;
		StencilState.m_nWriteMask = 0xFF;
		StencilState.SetStencilState( pRenderContext );
	}

	I::RenderView->SetBlend( 1.0f );
	I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

	if ( Vars::Outline::Players::Enabled.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
		{
			const auto &pPlayer = pEntity->As<CTFPlayer>( );

			if ( pPlayer->deadflag( ) || pPlayer->IsDormant( ) )
				continue;

			if ( !Utils::IsOnScreen( pLocal, pPlayer ) )
				continue;

			if ( pPlayer == pLocal )
			{
				continue;
			}

			if ( Vars::Outline::Main::Enemies.var && pPlayer->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				continue;

			Color_t DrawColor = Colors::GetEntityColor( pPlayer, pLocal );

			m_vecGlowEntities.push_back( { pPlayer, DrawColor, Vars::Outline::Players::OutlineAlpha.var, false } );

			if ( !Models.HasDrawn( pPlayer ) )
			{
				DrawModel( pPlayer, STUDIO_RENDER, true );
			}
			if ( Vars::Outline::Players::AttachmentsOutline.var )
			{
				auto attach = GET_ENT_I( *( int * )( ( uintptr_t )pPlayer + 0x1B0 ) & 0xFFF );
				for ( int i = 0; i < 32; i++ )
				{
					if ( !attach )
						continue;

					if ( attach->GetClassId( ) == EClassIds::CTFWearable )
					{
						m_vecGlowEntities.push_back( { attach, DrawColor, Vars::Outline::Players::OutlineAlpha.var, false } );

						if ( !Models.HasDrawn( attach ) )
							DrawModel( attach, STUDIO_RENDER, true );
					}

					attach = GET_ENT_I( *( int * )( ( uintptr_t )attach + 0x1B4 ) & 0xFFF );
				}

				if ( const auto &pWeapon = GET_ENT_H( pPlayer->m_hActiveWeapon( ) )->As<CTFWeaponBase>( ) )
				{
					m_vecGlowEntities.push_back( { pWeapon, DrawColor, Vars::Outline::Players::OutlineAlpha.var, false } );

					if ( !Models.HasDrawn( pWeapon ) )
						DrawModel( pWeapon, STUDIO_RENDER, true );
				}
			}
		}

		if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
		{
			if ( I::Input->CAM_IsThirdPerson( ) )
			{
				Color_t DrawColor = Colors::GetEntityColor( pLocal, pLocal );

				m_vecGlowEntities.push_back( { pLocal, DrawColor, Vars::Outline::Players::OutlineAlpha.var, false } );

				if ( !Models.HasDrawn( pLocal ) )
					DrawModel( pLocal, STUDIO_RENDER, true );

				if ( Vars::Outline::Players::AttachmentsOutline.var )
				{
					auto attach = GET_ENT_I( *( int * )( ( uintptr_t )pLocal + 0x1B0 ) & 0xFFF );
					for ( int i = 0; i < 32; i++ )
					{
						if ( !attach )
							continue;

						if ( attach->GetClassId( ) == EClassIds::CTFWearable )
						{
							m_vecGlowEntities.push_back( { attach, DrawColor, Vars::Outline::Players::OutlineAlpha.var, false } );

							if ( !Models.HasDrawn( attach ) )
								DrawModel( attach, STUDIO_RENDER, true );
						}

						attach = GET_ENT_I( *( int * )( ( uintptr_t )attach + 0x1B4 ) & 0xFFF );
					}

					if ( const auto &pWeapon = g_EntityCache.GetWeapon( ) )
					{
						m_vecGlowEntities.push_back( { pWeapon, DrawColor, Vars::Outline::Players::OutlineAlpha.var, false } );

						if ( !Models.HasDrawn( pWeapon ) )
							DrawModel( pWeapon, STUDIO_RENDER, true );
					}
				}
			}
		}
	}

	/*if ( Vars::Outline::Players::ViewmodelOutline.var )
	{
		if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
		{
			for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_VIEWMODEL ) )
			{
				if ( !pEntity )
					continue;

				if ( !Utils::IsOnScreen( pLocal, pEntity->As<CBaseEntity>( ) ) )
					continue;

				if ( I::Input->CAM_IsThirdPerson( ) )
					continue;

				r_drawviewmodel->SetValue( 0 );

				m_vecGlowEntities.push_back( { pEntity, Vars::Colors::LocalPlayer, Vars::Outline::Players::OutlineAlpha.var, true } );

				if ( !Models.HasDrawn( pEntity ) )
				{
					DrawModel( pEntity, STUDIO_RENDER, true );
				}
			}
		}
	}*/

	if ( Vars::Outline::Buildings::Enabled.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::BUILDINGS_ALL ) )
		{
			const auto &pObject = pEntity->As< CBaseObject >( );

			if ( !pObject || pObject->IsDormant( ) )
				continue;

			if ( !Utils::IsOnScreen( pLocal, pObject ) )
				continue;

			if ( Vars::Outline::Main::Enemies.var && pObject->m_iTeamNum( ) == pLocal->m_iTeamNum( ) && GET_ENT_H( pObject->m_hBuilder( ) ) != pLocal )
				continue;

			Color_t DrawColor = Colors::GetBuildingColor( pObject, pLocal );

			if ( pObject->m_iHealth( ) != 0 )
				m_vecGlowEntities.push_back( { pObject, DrawColor, Vars::Outline::Buildings::OutlineAlpha.var, false } );

			if ( !Models.HasDrawn( pObject ) )
			{
				if ( pObject->m_iHealth( ) != 0 && !pObject->m_bPlacing( ) )
					DrawModel( pObject, STUDIO_RENDER, true );
			}
		}
	}

	if ( Vars::Outline::World::Enabled.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_ALL ) )
		{
			const auto &pAnimated = pEntity->As<CBaseAnimating>( );

			if ( !pAnimated || pAnimated->IsDormant( ) )
				continue;

			if ( pAnimated->GetClassId( ) == EClassIds::CCaptureFlag )
			{
				const auto &pIntel = pAnimated->As<CCaptureFlag>( );

				if ( pIntel->m_nFlagStatus( ) == 1 && GET_ENT_H( pIntel->m_hPrevOwner( ) ) == pLocal )
					continue;
			}

			if ( !Utils::IsOnScreen( pLocal, pAnimated ) )
				continue;

			Color_t DrawColor = Colors::GetWorldColor( pAnimated, pLocal );

			m_vecGlowEntities.push_back( { pAnimated, DrawColor, Vars::Outline::World::OutlineAlpha.var, false } );

			if ( !Models.HasDrawn( pAnimated ) )
				DrawModel( pAnimated, STUDIO_RENDER, true );
		}

		/*for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_NPC ) )
		{
			const auto &pNpc = pEntity->As<CBaseEntity>( );

			if ( !pNpc->IsAlive( ) || pNpc->IsDormant( ) )
				continue;

			if ( !Utils::IsOnScreen( pLocal, pNpc ) )
				continue;

			if ( Vars::Outline::Main::Enemies.var && pNpc->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				continue;

			Color_t DrawColor = Colors::GetColorNPC( pNpc, pLocal );

			m_vecGlowEntities.push_back( { pNpc, DrawColor, Vars::Outline::World::OutlineAlpha.var, false } );

			if ( !Models.HasDrawn( pNpc ) )
				DrawModel( pNpc, STUDIO_RENDER, true );
		}*/
	}

	StencilStateDisable.SetStencilState( pRenderContext );

	if ( m_vecGlowEntities.empty( ) )
		return;

	I::ModelRender->ForcedMaterialOverride( m_pMatGlowColor );

	pRenderContext->PushRenderTargetAndViewport( );
	{
		pRenderContext->SetRenderTarget( m_pRenderBuffer1 );
		pRenderContext->Viewport( 0, 0, w, h );
		pRenderContext->ClearColor4ub( 0, 0, 0, 0 );
		pRenderContext->ClearBuffers( true, false, false );

		for ( const auto &GlowEntity : m_vecGlowEntities )
		{
			I::RenderView->SetBlend( GlowEntity.m_flAlpha );
			I::RenderView->SetColorModulation( Utils::CLR2FLT( GlowEntity.m_Color.r ),
											   Utils::CLR2FLT( GlowEntity.m_Color.g ),
											   Utils::CLR2FLT( GlowEntity.m_Color.b ) );
			/*if ( GlowEntity.viewmodel )
			{
				bColoringViewmodel = true;
			}*/
			float oldinvis = { };
			if ( GlowEntity.m_pEntity->GetClassId( ) == EClassIds::CTFPlayer )
			{
				if ( GlowEntity.m_pEntity->As<CTFPlayer>( )->m_iClass( ) == SPY )
				{
					oldinvis = GlowEntity.m_pEntity->As<CTFPlayer>( )->m_flInvisibility( );
					GlowEntity.m_pEntity->As<CTFPlayer>( )->SetInvisibility( 0.0f );
				}
			}
			DrawModel( GlowEntity.m_pEntity, STUDIO_RENDER, false );
			if ( GlowEntity.m_pEntity->GetClassId( ) == EClassIds::CTFPlayer )
			{
				if ( GlowEntity.m_pEntity->As<CTFPlayer>( )->m_iClass( ) == SPY )
				{
					GlowEntity.m_pEntity->As<CTFPlayer>( )->SetInvisibility( oldinvis );
				}
			}
			/*if ( GlowEntity.viewmodel )
			{
				bColoringViewmodel = false;
			}*/
		}

		StencilStateDisable.SetStencilState( pRenderContext );
	}
	pRenderContext->PopRenderTargetAndViewport( );

	if ( Vars::Outline::Main::BlurOutline.var ) {
		pRenderContext->PushRenderTargetAndViewport( );
		{
			pRenderContext->Viewport( 0, 0, w, h );

			pRenderContext->SetRenderTarget( m_pRenderBuffer2 );
			pRenderContext->DrawScreenSpaceRectangle( m_pMatBlurX, 0,
													  0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
			//pRenderContext->DrawScreenSpaceRectangle(m_pMatBlurX, 0, 0, w, h, 0.0f, 0.0f, w, h, w, h);

			pRenderContext->SetRenderTarget( m_pRenderBuffer1 );
			pRenderContext->DrawScreenSpaceRectangle( m_pMatBlurY, 0,
													  0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
			//pRenderContext->DrawScreenSpaceRectangle(m_pMatBlurY, 0, 0, w, h, 0.0f, 0.0f, w, h, w, h);
		}
		pRenderContext->PopRenderTargetAndViewport( );
	}

	ShaderStencilState_t StencilState = {};
	StencilState.m_bEnable = true;
	StencilState.m_nWriteMask = 0x0;
	StencilState.m_nTestMask = 0xFF;
	StencilState.m_nReferenceValue = 0;
	StencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
	StencilState.m_PassOp = STENCILOPERATION_KEEP;
	StencilState.m_FailOp = STENCILOPERATION_KEEP;
	StencilState.m_ZFailOp = STENCILOPERATION_KEEP;
	StencilState.SetStencilState( pRenderContext );

	/*if ( bDrawViewmodel )
	{
		bColoringViewmodel = true;
	}*/

	if ( Vars::Outline::Main::WireframeOutline.var )
	{
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, -1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, -1, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 0, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 0, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 1, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, -1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
	}
	else if ( Vars::Outline::Main::BlurOutline.var ) {
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
	}

	if ( Vars::Outline::Main::CheapOutline.var )
	{
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, -1, -1, w + 2, h + 2, -2.0f, -2.0f, w + 1, h + 1, w + 2, h + 2 );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, -1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 1, 1, w - 1, h - 1, 2.0f, 2.0f, w, h, w - 1, h - 1 );
	}
	else if ( Vars::Outline::Main::BlurOutline.var ) {
		pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
	}

	/*if ( bDrawViewmodel )
	{
		bColoringViewmodel = false;
	}*/

	StencilStateDisable.SetStencilState( pRenderContext );

	I::ModelRender->ForcedMaterialOverride( nullptr );
	I::RenderView->SetColorModulation( flOriginalColor );
	I::RenderView->SetBlend( flOriginalBlend );

}