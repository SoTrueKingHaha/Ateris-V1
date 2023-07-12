#include "Models.h"
#include "../../../SDK/Include/EntityCache/EntityCache.h"
#include "../Outlines/Outlines.h"

bool __Models::ShouldRun( )
{
	if ( I::EngineVGui->IsGameUIVisible( ) || !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) )
		return false;

	return true;
}

void __Models::DrawModel( IClientEntity *pEntity, bool pOverlayMat, Color_t color, float flAlpha )
{
	m_bRendering = true;
	pEntity->DrawModel( STUDIO_RENDER );
	if ( pOverlayMat )
	{
		if ( IMaterialVar *$phongtint = m_pMatOverlay->FindVar( "$phongtint", nullptr, false ) )
		{
			$phongtint->SetVecValue(
				Utils::CLR2FLT( color.r ),
				Utils::CLR2FLT( color.g ),
				Utils::CLR2FLT( color.b ) );
		}
		if ( IMaterialVar *$envmaptint = m_pMatOverlay->FindVar( "$envmaptint", nullptr, false ) )
		{
			$envmaptint->SetVecValue(
				Utils::CLR2FLT( color.r ),
				Utils::CLR2FLT( color.g ),
				Utils::CLR2FLT( color.b ) );
		}
		if ( IMaterialVar *$phongfresnelranges = m_pMatOverlay->FindVar( "$phongfresnelranges", nullptr, false ) )
		{
			$phongfresnelranges->SetVecValue( 0, 1.f, 2.f );
		}
		m_pMatOverlay->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );

		I::RenderView->SetBlend( flAlpha );
		I::ModelRender->ForcedMaterialOverride( m_pMatOverlay );

		pEntity->DrawModel( STUDIO_RENDER );
	}
	m_DrawnEntities[ pEntity ] = true;
	m_bRendering = false;
}

IMaterial * __Models::CreateNRef( char const *szName, void *pKV, bool bSave )
{
	/*I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "Creating Reference: " );
	I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, szName );
	I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "\n" );*/
	IMaterial *returnMaterial = I::MaterialSystem->CreateMaterial( szName, pKV );
	returnMaterial->IncrementReferenceCount( );

	int $flags{}, $flags_defined{}, $flags2{}, $flags_defined2{};
	if ( auto var = returnMaterial->FindVar( "$flags", nullptr ) )
		$flags = var->GetIntValueInternal( );

	if ( auto var = returnMaterial->FindVar( "$flags_defined", nullptr ) )
		$flags_defined = var->GetIntValueInternal( );

	if ( auto var = returnMaterial->FindVar( "$flags2", nullptr ) )
		$flags2 = var->GetIntValueInternal( );

	if ( auto var = returnMaterial->FindVar( "$flags_defined2", nullptr ) )
		$flags_defined2 = var->GetIntValueInternal( );

	backupInformation[ returnMaterial ] = { $flags, $flags_defined, $flags2, $flags_defined2 };
	matList.push_back( returnMaterial );
	++iInitCount;
	return returnMaterial;
}

void __Models::ValidateMaterial( IMaterial *pMaterial )
{
	/*I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "Validating Material: " );
	I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, pMaterial->GetName( ) );
	I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "\n" );*/
	if ( auto var = pMaterial->FindVar( "$flags", nullptr ) )
		var->SetIntValue( backupInformation[pMaterial ].$flags );

	if ( auto var = pMaterial->FindVar( "$flags_defined", nullptr ) )
		var->SetIntValue( backupInformation[ pMaterial ].$flags_defined );

	if ( auto var = pMaterial->FindVar( "$flags2", nullptr ) )
		var->SetIntValue( backupInformation[ pMaterial ].$flags2 );

	if ( auto var = pMaterial->FindVar( "$flags_defined2", nullptr ) )
		var->SetIntValue( backupInformation[ pMaterial ].$flags_defined2 );
	//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "Finished.\n" );
	++iValidCount;
}

void __Models::DeleteMaterials( )
{
	if ( matList.size( ) == 0U ) { return; }
	for ( IMaterial *material : matList )
	{
		if ( !material ) { continue; }
		/*I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "Deleting Material: " );
		I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, material->GetName( ) );
		I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "\n" );*/
		material->DecrementReferenceCount( );
		material->DeleteIfUnreferenced( );
		material = nullptr;
	}

	//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "Finished.\n" );
	++iDelCount;
	matList.clear( ); bSetup = false;
}

