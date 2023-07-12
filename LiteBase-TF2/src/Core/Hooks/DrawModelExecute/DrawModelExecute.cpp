#include "DrawModelExecute.h"
#include "../../Features/Exploits/AntiAim/FakeAng.h"

//void RenderFakeAng( void* ecx, void* edx, const DrawModelState_t &pState, const ModelRenderInfo_t &pInfo );
//void DrawBT( void *ecx, void *edx, CTFPlayer *pEntity, const DrawModelState_t &pState, const ModelRenderInfo_t &pInfo, matrix3x4_t *pBoneToWorld );

void __fastcall Hooks::DrawModelExecute::Func( void *ecx, void *edx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld )
{
	IClientEntity *pEntity = I::ClientEntityList->GetClientEntity( pInfo.entity_index );
	//DrawBT( ecx, edx, pEntity->As<CTFPlayer>( ), state, pInfo, pCustomBoneToWorld );
	if ( ( Models.HasDrawn( pEntity ) || Outlines.HasDrawn( pEntity ) ) && !Outlines.m_bDrawingGlow ) { return; }

	Hook.CallOriginal<fn>( )( ecx, edx, state, pInfo, pCustomBoneToWorld );
}

void Hooks::DrawModelExecute::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::ModelRender, 19));
	Hook.Create(FN, Func);
}

/*void DrawBT( void *ecx, void *edx, CTFPlayer *pEntity, const DrawModelState_t &pState, const ModelRenderInfo_t &pInfo,
			 matrix3x4_t *pBoneToWorld )
{
	auto OriginalFn = Hooks::DrawModelExecute::Hook.CallOriginal<Hooks::DrawModelExecute::fn>( );

	if ( g_GlobalInfo.m_WeaponType == EWeaponType::PROJECTILE ) return;

	if ( Vars::Backtrack::Active.var && Vars::Backtrack::BtChams::Enabled.var )
	{
		if ( pEntity && pEntity->GetClassId( ) == EClassIds::CTFPlayer )
		{
			if ( pEntity->deadflag( ) )
			{
				return;
			}
			if ( !Outlines.m_bRendering && !Models.m_bRendering )
			{
				if ( Vars::Backtrack::BtChams::EnemyOnly.var && g_EntityCache.GetLocal( ) && pEntity->m_iTeamNum( ) ==
					 g_EntityCache.GetLocal( )->m_iTeamNum( ) )
				{
					return;
				}

				IMaterial *chosenMat = Vars::Backtrack::BtChams::Material.var ? Models.matList.at( Vars::Backtrack::BtChams::Material.var - 1 ) : nullptr;

				I::ModelRender->ForcedMaterialOverride( chosenMat );

				if ( chosenMat )
				{
					I::RenderView->SetColorModulation(
						Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.r ),
						Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.g ),
						Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.b ) );
				}


				if ( const auto &pRenderContext = I::MaterialSystem->GetRenderContext( ) )
				{
					if ( Vars::Backtrack::BtChams::IgnoreZ.var )
						pRenderContext->DepthRange( 0.0f, 0.2f );
				}

				I::RenderView->SetBlend( Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.a ) );

				const auto &vRecords = g_Backtrack.GetRecords( pEntity );
				if ( Vars::Backtrack::BtChams::LastOnly.var )
				{
					std::optional<TickRecord> vLastRec = g_Backtrack.GetLastRecord( pEntity );
					if ( vLastRec )
					{
						OriginalFn( ecx, edx, pState, pInfo, ( matrix3x4_t * )( &vLastRec->BoneMatrix ) );
					}
				}
				else
				{
					if ( vRecords )
					{
						for ( auto &record : *vRecords )
						{
							I::RenderView->SetColorModulation( record.bOnShot ? 1 : Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.r ), record.bOnShot ? 0 : Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.g ), record.bOnShot ? 0 : Utils::CLR2FLT( Vars::Backtrack::BtChams::BacktrackColor.b ) );
							if ( g_Backtrack.WithinRewind( record ) ) { OriginalFn( ecx, edx, pState, pInfo, ( matrix3x4_t * )( &record.BoneMatrix ) ); }
						}
					}
				}

				I::ModelRender->ForcedMaterialOverride( nullptr );
				I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
				I::RenderView->SetBlend( 1.0f );

				if ( const auto &pRenderContext = I::MaterialSystem->GetRenderContext( ) )
				{
					if ( Vars::Backtrack::BtChams::IgnoreZ.var )
						pRenderContext->DepthRange( 0.0f, 1.0f );
				}
			}
		}
	}
}*/

