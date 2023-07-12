#pragma once
#include "imatrendercontext.h"
#include "keyvalues.h"

class IMaterialSystemHardwareConfig;
class MaterialSystem_Config_t;
class MaterialAdapterInfo_t;
class MaterialVideoMode_t;
class ModeChangeCallbackFunc_t;
class MaterialSystemHardwareIdentifier_t;
class HDRType_t;
class MaterialBufferReleaseFunc_t;
class MaterialBufferRestoreFunc_t;
class MaterialSystem_SortInfo_t;
class MaterialLock_t;
class MaterialContextType_t;
class ITextureCompositor;
class IShader;
class IFileList;
typedef unsigned short MaterialHandle_t;

class IMaterialProxy;

class IMaterialProxyFactory
{
public:
	virtual IMaterialProxy *CreateProxy(const char *proxyName) = 0;
	virtual void DeleteProxy(IMaterialProxy *pProxy) = 0;
};

enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,
	RT_SIZE_DEFAULT = 1,
	RT_SIZE_PICMIP = 2,
	RT_SIZE_HDR = 3,
	RT_SIZE_FULL_FRAME_BUFFER = 4,
	RT_SIZE_OFFSCREEN = 5,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6,
	RT_SIZE_REPLAY_SCREENSHOT = 7,
	RT_SIZE_LITERAL = 8,
	RT_SIZE_LITERAL_PICMIP = 9

};

enum CompiledVtfFlags
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_SRGB = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,
	TEXTUREFLAGS_STAGING_MEMORY = 0x00080000,
	TEXTUREFLAGS_IMMEDIATE_CLEANUP = 0x00100000,
	TEXTUREFLAGS_IGNORE_PICMIP = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
	TEXTUREFLAGS_CLAMPU = 0x02000000,
	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
	TEXTUREFLAGS_SSBUMP = 0x08000000,
	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
	TEXTUREFLAGS_BORDER = 0x20000000,
	TEXTUREFLAGS_STREAMABLE_COARSE = 0x40000000,
	TEXTUREFLAGS_STREAMABLE_FINE = 0x80000000,
	TEXTUREFLAGS_STREAMABLE = (TEXTUREFLAGS_STREAMABLE_COARSE | TEXTUREFLAGS_STREAMABLE_FINE)
};

class IMaterialSystem : public IAppSystem
{
public:
	IMaterial *FindMaterial( char const *szMat, const char *szTextureGroup, bool bComplain = true, const char *szPrefix = NULL )
	{
		typedef IMaterial *( __thiscall *FN )( PVOID, const char *, const char *, bool, const char * );
		return GetVFunc<FN>( this, 73 )( this, szMat, szTextureGroup, bComplain, szPrefix );
	}

	IMaterial *CreateMaterial( char const *szName, void *pKV )
	{
		typedef IMaterial *( __thiscall *FN )( PVOID, const char *, void * );
		return GetVFunc<FN>( this, 72 )( this, szName, pKV );
	}

	IMaterial *GetMaterial( MaterialHandle_t hMat )
	{
		typedef IMaterial *( __thiscall *FN )( PVOID, MaterialHandle_t );
		return GetVFunc<FN>( this, 78 )( this, hMat );
	}

	MaterialHandle_t FirstMaterial( )
	{
		typedef MaterialHandle_t( __thiscall *FN )( PVOID );
		return GetVFunc<FN>( this, 75 )( this );
	}

	MaterialHandle_t InvalidMaterial( )
	{
		typedef MaterialHandle_t( __thiscall *FN )( PVOID );
		return GetVFunc<FN>( this, 77 )( this );
	}

	MaterialHandle_t NextMaterial( MaterialHandle_t hMat )
	{
		typedef MaterialHandle_t( __thiscall *FN )( PVOID, MaterialHandle_t );
		return GetVFunc<FN>( this, 76 )( this, hMat );
	}

	ITexture *CreateNamedRenderTargetTextureEx( const char *pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format,
												MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
												unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
												unsigned int renderTargetFlags = 0 )
	{
		typedef ITexture *( __thiscall *FN )( PVOID, const char *, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t, unsigned int, unsigned int );
		return GetVFunc<FN>( this, 87 )( this, pRTName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags );
	}

	ITexture *FindTexture( char const *pTextureName, const char *pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0 )
	{
		typedef ITexture *( __thiscall *FN )( PVOID, char const *, const char *, bool, int );
		return GetVFunc<FN>( this, 81 )( this, pTextureName, pTextureGroupName, complain, nAdditionalCreationFlags );
	}

	IMatRenderContext *GetRenderContext( )
	{
		typedef IMatRenderContext *( __thiscall *FN )( PVOID );
		return GetVFunc<FN>( this, 100 )( this );
	}
};

namespace I { inline IMaterialSystem *MaterialSystem; }