void __Models::Init( )
{
	if ( bSetup ) { return; }

	{
		static auto m_pMatFlatkv = new KeyValues( "UnlitGeneric" );
		m_pMatFlatkv->SetString( "$basetexture", "vgui/white_additive" );
		m_pMatFlatkv->SetString( "$wireframe", "0" );
		m_pMatFlatkv->SetString( "$cloakPassEnabled", "1" );
		m_pMatFlat = Models.CreateNRef( "m_pMatFlat", m_pMatFlatkv );
	}

	{
		static auto m_pMatShadedkv = new KeyValues( "VertexLitGeneric" );
		m_pMatShadedkv->SetString( "$basetexture", "vgui/white_additive" );
		m_pMatShadedkv->SetString( "$bumpmap", "vgui/white_additive" );
		//kv->SetString( "$halflambert", "1" );
		m_pMatShadedkv->SetString( "$selfillum", "1" );
		m_pMatShadedkv->SetString( "$selfillumfresnel", "1" );
		m_pMatShadedkv->SetString( "$selfillumfresnelminmaxexp", "[0 1 1]" );
		m_pMatShadedkv->SetString( "$wireframe", "0" );
		m_pMatShadedkv->SetString( "$cloakPassEnabled", "1" );
		m_pMatShaded = Models.CreateNRef( "m_pMatShaded", m_pMatShadedkv );
	}

	{
		static auto m_pMatFresnelHDR0kv = new KeyValues( "VertexLitGeneric" );
		m_pMatFresnelHDR0kv->SetString( "$basetexture", "vgui/white_additive" );
		m_pMatFresnelHDR0kv->SetString( "$bumpmap", "models/player/shared/shared_normal" );
		//kv->SetString( "$halflambert", "1" );
		m_pMatFresnelHDR0kv->SetString( "$envmap", "skybox/sky_dustbowl_01" );
		m_pMatFresnelHDR0kv->SetString( "$envmapfresnel", "1" );
		m_pMatFresnelHDR0kv->SetString( "$phong", "1" );
		m_pMatFresnelHDR0kv->SetString( "$phongfresnelranges", "[0 1.5 2]" );
		m_pMatFresnelHDR0kv->SetString( "$selfillum", "1" );
		m_pMatFresnelHDR0kv->SetString( "$selfillumfresnel", "1" );
		m_pMatFresnelHDR0kv->SetString( "$selfillumfresnelminmaxexp", "[0.5 0.5 0]" );
		m_pMatFresnelHDR0kv->SetString( "$selfillumtint", "[0 0 0]" );
		m_pMatFresnelHDR0kv->SetString( "$envmaptint", "[0 1 0]" );
		m_pMatFresnelHDR0kv->SetString( "$wireframe", "0" );
		m_pMatFresnelHDR0kv->SetString( "$cloakPassEnabled", "1" );
		m_pMatFresnelHDR0 = Models.CreateNRef( "m_pMatFresnelHDR0", m_pMatFresnelHDR0kv );
	}

	{
		static auto m_pMatFresnelHDR1kv = new KeyValues( "VertexLitGeneric" );
		m_pMatFresnelHDR1kv->SetString( "$basetexture", "vgui/white_additive" );
		m_pMatFresnelHDR1kv->SetString( "$bumpmap", "models/player/shared/shared_normal" );
		//kv->SetString( "$halflambert", "1" );
		m_pMatFresnelHDR1kv->SetString( "$envmap", "skybox/sky_dustbowl_01" );
		m_pMatFresnelHDR1kv->SetString( "$envmapfresnel", "1" );
		m_pMatFresnelHDR1kv->SetString( "$phong", "1" );
		m_pMatFresnelHDR1kv->SetString( "$phongfresnelranges", "[0 0.05 0.1]" );
		m_pMatFresnelHDR1kv->SetString( "$selfillum", "1" );
		m_pMatFresnelHDR1kv->SetString( "$selfillumfresnel", "1" );
		m_pMatFresnelHDR1kv->SetString( "$selfillumfresnelminmaxexp", "[0.5 0.5 0]" );
		m_pMatFresnelHDR1kv->SetString( "$selfillumtint", "[0 0 0]" );
		m_pMatFresnelHDR1kv->SetString( "$envmaptint", "[0 1 0]" );
		m_pMatFresnelHDR1kv->SetString( "$wireframe", "0" );
		m_pMatFresnelHDR1kv->SetString( "$cloakPassEnabled", "1" );
		m_pMatFresnelHDR1 = Models.CreateNRef( "m_pMatFresnelHDR1", m_pMatFresnelHDR1kv );
	}

	{
	   static auto m_pMatOverlaykv = new KeyValues( "VertexLitGeneric" );
	   m_pMatOverlaykv->SetString( "$basetexture", "models/player/shared/ice_player" );
	   m_pMatOverlaykv->SetString( "$bumpmap", "models/player/shared/shared_normal" );
	   m_pMatOverlaykv->SetString( "$translucent", "1" );
	   m_pMatOverlaykv->SetString( "$additive", "1" );
	   m_pMatOverlaykv->SetString( "$phong", "1" );
	   m_pMatOverlaykv->SetString( "$phongfresnelranges", "[0 0.5 1]" );
	   m_pMatOverlaykv->SetString( "$phongtint", "[0 0 0]" );
	   m_pMatOverlaykv->SetString( "$envmap", "skybox/sky_dustbowl_01" );
	   m_pMatOverlaykv->SetString( "$envmapfresnel", "1" );
	   m_pMatOverlaykv->SetString( "$envmaptint", "[1 1 1]" );
	   m_pMatOverlaykv->SetString( "$selfillum", "1" );
	   m_pMatOverlaykv->SetString( "$selfillumtint", "[0 0 0]" );
	   //m_pMatOverlaykv->SetString( "$rimlight", "1" );
	   //m_pMatOverlaykv->SetString( "$rimlightboost", "-5" );
	   m_pMatOverlaykv->SetString( "$wireframe", "0" );
	   m_pMatOverlaykv->SetString( "$cloakPassEnabled", "1" );
	   m_pMatOverlay = Models.CreateNRef( "m_pMatOverlay", m_pMatOverlaykv );
	}

	bSetup = true;
}

