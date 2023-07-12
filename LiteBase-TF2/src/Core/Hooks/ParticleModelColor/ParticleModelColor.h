#pragma once
#include "../../SDK/SDK.h"

typedef union
{
    float m128_f32[ 4 ];
    uint32_t m128_u32[ 4 ];
} fltx4;

struct SpriteRenderInfo_t
{
    size_t m_nXYZStride{};
    fltx4 *m_pXYZ{};
    size_t m_nRotStride{};
    fltx4 *m_pRot{};
    size_t m_nYawStride{};
    fltx4 *m_pYaw{};
    size_t m_nRGBStride{};
    fltx4 *m_pRGB{};
    size_t m_nCreationTimeStride{};
    fltx4 *m_pCreationTimeStamp{};
    size_t m_nSequenceStride{};
    fltx4 *m_pSequenceNumber{};
    size_t m_nSequence1Stride{};
    fltx4 *m_pSequence1Number{};
    float m_flAgeScale{};
    float m_flAgeScale2{};
    void *m_pSheet{};
    int m_nVertexOffset{};
    void *m_pParticles{};
};

namespace Hooks
{
	/*namespace CNewParticleEffectDrawModel
	{
		inline CHook Hook;
		using fn = int( __fastcall * )( void* ecx, void* edx, int );
        int __fastcall Func( void *ecx, void *edx, int flags );
		void Init( );
	}*/

    namespace RenderSpriteCard
    {
        inline CHook Hook;
        using fn = void( __fastcall * )( void *ecx, void *edx, void*, void*, SpriteRenderInfo_t&, int, void*, void* );
        void __fastcall Func( void *ecx, void *edx, void *pMeshBuilder, void *pCtx, SpriteRenderInfo_t &info, int hParticle, void* pSortList, void *pCamera );
        void Init( );
    }
}