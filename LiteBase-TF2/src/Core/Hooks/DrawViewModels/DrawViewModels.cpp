#include "DrawViewModels.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

void SetDrawInfo( IMaterial *pMaterial, Color_t Color, float flAlpha, bool bDepth )
{
	I::ModelRender->ForcedMaterialOverride( pMaterial );
	I::RenderView->SetColorModulation( Color );
	I::RenderView->SetBlend( flAlpha );

	if ( auto pRenderContext = I::MaterialSystem->GetRenderContext( ) )
		pRenderContext->DepthRange( 0.0f, bDepth ? 1.0f : 0.2f );
}

void ResetDrawInfo( )
{
	I::ModelRender->ForcedMaterialOverride( nullptr );
	I::RenderView->SetColorModulation( 1.0f, 1.0f, 1.0f );
	I::RenderView->SetBlend( 1.0f );

	if ( auto pRenderContext = I::MaterialSystem->GetRenderContext( ) )
		pRenderContext->DepthRange( 0.0f, 1.0f );
}

void Hooks::DrawViewModels::SetScale( int nScale, bool bReset = false )
{
	static IMaterialVar *pVar = nullptr;
	static bool bFound = false;

	if ( bReset ) { pVar = nullptr; bFound = false; return; }

	if ( !bFound && m_pMatBlurY_vm )
	{
		pVar = m_pMatBlurY_vm->FindVar( ( "$bloomamount" ), &bFound );
	}

	else if ( pVar )
	{
		pVar->SetIntValue( nScale );
	}
}