void __Models::Render( )
{
	if ( !m_DrawnEntities.empty( ) )
		m_DrawnEntities.clear( );

	m_bHasSetStencil = false;

	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		if ( !ShouldRun( ) || !pLocal )
			return;

		if ( const auto &pRenderContext = I::MaterialSystem->GetRenderContext( ) )
		{
			ShaderStencilState_t StencilState = {};
			StencilState.m_bEnable = true;
			StencilState.m_nReferenceValue = 1;
			StencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
			StencilState.m_PassOp = STENCILOPERATION_REPLACE;
			StencilState.m_FailOp = STENCILOPERATION_KEEP;
			StencilState.m_ZFailOp = STENCILOPERATION_REPLACE;
			StencilState.SetStencilState( pRenderContext );
			m_bHasSetStencil = true;

			Override( pLocal , pRenderContext );
		}
	}
}

void __Models::Override( CTFPlayer *pLocal , IMatRenderContext *pRenderContext )
{
	if ( !Vars::Models::Main::Enabled.var )
		return;

	if ( !ShouldRun( ) )
		return;

	bool bMatWasForced = false;

	if ( Vars::Models::Main::Enabled.var )
	{
		switch ( Vars::Models::Players::Model.var )
		{
			case Vars::Models::NONE:
			{
				I::ModelRender->ForcedMaterialOverride( nullptr );
				m_pMatPlayer = nullptr;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::FLAT:
			{
				I::ModelRender->ForcedMaterialOverride( m_pMatFlat );
				m_pMatPlayer = m_pMatFlat;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::SHADED:
			{
				I::ModelRender->ForcedMaterialOverride( m_pMatShaded );
				m_pMatPlayer = m_pMatShaded;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::FRESNEL:
			{
				if ( g_ConVars.FindVar( "mat_hdr_level" )->GetInt( ) > 1 )
					m_pMatFresnel = m_pMatFresnelHDR1;
				else
					m_pMatFresnel = m_pMatFresnelHDR0;

				I::ModelRender->ForcedMaterialOverride( m_pMatFresnel );
				m_pMatPlayer = m_pMatFresnel;
				bMatWasForced = true;
				break;
			}
			default: break;
		}
	}

	if ( !bMatWasForced )
		I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

	if ( Vars::Models::Players::Enabled.var )
	{
		for ( const auto& pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
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

			/*if ( pPlayer->m_nPlayerCond( ) & ( 1 << 4 ) )
				pPlayer->m_nPlayerCond( ) &= ~( 1 << 4 );*/

			if ( Vars::Models::Main::Enemies.var && pPlayer->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				continue;

			if ( Vars::Models::Players::IgnoreZ.var )
				if ( pPlayer != pLocal )
					pRenderContext->DepthRange( 0.0f, 0.2f );

			Color_t PlayerColor = Colors::GetEntityColor( pPlayer, pLocal );
			if ( bMatWasForced )
			{
				if ( Vars::Models::Players::Model.var != 3 )
					I::RenderView->SetColorModulation( PlayerColor );
				else
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

				I::RenderView->SetBlend( Utils::CLR2FLT( PlayerColor.a ) );

				bool foundselfillumtint = false;
				bool foundenvmaptint = false;
				if ( Vars::Models::Players::Model.var == 3 )
				{
					IMaterialVar *fresnelSelfillumtint = m_pMatFresnel->FindVar( "$selfillumtint", &foundselfillumtint );
					if ( foundselfillumtint )
					{
						fresnelSelfillumtint->SetVecValue( Utils::CLR2FLT( Vars::Colors::Fresnel.r ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.g ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.b ) );
					}
					IMaterialVar *fresnelEnvmaptint = m_pMatFresnel->FindVar( "$envmaptint", &foundenvmaptint );
					if ( foundenvmaptint )
					{
						fresnelEnvmaptint->SetVecValue( Utils::CLR2FLT( PlayerColor.r ), Utils::CLR2FLT( PlayerColor.g ), Utils::CLR2FLT( PlayerColor.b ) );
					}
				}
			}

			if ( Vars::Models::Players::Model.var == 0 )
			{
				I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
				I::RenderView->SetBlend( 1.0f );
			}

			float oldinvis;
			if ( pPlayer->m_iClass( ) == SPY )
			{
				oldinvis = pPlayer->m_flInvisibility( );
				pPlayer->SetInvisibility( 0.0f );
			}
			DrawModel( pPlayer, Vars::Models::Players::Overlay.var, PlayerColor, Vars::Models::Players::OverlayAlpha.var );
			if ( Vars::Models::Players::Overlay.var )
			{
				if ( Vars::Models::Players::Model.var == 0 )
				{
					I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
					I::RenderView->SetBlend( 1.0f );
				}
				else
				{
					I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
					I::RenderView->SetColorModulation( Utils::CLR2FLT( PlayerColor.r ),
													   Utils::CLR2FLT( PlayerColor.g ),
													   Utils::CLR2FLT( PlayerColor.b ) );
					I::RenderView->SetBlend( Utils::CLR2FLT( PlayerColor.a ) );
				}
			}
			if ( pPlayer->m_iClass( ) == SPY )
			{
				pPlayer->SetInvisibility( oldinvis );
			}

			auto pAttachment = GET_ENT_I( *( int * )( ( uintptr_t )pPlayer + 0x1B0 ) & 0xFFF );
			for ( int i = 0; i < 32; i++ )
			{
				if ( !pAttachment )
					continue;

				if ( pAttachment->GetClassId( ) == EClassIds::CTFWearable )
				{
					if ( pAttachment->ShouldDraw( ) )
					{
						DrawModel( pAttachment, Vars::Models::Players::Overlay.var, PlayerColor, Vars::Models::Players::OverlayAlpha.var );
						if ( Vars::Models::Players::Overlay.var )
						{
							if ( Vars::Models::Players::Model.var == 0 )
							{
								I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
								I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
								I::RenderView->SetBlend( 1.0f );
							}
							else
							{
								I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
								I::RenderView->SetColorModulation( Utils::CLR2FLT( PlayerColor.r ),
																   Utils::CLR2FLT( PlayerColor.g ),
																   Utils::CLR2FLT( PlayerColor.b ) );
								I::RenderView->SetBlend( Utils::CLR2FLT( PlayerColor.a ) );
							}
						}
					}
				}

				pAttachment = GET_ENT_I( *( int * )( ( uintptr_t )pAttachment + 0x1B4 ) & 0xFFF );
			}

			if ( const auto &pWeapon = GET_ENT_H( pPlayer->m_hActiveWeapon( ) )->As<CTFWeaponBase>( ) )
			{
				if ( pWeapon->ShouldDraw( ) )
				{
					DrawModel( pWeapon, Vars::Models::Players::Overlay.var, PlayerColor, Vars::Models::Players::OverlayAlpha.var );
					if ( Vars::Models::Players::Overlay.var )
					{
						if ( Vars::Models::Players::Model.var == 0 )
						{
							I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
							I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
							I::RenderView->SetBlend( 1.0f );
						}
						else
						{
							I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
							I::RenderView->SetColorModulation( Utils::CLR2FLT( PlayerColor.r ),
															   Utils::CLR2FLT( PlayerColor.g ),
															   Utils::CLR2FLT( PlayerColor.b ) );
							I::RenderView->SetBlend( Utils::CLR2FLT( PlayerColor.a ) );
						}
					}
				}
			}
		}

		if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
		{
			Color_t LocalColor = Colors::GetEntityColor( pLocal, pLocal );

			if ( bMatWasForced )
			{
				if ( Vars::Models::Players::Model.var != 3 )
					I::RenderView->SetColorModulation( LocalColor );
				else
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

				I::RenderView->SetBlend( Utils::CLR2FLT( LocalColor.a ) );

				bool foundselfillumtint = false;
				bool foundenvmaptint = false;
				if ( Vars::Models::Players::Model.var == 3 )
				{
					IMaterialVar *fresnelSelfillumtint = m_pMatFresnel->FindVar( "$selfillumtint", &foundselfillumtint );
					if ( foundselfillumtint )
					{
						fresnelSelfillumtint->SetVecValue( Utils::CLR2FLT( Vars::Colors::Fresnel.r ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.g ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.b ) );
					}
					IMaterialVar *fresnelEnvmaptint = m_pMatFresnel->FindVar( "$envmaptint", &foundenvmaptint );
					if ( foundenvmaptint )
					{
						fresnelEnvmaptint->SetVecValue( Utils::CLR2FLT( LocalColor.r ), Utils::CLR2FLT( LocalColor.g ), Utils::CLR2FLT( LocalColor.b ) );
					}
				}
			}

			if ( Vars::Models::Players::Model.var == 0 )
			{
				I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
				I::RenderView->SetBlend( 1.0f );
			}

			if ( I::Input->CAM_IsThirdPerson( ) )
			{
				DrawModel( pLocal, Vars::Models::Players::Overlay.var, LocalColor, Vars::Models::Players::OverlayAlpha.var );
				if ( Vars::Models::Players::Overlay.var )
				{
					if ( Vars::Models::Players::Model.var == 0 )
					{
						I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
						I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
						I::RenderView->SetBlend( 1.0f );
					}
					else
					{
						I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
						I::RenderView->SetColorModulation( Utils::CLR2FLT( LocalColor.r ),
														   Utils::CLR2FLT( LocalColor.g ),
														   Utils::CLR2FLT( LocalColor.b ) );
						I::RenderView->SetBlend( Utils::CLR2FLT( LocalColor.a ) );
					}
				}
				auto pAttachment = GET_ENT_I( *( int * )( ( uintptr_t )pLocal + 0x1B0 ) & 0xFFF );
				for ( int i = 0; i < 32; i++ )
				{
					if ( !pAttachment )
						continue;

					if ( pAttachment->GetClassId( ) == EClassIds::CTFWearable )
					{
						if ( pAttachment->ShouldDraw( ) )
						{
							DrawModel( pAttachment, Vars::Models::Players::Overlay.var, LocalColor, Vars::Models::Players::OverlayAlpha.var );
							if ( Vars::Models::Players::Overlay.var )
							{
								if ( Vars::Models::Players::Model.var == 0 )
								{
									I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
									I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
									I::RenderView->SetBlend( 1.0f );
								}
								else
								{
									I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
									I::RenderView->SetColorModulation( Utils::CLR2FLT( LocalColor.r ),
																	   Utils::CLR2FLT( LocalColor.g ),
																	   Utils::CLR2FLT( LocalColor.b ) );
									I::RenderView->SetBlend( Utils::CLR2FLT( LocalColor.a ) );
								}
							}
						}
					}

					pAttachment = GET_ENT_I( *( int * )( ( uintptr_t )pAttachment + 0x1B4 ) & 0xFFF );
				}

				if ( const auto &pWeapon = g_EntityCache.GetWeapon( ) )
				{
					if ( pWeapon->ShouldDraw( ) )
					{
						DrawModel( pWeapon, Vars::Models::Players::Overlay.var, LocalColor, Vars::Models::Players::OverlayAlpha.var );
						if ( Vars::Models::Players::Overlay.var )
						{
							if ( Vars::Models::Players::Model.var == 0 )
							{
								I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
								I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
								I::RenderView->SetBlend( 1.0f );
							}
							else
							{
								I::ModelRender->ForcedMaterialOverride( m_pMatPlayer );
								I::RenderView->SetColorModulation( Utils::CLR2FLT( LocalColor.r ),
																   Utils::CLR2FLT( LocalColor.g ),
																   Utils::CLR2FLT( LocalColor.b ) );
								I::RenderView->SetBlend( Utils::CLR2FLT( LocalColor.a ) );
							}
						}
					}
				}
			}
		}

		pRenderContext->DepthRange( 0.0f, 1.0f );
	}

	if ( Vars::Models::Main::Enabled.var)
	{
		switch ( Vars::Models::Buildings::Model.var )
		{
			case Vars::Models::NONE:
			{
				I::ModelRender->ForcedMaterialOverride( nullptr );
				m_pMatBuilding = nullptr;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::FLAT:
			{
				I::ModelRender->ForcedMaterialOverride( m_pMatFlat );
				m_pMatBuilding = m_pMatFlat;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::SHADED:
			{
				I::ModelRender->ForcedMaterialOverride( m_pMatShaded );
				m_pMatBuilding = m_pMatShaded;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::FRESNEL:
			{
				if ( g_ConVars.FindVar( "mat_hdr_level" )->GetInt( ) > 1 )
					m_pMatFresnel = m_pMatFresnelHDR1;
				else
					m_pMatFresnel = m_pMatFresnelHDR0;

				I::ModelRender->ForcedMaterialOverride( m_pMatFresnel );
				m_pMatBuilding = m_pMatFresnel;
				bMatWasForced = true;
				break;
			}
			default: break;
		}
	}

	if ( !bMatWasForced )
		I::RenderView->SetColorModulation( 1.0f , 1.0f , 1.0f );

	if ( Vars::Models::Buildings::Enabled.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::BUILDINGS_ALL ) )
		{
			const auto &pObject = pEntity->As< CBaseObject >( );

			if ( !pObject || pObject->IsDormant( ) )
				continue;

			if ( !Utils::IsOnScreen( pLocal, pObject ) )
				continue;

			if ( Vars::Models::Main::Enemies.var && pObject->m_iTeamNum( ) == pLocal->m_iTeamNum( ) && GET_ENT_H( pObject->m_hBuilder( ) ) != pLocal )
				continue;

			if ( Vars::Models::Buildings::IgnoreZ.var )
				pRenderContext->DepthRange( 0.0f, 0.2f );

			Color_t BuildingColor = Colors::GetBuildingColor( pObject, pLocal );
			if ( bMatWasForced )
			{
				if ( Vars::Models::Buildings::Model.var != 3 )
					I::RenderView->SetColorModulation( BuildingColor );
				else
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

				I::RenderView->SetBlend( Utils::CLR2FLT( BuildingColor.a ) );

				bool foundselfillumtint = false;
				bool foundenvmaptint = false;
				if ( Vars::Models::Buildings::Model.var == 3 )
				{
					IMaterialVar *fresnelSelfillumtint = m_pMatFresnel->FindVar( "$selfillumtint", &foundselfillumtint );
					if ( foundselfillumtint )
					{
						fresnelSelfillumtint->SetVecValue( Utils::CLR2FLT( Vars::Colors::Fresnel.r ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.g ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.b ) );
					}
					IMaterialVar *fresnelEnvmaptint = m_pMatFresnel->FindVar( "$envmaptint", &foundenvmaptint );
					if ( foundenvmaptint )
					{
						fresnelEnvmaptint->SetVecValue( Utils::CLR2FLT( BuildingColor.r ), Utils::CLR2FLT( BuildingColor.g ), Utils::CLR2FLT( BuildingColor.b ) );
					}
				}
			}

			if ( Vars::Models::Buildings::Model.var == 0 )
			{
				I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
				I::RenderView->SetBlend( 1.0f );
			}

			if ( pObject->m_iHealth( ) != 0 && !pObject->m_bPlacing( ) )
			{
				DrawModel( pObject, Vars::Models::Buildings::Overlay.var, BuildingColor, Vars::Models::Buildings::OverlayAlpha.var );
				if ( Vars::Models::Buildings::Overlay.var )
				{
					if ( Vars::Models::Buildings::Model.var == 0 )
					{
						I::ModelRender->ForcedMaterialOverride( m_pMatBuilding );
						I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
						I::RenderView->SetBlend( 1.0f );
					}
					else
					{
						I::ModelRender->ForcedMaterialOverride( m_pMatBuilding );
						I::RenderView->SetColorModulation( Utils::CLR2FLT( BuildingColor.r ),
														   Utils::CLR2FLT( BuildingColor.g ),
														   Utils::CLR2FLT( BuildingColor.b ) );
						I::RenderView->SetBlend( Utils::CLR2FLT( BuildingColor.a ) );
					}
				}
			}

			pRenderContext->DepthRange( 0.0f, 1.0f );
		}
	}

	if ( Vars::Models::Main::Enabled.var )
	{
		switch ( Vars::Models::World::Model.var )
		{
			case Vars::Models::NONE:
			{
				I::ModelRender->ForcedMaterialOverride( nullptr );
				m_pMatWorld = nullptr;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::FLAT:
			{
				I::ModelRender->ForcedMaterialOverride( m_pMatFlat );
				m_pMatWorld = m_pMatFlat;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::SHADED:
			{
				I::ModelRender->ForcedMaterialOverride( m_pMatShaded );
				m_pMatWorld = m_pMatShaded;
				bMatWasForced = true;
				break;
			}
			case Vars::Models::FRESNEL:
			{
				if ( g_ConVars.FindVar( "mat_hdr_level" )->GetInt( ) > 1 )
					m_pMatFresnel = m_pMatFresnelHDR1;
				else
					m_pMatFresnel = m_pMatFresnelHDR0;
					
				I::ModelRender->ForcedMaterialOverride( m_pMatFresnel );
				m_pMatWorld = m_pMatFresnel;
				bMatWasForced = true;
				break;
			}
			default: break;
		}
	}

	if ( !bMatWasForced )
		I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

	if ( Vars::Models::World::Enabled.var )
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

			if ( Vars::Models::World::IgnoreZ.var )
				pRenderContext->DepthRange( 0.0f, 0.2f );

			Color_t WorldColor = Colors::GetWorldColor( pAnimated, pLocal );
			if ( bMatWasForced )
			{
				if ( Vars::Models::World::Model.var != 3 )
					I::RenderView->SetColorModulation( WorldColor );
				else
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

				I::RenderView->SetBlend( Utils::CLR2FLT( WorldColor.a ) );

				bool foundselfillumtint = false;
				bool foundenvmaptint = false;
				if ( Vars::Models::World::Model.var == 3 )
				{
					IMaterialVar *fresnelSelfillumtint = m_pMatFresnel->FindVar( "$selfillumtint", &foundselfillumtint );
					if ( foundselfillumtint )
					{
						fresnelSelfillumtint->SetVecValue( Utils::CLR2FLT( Vars::Colors::Fresnel.r ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.g ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.b ) );
					}
					IMaterialVar *fresnelEnvmaptint = m_pMatFresnel->FindVar( "$envmaptint", &foundenvmaptint );
					if ( foundenvmaptint )
					{
						fresnelEnvmaptint->SetVecValue( Utils::CLR2FLT( WorldColor.r ), Utils::CLR2FLT( WorldColor.g ), Utils::CLR2FLT( WorldColor.b ) );
					}
				}
			}

			if ( Vars::Models::World::Model.var == 0 )
			{
				I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
				I::RenderView->SetBlend( 1.0f );
			}

			DrawModel( pAnimated, Vars::Models::World::Overlay.var, WorldColor, Vars::Models::World::OverlayAlpha.var );
			if ( Vars::Models::World::Overlay.var )
			{
				if ( Vars::Models::World::Model.var == 0 )
				{
					I::ModelRender->ForcedMaterialOverride( m_pMatWorld );
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
					I::RenderView->SetBlend( 1.0f );
				}
				else
				{
					I::ModelRender->ForcedMaterialOverride( m_pMatWorld );
					I::RenderView->SetColorModulation( Utils::CLR2FLT( WorldColor.r ),
													   Utils::CLR2FLT( WorldColor.g ),
													   Utils::CLR2FLT( WorldColor.b ) );
					I::RenderView->SetBlend( Utils::CLR2FLT( WorldColor.a ) );
				}
			}

			pRenderContext->DepthRange( 0.0f, 1.0f );
		}

		/*for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_NPC ) )
		{
			const auto &pNpc = pEntity->As<CBaseEntity>( );

			if ( !pNpc->IsAlive( ) || pNpc->IsDormant( ) )
				continue;

			if ( !Utils::IsOnScreen( pLocal, pNpc ) )
				continue;

			if ( Vars::Models::Main::Enemies.var && pNpc->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				continue;

			if ( Vars::Models::World::IgnoreZ.var )
				pRenderContext->DepthRange( 0.0f, 0.2f );

			Color_t NPCColor = Colors::GetColorNPC( pNpc, pLocal );

			if ( bMatWasForced )
			{
				if ( Vars::Models::World::Model.var != 3 )
					I::RenderView->SetColorModulation( NPCColor );
				else
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

				I::RenderView->SetBlend( Utils::CLR2FLT( NPCColor.a ) );

				bool foundselfillumtint = false;
				bool foundenvmaptint = false;
				if ( Vars::Models::World::Model.var == 3 )
				{
					IMaterialVar *fresnelSelfillumtint = m_pMatFresnel->FindVar( "$selfillumtint", &foundselfillumtint );
					if ( foundselfillumtint )
					{
						fresnelSelfillumtint->SetVecValue( Utils::CLR2FLT( Vars::Colors::Fresnel.r ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.g ),
														   Utils::CLR2FLT( Vars::Colors::Fresnel.b ) );
					}
					IMaterialVar *fresnelEnvmaptint = m_pMatFresnel->FindVar( "$envmaptint", &foundenvmaptint );
					if ( foundenvmaptint )
					{
						fresnelEnvmaptint->SetVecValue( Utils::CLR2FLT( NPCColor.r ), Utils::CLR2FLT( NPCColor.g ), Utils::CLR2FLT( NPCColor.b ) );
					}
				}
			}

			if ( Vars::Models::World::Model.var == 0 )
			{
				I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
				I::RenderView->SetBlend( 1.0f );
			}

			DrawModel( pNpc, Vars::Models::World::Overlay.var, NPCColor, Vars::Models::World::OverlayAlpha.var );
			if ( Vars::Models::World::Overlay.var )
			{
				if ( Vars::Models::World::Model.var == 0 )
				{
					I::ModelRender->ForcedMaterialOverride( m_pMatWorld );
					I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
					I::RenderView->SetBlend( 1.0f );
				}
				else
				{
					I::ModelRender->ForcedMaterialOverride( m_pMatWorld );
					I::RenderView->SetColorModulation( Utils::CLR2FLT( NPCColor.r ),
													   Utils::CLR2FLT( NPCColor.g ),
													   Utils::CLR2FLT( NPCColor.b ) );
					I::RenderView->SetBlend( Utils::CLR2FLT( NPCColor.a ) );
				}
			}
		}*/
	}

	I::ModelRender->ForcedMaterialOverride( nullptr );
	I::RenderView->SetColorModulation( 1.0f , 1.0f , 1.0f );
	pRenderContext->DepthRange( 0.0f , 1.0f );
}