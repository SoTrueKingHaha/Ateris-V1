#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace CheckForSequenceChange
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, CStudioHdr *, int, bool, bool );
		void __fastcall Func( void *ecx, void *edx, CStudioHdr *hdr, int nCurSequence, bool bForceNewSequence, bool bInterpolate );
		void Init( );
	}

	namespace MaintainSequenceTransitions
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, void *, float, Vec3, Vector4D );
		void __fastcall Func( void *ecx, void *edx, void *boneSetup, float flCycle, Vec3 pos[ ], Vector4D q[ ] );
		void Init( );
	}
}