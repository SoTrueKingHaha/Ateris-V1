#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace DrawViewModels
	{
		inline IMaterial *m_pMatGlowColor;
		inline ITexture *m_pRtFullFrame;
		inline ITexture *m_pRenderBuffer1;
		inline ITexture *m_pRenderBuffer2;
		inline IMaterial *m_pMatBlurX_vm;
		inline IMaterial *m_pMatBlurY_vm;
		inline IMaterial *m_pMatBlurXwf_vm;
		inline IMaterial *m_pMatBlurYwf_vm;
		inline IMaterial *m_pMatHaloAddToScreen_vm;
		inline static bool init = false;
		void SetScale( int nScale, bool bReset );

		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, const CViewSetup &, bool );
		void __fastcall Func( void *ecx, void *edx, const CViewSetup &pViewRender, bool bDrawViewmodel );
		void Init( );
	}
}