/*void RenderFakeAng( void *ecx, void *edx, const DrawModelState_t &pState, const ModelRenderInfo_t &pInfo ) {
	const auto &pRenderContext = I::MaterialSystem->GetRenderContext( );

	if ( !g_FakeAng.DrawChams ) {
		return;
	}

	if ( !pRenderContext ) {
		return;
	}

	{
		pRenderContext->DepthRange( 0.0f, 1.0f );
		I::ModelRender->ForcedMaterialOverride( nullptr );
		I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
		I::RenderView->SetBlend( 1.0f );
	}

	{
		IMaterial *chamsMaterial = Models.flatmaterial;
		Color_t color = { 255, 255, 255, 100 };

		if ( chamsMaterial ) {
			chamsMaterial->IncrementReferenceCount( );
		}

		pRenderContext->DepthRange( 0.0f, 0.2f );

		I::RenderView->SetBlend( 0.0f );

		I::ModelRender->ForcedMaterialOverride( nullptr );

		I::RenderView->SetColorModulation(
			Utils::CLR2FLT( Vars::Colors::LocalPlayer.r ),
			Utils::CLR2FLT( Vars::Colors::LocalPlayer.g ),
			Utils::CLR2FLT( Vars::Colors::LocalPlayer.b ) );

		Hooks::DrawModelExecute::Hook.CallOriginal<Hooks::DrawModelExecute::fn>( )( ecx, edx, pState, pInfo, reinterpret_cast< matrix3x4_t * >( &g_FakeAng.BoneMatrix ) );

		IMaterial *overlayMaterial = Models.overlaymaterial;

		if ( overlayMaterial )
		{
			overlayMaterial->IncrementReferenceCount( );

			if ( IMaterialVar *$phongtint = overlayMaterial->FindVar( "$phongtint", nullptr, false ) )
			{
				$phongtint->SetVecvar(
					Utils::CLR2FLT( 10 ),
					Utils::CLR2FLT( 0 ),
					Utils::CLR2FLT( 30 ) );
			}
			if ( IMaterialVar *$envmaptint = overlayMaterial->FindVar( "$envmaptint", nullptr, false ) )
			{
				$envmaptint->SetVecvar(
					Utils::CLR2FLT( 10 ),
					Utils::CLR2FLT( 0 ),
					Utils::CLR2FLT( 30 ) );
			}
			if ( IMaterialVar *$phongfresnelranges = overlayMaterial->FindVar( "$phongfresnelranges", nullptr, false ) )
			{
				$phongfresnelranges->SetVecvar( 0, 0.5 / 1.0f, 10 / 1.0f );
			}
			overlayMaterial->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, chams.overlayType == 2  false );

			I::RenderView->SetBlend( 1.0f );
			I::ModelRender->ForcedMaterialOverride( overlayMaterial );

			Hooks::DrawModelExecute::Hook.CallOriginal<Hooks::DrawModelExecute::fn>( )( ecx, edx, pState, pInfo, reinterpret_cast< matrix3x4_t * >( &g_FakeAng.BoneMatrix ) );
		}

		pRenderContext->DepthRange( 0.0f, 1.0f );
		I::ModelRender->ForcedMaterialOverride( nullptr );
		I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );

		I::RenderView->SetBlend( 1.0f );
		return;
	}
}*/