void __fastcall Hooks::DrawViewModels::Func( void *ecx, void *edx, const CViewSetup &pViewSetup, bool bDrawViewmodel )
{
	const auto &pLocal = g_EntityCache.GetLocal( );

	if ( !ecx || !edx || !pLocal )
	{
		return;
	}
	if ( !init )
	{
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

		static auto m_pMatBlurXkv_vm = new KeyValues( "BlurFilterX" );
		m_pMatBlurXkv_vm->SetString( "$basetexture", "glow_buffer_1" );
		m_pMatBlurX_vm = Models.CreateNRef( "m_pMatBlurX_vm", m_pMatBlurXkv_vm );

		static auto m_pMatBlurXwfkv_vm = new KeyValues( "BlurFilterX" );
		m_pMatBlurXwfkv_vm->SetString( "$basetexture", "glow_buffer_1" );
		m_pMatBlurXwfkv_vm->SetBool( "$wireframe", true );
		m_pMatBlurXwf_vm = Models.CreateNRef( "m_pMatBlurXwf_vm", m_pMatBlurXwfkv_vm );

		static auto m_pMatBlurYkv_vm = new KeyValues( "BlurFilterY" );
		m_pMatBlurYkv_vm->SetString( "$basetexture", "glow_buffer_2" );
		m_pMatBlurY_vm = Models.CreateNRef( "m_pMatBlurY_vm", m_pMatBlurYkv_vm );

		static auto m_pMatBlurYwfkv_vm = new KeyValues( "BlurFilterY" );
		m_pMatBlurYwfkv_vm->SetString( "$basetexture", "glow_buffer_2" );
		m_pMatBlurYwfkv_vm->SetBool( "$wireframe", true );
		m_pMatBlurYwf_vm = Models.CreateNRef( "m_pMatBlurYwf_vm", m_pMatBlurYwfkv_vm );

		static auto m_pMatHaloAddToScreenkv_vm = new KeyValues( "UnlitGeneric" );
		m_pMatHaloAddToScreenkv_vm->SetString( "$basetexture", "glow_buffer_1" );
		m_pMatHaloAddToScreenkv_vm->SetBool( "$additive", true );
		m_pMatHaloAddToScreen_vm = Models.CreateNRef( "m_pMatHaloAddToScreen_vm", m_pMatHaloAddToScreenkv_vm );
		init = true;
	}

	if ( Vars::Outline::Players::ViewmodelOutline.var )
	{
		if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
		{
			if ( auto pRenderContext = I::MaterialSystem->GetRenderContext( ) )
			{
				int w = g_Draw.m_nScreenW;
				int h = g_Draw.m_nScreenH;

				if ( w < 1 || h < 1 || w > 4096 || h > 2160 )
					return Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );

				SetScale( Vars::Outline::Players::OutlineScale.var );

				pRenderContext->SetStencilEnable( true );
				{
					pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_ALWAYS );
					pRenderContext->SetStencilPassOperation( STENCILOPERATION_REPLACE );
					pRenderContext->SetStencilFailOperation( STENCILOPERATION_KEEP );
					pRenderContext->SetStencilZFailOperation( STENCILOPERATION_REPLACE );
					pRenderContext->SetStencilWriteMask( 0x0 );
					pRenderContext->SetStencilTestMask( 0xFF );
					pRenderContext->SetStencilReferenceValue( 1 );
					{
						SetDrawInfo( nullptr, { 255, 255, 255, 255 }, 1.0f, false );
						{
							Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );
						}
						ResetDrawInfo( );

						I::ModelRender->ForcedMaterialOverride( m_pMatGlowColor );

						pRenderContext->PushRenderTargetAndViewport( );
						{
							pRenderContext->SetRenderTarget( m_pRenderBuffer1 );
							pRenderContext->Viewport( 0, 0, w, h );
							pRenderContext->ClearColor4ub( 0, 0, 0, 0 );
							pRenderContext->ClearBuffers( true, false, false );

							I::RenderView->SetBlend( Vars::Outline::Players::OutlineAlpha.var );
							I::RenderView->SetColorModulation( Utils::CLR2FLT( Vars::Colors::LocalPlayer.r ),
															   Utils::CLR2FLT( Vars::Colors::LocalPlayer.g ),
															   Utils::CLR2FLT( Vars::Colors::LocalPlayer.b ) );
							Outlines.bColoringViewmodel = true;
							Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );
							Outlines.bColoringViewmodel = false;
						}
						pRenderContext->PopRenderTargetAndViewport( );

						if ( Vars::Outline::Main::BlurOutline.var ) {
							pRenderContext->PushRenderTargetAndViewport( );
							{
								pRenderContext->Viewport( 0, 0, w, h );

								pRenderContext->SetRenderTarget( m_pRenderBuffer2 );
								pRenderContext->DrawScreenSpaceRectangle( m_pMatBlurX_vm, 0,
																		  0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
								//pRenderContext->DrawScreenSpaceRectangle(m_pMatBlurX, 0, 0, w, h, 0.0f, 0.0f, w, h, w, h);

								pRenderContext->SetRenderTarget( m_pRenderBuffer1 );
								pRenderContext->DrawScreenSpaceRectangle( m_pMatBlurY_vm, 0,
																		  0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
								//pRenderContext->DrawScreenSpaceRectangle(m_pMatBlurY, 0, 0, w, h, 0.0f, 0.0f, w, h, w, h);
							}
							pRenderContext->PopRenderTargetAndViewport( );
						}
					}

					pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_EQUAL );
					pRenderContext->SetStencilPassOperation( STENCILOPERATION_KEEP );
					pRenderContext->SetStencilFailOperation( STENCILOPERATION_KEEP );
					pRenderContext->SetStencilZFailOperation( STENCILOPERATION_KEEP );
					pRenderContext->SetStencilWriteMask( 0xFF );
					pRenderContext->SetStencilTestMask( 0x0 );
					pRenderContext->SetStencilReferenceValue( 0 );
					{
						Outlines.bColoringViewmodel = true;
						if ( Vars::Outline::Main::WireframeOutline.var )
						{
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, -1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, -1, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 0, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 0, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 1, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, -1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
						}
						else if ( Vars::Outline::Main::BlurOutline.var ) {
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
						}

						if ( Vars::Outline::Main::CheapOutline.var )
						{
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, -1, -1, w + 2, h + 2, -2.0f, -2.0f, w + 1, h + 1, w + 2, h + 2 );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, -1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 1, 1, w - 1, h - 1, 2.0f, 2.0f, w, h, w - 1, h - 1 );
						}
						else if ( Vars::Outline::Main::BlurOutline.var ) {
							pRenderContext->DrawScreenSpaceRectangle( m_pMatHaloAddToScreen_vm, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h );
						}
						Outlines.bColoringViewmodel = false;
					}
				}
				pRenderContext->SetStencilEnable( false );
				SetDrawInfo( nullptr, { 255, 255, 255, 255 }, 1.0f, false );
				{
					Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );
				}
				ResetDrawInfo( );
			}
			else
			{
				return Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );
			}
		}
		else
		{
			return Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );
		}
	}
	else
	{
		return Hook.CallOriginal<fn>( )( ecx, edx, pViewSetup, bDrawViewmodel );
	}
}

void Hooks::DrawViewModels::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 81 EC ? ? ? ? 8B 15 ? ? ? ? 53 8B D9 33 C9 89 4D C4 89 4D C8 8B 02 89 5D D4 89 4D CC 89 4D D0 56" ) );
	Hook.Create( pSrc, Func );
}