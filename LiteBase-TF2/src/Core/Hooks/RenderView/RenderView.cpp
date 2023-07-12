#include "RenderView.h"
#include "../../Features/Visuals/Outlines/Outlines.h"
#include "../../Features/Visuals/AttributeChanger/AttributeChanger.h"

void __fastcall Hooks::RenderView::Func( void *ecx, void *edx, const CViewSetup &view, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw )
{
	static std::once_flag onceFlag;
	std::call_once( onceFlag, [ ]
	{
		//g_AttributeChanger.Init( );
	} );

	Hook.CallOriginal<void( __thiscall * )( void *, const CViewSetup &, int, int )>( )( ecx, view, nClearFlags, whatToDraw );
}

void Hooks::RenderView::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::ViewRender, 6 ) );
	Hook.Create( FN, Func );
}