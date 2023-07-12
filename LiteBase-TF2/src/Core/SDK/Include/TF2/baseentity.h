#pragma once
#include "ivmodelinfo.h"
#include "ivmodelrender.h"

class CUserCmd;
class CBaseCombatWeapon;
class CTakeDamageInfo;
class CDmgAccumulator;
class CEntityMapData;
class ISave;
class IRestore;
class C_Team;
class IPhysicsObject;
class CDamageModifier;
class CNewParticleEffect;
class IClientVehicle;
class CBaseAnimating;
class CStudioHdr;
class CBaseCombatCharacter;

struct FireBulletsInfo_t
{
	FireBulletsInfo_t( )
	{
		m_iShots = 1;
		m_vecSpread.Clear( );
		m_flDistance = 8192;
		m_iTracerFreq = 4;
		m_flDamage = 0;
		m_iPlayerDamage = 0;
		m_pAttacker = nullptr;
		m_nFlags = 0;
		m_pAdditionalIgnoreEnt = nullptr;
		m_flDamageForceScale = 1.0f;
		m_bPrimaryAttack = true;
		m_bUseServerRandomSeed = false;
	}

	FireBulletsInfo_t( int nShots, const Vec3 &vecSrc, const Vec3 &vecDir, const Vec3 &vecSpread, float flDistance,
					   int nAmmoType, bool bPrimaryAttack = true )
	{
		m_iShots = nShots;
		m_vecSrc = vecSrc;
		m_vecDirShooting = vecDir;
		m_vecSpread = vecSpread;
		m_flDistance = flDistance;
		m_iAmmoType = nAmmoType;
		m_iTracerFreq = 4;
		m_flDamage = 0;
		m_iPlayerDamage = 0;
		m_pAttacker = nullptr;
		m_nFlags = 0;
		m_pAdditionalIgnoreEnt = nullptr;
		m_flDamageForceScale = 1.0f;
		m_bPrimaryAttack = bPrimaryAttack;
		m_bUseServerRandomSeed = false;
	}

	int m_iShots = 0;
	Vec3 m_vecSrc = {};
	Vec3 m_vecDirShooting = {};
	Vec3 m_vecSpread = {};
	float m_flDistance = 0.f;
	int m_iAmmoType = 0;
	int m_iTracerFreq = 0;
	float m_flDamage = 0.f;
	int m_iPlayerDamage = 0; // Damage to be used instead of m_flDamage if we hit a player
	int m_nFlags = 0; // See FireBulletsFlags_t
	float m_flDamageForceScale = 0.f;
	CBaseEntity *m_pAttacker = nullptr;
	CBaseEntity *m_pAdditionalIgnoreEnt = nullptr;
	bool m_bPrimaryAttack = false;
	bool m_bUseServerRandomSeed = false;
};

struct EmitSound_t;
enum CollideType_t
{
	ENTITY_SHOULD_NOT_COLLIDE = 0,
	ENTITY_SHOULD_COLLIDE,
	ENTITY_SHOULD_RESPOND
};

class CBaseEntity : public IClientEntity
{
public:
	virtual                         ~CBaseEntity( ) = 0;

	virtual datamap_t *GetDataDescMap( ) = 0;
	virtual void *YouForgotToImplementOrDeclareClientClass( ) = 0;
	virtual datamap_t *GetPredDescMap( ) = 0;

	virtual void					FireBullets( const FireBulletsInfo_t &info ) = 0;
	virtual void					ModifyFireBulletsDamage( CTakeDamageInfo *dmgInfo ) = 0;
	virtual bool					ShouldDrawUnderwaterBulletBubbles( ) = 0;
	virtual bool					ShouldDrawWaterImpacts( void ) = 0;
	virtual bool					HandleShotImpactingWater( const FireBulletsInfo_t &info, const Vector &vecEnd, CTraceFilter *pTraceFilter, Vector *pVecTracerDest ) = 0;
	virtual CTraceFilter *GetBeamTraceFilter( void ) = 0;
	virtual void					DispatchTraceAttack( const CTakeDamageInfo *info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator = NULL ) = 0;
	virtual void					TraceAttack( const CTakeDamageInfo *info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator = NULL ) = 0;
	virtual void					DoImpactEffect( trace_t &tr, int nDamageType ) = 0;
	virtual void					MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType ) = 0;
	virtual int						GetTracerAttachment( void ) = 0;
	virtual int						BloodColor( ) = 0;
	virtual const char *GetTracerType( ) = 0;
	virtual void					Spawn( void ) = 0;
	virtual void					SpawnClientEntity( void ) = 0;
	virtual void					Precache( void ) = 0;
	virtual void					Activate( ) = 0;
	virtual void					ParseMapData( CEntityMapData *mapData ) = 0;
	virtual bool					KeyValue( const char *szKeyName, const char *szValue ) = 0;
	virtual bool					KeyValue( const char *szKeyName, float flValue ) = 0;
	virtual bool					KeyValue( const char *szKeyName, const Vector &vecValue ) = 0;
	virtual bool					GetKeyValue( const char *szKeyName, char *szValue, int iMaxLen ) = 0;
	virtual bool					Init( int entnum, int iSerialNum ) = 0;
	virtual CBaseAnimating *GetBaseAnimating( ) = 0;
	virtual void					SetClassname( const char *className ) = 0;
	virtual void			        RecordToolMessage( ) = 0;
	virtual Vector					GetObserverCamOrigin( void ) = 0;
	virtual bool					TestCollision( const Ray_t &ray, unsigned int fContentsMask, trace_t &tr ) = 0;
	virtual bool					TestHitboxes( const Ray_t &ray, unsigned int fContentsMask, trace_t &tr ) = 0;
	virtual float					GetAttackDamageScale( void ) = 0;
	virtual void					ValidateModelIndex( void ) = 0;
	virtual void					SetDormant( bool bDormant ) = 0;
	virtual int				        GetEFlags( ) const = 0;
	virtual void			        SetEFlags( int iEFlags ) = 0;
	virtual bool					ShouldSavePhysics( ) = 0;
	virtual void					OnSave( ) = 0;
	virtual void					OnRestore( ) = 0;
	virtual int						ObjectCaps( void ) = 0;
	virtual int						Save( ISave &save ) = 0;
	virtual int						Restore( IRestore &restore ) = 0;
	virtual bool	                CreateVPhysics( ) = 0;
	virtual void	                VPhysicsDestroyObject( void ) = 0;
	virtual void					VPhysicsUpdate( IPhysicsObject *pPhysics ) = 0;
	virtual int						VPhysicsGetObjectList( IPhysicsObject **pList, int listMax ) = 0;
	virtual bool					VPhysicsIsFlesh( void ) = 0;
	virtual void					AddEntity( void ) = 0;
	virtual const Vector &GetPrevLocalOrigin( ) const = 0;
	virtual const QAngle &GetPrevLocalAngles( ) const = 0;
	virtual int						CalcOverrideModelIndex( ) = 0;
	virtual const Vector &WorldAlignMins( ) const = 0;
	virtual const Vector &WorldAlignMaxs( ) const = 0;
	virtual const Vector &WorldSpaceCenter( ) const = 0;
	virtual void					ComputeWorldSpaceSurroundingBox( Vector *pVecWorldMins, Vector *pVecWorldMaxs ) = 0;
	virtual float					GetHealthBarHeightOffset( ) const = 0;
	virtual SolidType_t				GetSolid( void ) const = 0;
	virtual int			 			GetSolidFlags( void ) const = 0;
	virtual bool					GetAttachment( int number, Vector &origin ) = 0;
	virtual bool					GetAttachmentVelocity( int number, Vector &originVel, Vec3 &angleVel ) = 0;
	virtual C_Team *GetTeam( void ) const = 0;
	virtual int						GetTeamNumber( void ) const = 0;
	virtual void					ChangeTeam( int iTeamNum ) = 0;
	virtual int						GetRenderTeamNumber( void ) = 0;
	virtual bool					InSameTeam( const CBaseEntity *pEntity ) const = 0;
	virtual bool					InLocalTeam( void ) = 0;
	virtual bool					IsValidIDTarget( void ) = 0;
	virtual const char *GetIDString( void ) = 0;
	virtual void                    ModifyEmitSoundParams( EmitSound_t &params ) = 0;
	virtual bool                    InitializeAsClientEntity( const char *pszModelName, int renderGroup ) = 0; //RenderGroup_t
	virtual void					Simulate( ) = 0;
	virtual IClientVehicle *GetClientVehicle( ) = 0;
	virtual void					GetAimEntOrigin( IClientEntity *pAttachedTo, Vector *pAbsOrigin, QAngle *pAbsAngles ) = 0;
	virtual const Vector &GetOldOrigin( ) = 0;
	virtual int			            GetRenderGroup( ) = 0; //RenderGroup_t
	virtual void					GetToolRecordingState( KeyValues *msg ) = 0;
	virtual void					CleanupToolRecordingState( KeyValues *msg ) = 0;
	virtual CollideType_t			GetCollideType( void ) = 0;
	virtual void					UpdateVisibility( ) = 0;
	virtual bool					IsSelfAnimating( ) = 0;
	virtual	void					OnLatchInterpolatedVariables( int flags ) = 0;
	virtual CStudioHdr *OnNewModel( ) = 0;
	virtual void					OnNewParticleEffect( const char *pszParticleName, CNewParticleEffect *pNewParticleEffect ) = 0;
	virtual void					ResetLatched( ) = 0;
	virtual bool					Interpolate( float currentTime ) = 0;
	virtual bool					IsSubModel( void ) = 0;
	virtual void					CreateLightEffects( void ) = 0;
	virtual void					Clear( void ) = 0;
	virtual int						DrawBrushModel( bool bTranslucent, int nFlags, bool bTwoPass ) = 0;
	virtual float					GetTextureAnimationStartTime( ) = 0;
	virtual void					TextureAnimationWrapped( ) = 0;
	virtual void					SetNextClientThink( float nextThinkTime ) = 0;
	virtual void					SetHealth( int iHealth ) = 0;
	virtual int						GetHealth( ) const = 0;
	virtual int						GetMaxHealth( ) const = 0;
	virtual bool					IsVisibleToTargetID( void ) const = 0;
	virtual bool					IsHealthBarVisible( void ) const = 0;
	virtual ShadowType_t			ShadowCastType( ) = 0;
	virtual bool					ShouldReceiveProjectedTextures( int flags ) = 0;
	virtual void					AddDecal( const Vector &rayStart, const Vector &rayEnd, const Vector &decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t &tr, int maxLODToDecal = ADDDECAL_TO_ALL_LODS ) = 0;
	virtual void					AddColoredDecal( const Vector &rayStart, const Vector &rayEnd, const Vector &decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t &tr, Color_t cColor, int maxLODToDecal = ADDDECAL_TO_ALL_LODS ) = 0;
	virtual bool					IsClientCreated( void ) const = 0;
	virtual void					UpdateOnRemove( void ) = 0;
	virtual void					SUB_Remove( void ) = 0;
	virtual void					SetPredictable( bool state ) = 0;
	virtual int						RestoreData( const char *context, int slot, int type ) = 0;
	virtual char const *DamageDecal( int bitsDamageType, int gameMaterial ) = 0;
	virtual void					DecalTrace( trace_t *pTrace, char const *decalName ) = 0;
	virtual void					ImpactTrace( trace_t *pTrace, int iDamageType, const char *pCustomImpactName ) = 0;
	virtual bool					ShouldPredict( void ) = 0;
	virtual void					Think( void ) = 0;
	virtual bool			        CanBePoweredUp( void ) = 0;
	virtual bool			        AttemptToPowerup( int iPowerup, float flTime, float flAmount = 0, CBaseEntity *pAttacker = NULL, CDamageModifier *pDamageModifier = NULL ) = 0;
	virtual bool			        IsCurrentlyTouching( void ) const = 0;
	virtual void			        StartTouch( CBaseEntity *pOther ) = 0;
	virtual void			        Touch( CBaseEntity *pOther ) = 0;
	virtual void			        EndTouch( CBaseEntity *pOther ) = 0;
	virtual unsigned int	        PhysicsSolidMaskForEntity( void ) const = 0;
	virtual void					PhysicsSimulate( void ) = 0;
	virtual bool					IsAlive( void ) = 0;
	virtual bool					IsPlayer( void ) const = 0;
	virtual bool					IsBaseCombatCharacter( void ) = 0;
	virtual CBaseCombatCharacter *MyCombatCharacterPointer( void ) = 0;
	virtual bool					IsNPC( void ) = 0;
	virtual bool					IsNextBot( ) = 0;
	virtual bool					IsBaseObject( void ) const = 0;
	virtual bool					IsBaseCombatWeapon( void ) const = 0;
	virtual CBaseCombatWeapon *MyCombatWeaponPointer( ) = 0;
	virtual bool					IsCombatItem( void ) const = 0;
	virtual bool					IsBaseTrain( void ) const = 0;
	virtual Vector			        EyePosition( void ) = 0;
	virtual const QAngle &EyeAngles( void ) = 0;
	virtual const QAngle &LocalEyeAngles( void ) = 0;
	virtual Vector		            EarPosition( void ) = 0;
	virtual bool		            ShouldCollide( int collisionGroup, int contentsMask ) const = 0;
	virtual const Vector &GetViewOffset( ) const = 0;
	virtual void		            SetViewOffset( const Vector &v ) = 0;
	virtual void                    ClientAdjustStartSoundParams( void *unk ) = 0; //EmitSound_t
	virtual void                    ClientAdjustStartSoundParams2( void *unk ) = 0; //StartSoundParams_t
	virtual bool                    ShouldInterpolate( ) = 0;
	virtual unsigned char           GetClientSideFade( ) = 0;
	virtual void                    BoneMergeFastCullBloat( Vector &localMins, Vector &localMaxs, const Vector &thisEntityMins, const Vector &thisEntityMaxs ) const = 0;
	virtual bool					OnPredictedEntityRemove( bool isbeingremoved, CBaseEntity *predicted ) = 0;
	virtual CBaseEntity *GetShadowUseOtherEntity( void ) const = 0;
	virtual void					SetShadowUseOtherEntity( CBaseEntity *pEntity ) = 0;
	virtual bool					AddRagdollToFadeQueue( void ) = 0;
	virtual bool					IsDeflectable( ) = 0;
	virtual int						GetStudioBody( void ) = 0;
	virtual void                    PerformCustomPhysics( Vector *pNewPosition, Vector *pNewVelocity, QAngle *pNewAngles, QAngle *pNewAngVelocity ) = 0;
	virtual bool					ValidateEntityAttachedToPlayer( bool &bShouldRetry ) = 0;
public:
	NETVAR(m_flAnimTime, float, "CBaseEntity", "m_flAnimTime");
	NETVAR(m_flSimulationTime, float, "CBaseEntity", "m_flSimulationTime");
	NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame");
	NETVAR(m_vecOrigin, Vec3, "CBaseEntity", "m_vecOrigin");
	NETVAR(m_angRotation, Vec3, "CBaseEntity", "m_angRotation");
	NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex");
	NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects");
	NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode");
	NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX");
	NETVAR(m_clrRender, int, "CBaseEntity", "m_clrRender");
	NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum");
	NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup");
	NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity");
	NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance");
	NETVAR(m_hOwnerEntity, int, "CBaseEntity", "m_hOwnerEntity");
	NETVAR(m_hEffectEntity, int, "CBaseEntity", "m_hEffectEntity");
	NETVAR(moveparent, int, "CBaseEntity", "moveparent");
	NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment");
	NETVAR(m_Collision, void *, "CBaseEntity", "m_Collision");
	NETVAR(m_vecMinsPreScaled, Vec3, "CBaseEntity", "m_vecMinsPreScaled");
	NETVAR(m_vecMaxsPreScaled, Vec3, "CBaseEntity", "m_vecMaxsPreScaled");
	NETVAR(m_vecMins, Vec3, "CBaseEntity", "m_vecMins");
	NETVAR(m_vecMaxs, Vec3, "CBaseEntity", "m_vecMaxs");
	NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType");
	NETVAR(m_usSolidFlags, int, "CBaseEntity", "m_usSolidFlags");
	NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType");
	NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat");
	NETVAR(m_bUniformTriggerBloat, bool, "CBaseEntity", "m_bUniformTriggerBloat");
	NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMins, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMins");
	NETVAR(m_vecSpecifiedSurroundingMaxs, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs");
	NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex");
	NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID");
	NETVAR(m_bIsPlayerSimulated, bool, "CBaseEntity", "m_bIsPlayerSimulated");
	NETVAR(m_bSimulatedEveryTick, bool, "CBaseEntity", "m_bSimulatedEveryTick");
	NETVAR(m_bAnimatedEveryTick, bool, "CBaseEntity", "m_bAnimatedEveryTick");
	NETVAR(m_bAlternateSorting, bool, "CBaseEntity", "m_bAlternateSorting");
	NETVAR(m_nModelIndexOverrides, void *, "CBaseEntity", "m_nModelIndexOverrides");
	NETVAR(movetype, int, "CBaseEntity", "movetype");

	//M_OFFSETGET(PunchAngles, Vec3, 0xE8C);

	inline Vec3 GetPunchAngles( )
	{
		return *reinterpret_cast< Vec3 * >( reinterpret_cast< DWORD >( this ) + 0xE8C );
	}

	inline void ClearPunchAngle() { //m_vecPunchAngle
		*reinterpret_cast< Vec3 * >( reinterpret_cast< DWORD >( this ) + 0xE8C ) = Vec3( 0.0f, 0.0f, 0.0f );
	}

	inline MoveType_t GetMoveType()
	{
		return *reinterpret_cast<MoveType_t*>(reinterpret_cast<DWORD>(this) + 0x1A4);
	}

	inline const char* GetModelName() {
	return I::ModelInfo->GetModelName(GetModel());
	}

	inline int GetPipebombType()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x8FC);
	}

	inline Vec3 GetWorldSpaceCenter() {
		Vec3 vMin, vMax; GetRenderBounds(vMin, vMax);
		Vec3 vWorldSpaceCenter = GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2.0f;
		return vWorldSpaceCenter;
	}

	inline void SetTickBase(const int nTickBase)
	{
		DWORD dwOffset = g_NetVars.get_offset("DT_BasePlayer", "localdata", "m_nTickBase");
		*reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + dwOffset) = nTickBase;
	}

	inline void SetFinalTick( const int nFinalTick )
	{
		DWORD dwOffset = g_NetVars.get_offset( "DT_BasePlayer", "localdata", "m_nTickBase" ) + 0x4;
		*reinterpret_cast< int * >( reinterpret_cast< DWORD >( this ) + dwOffset ) = nFinalTick;
	}

	inline void SetCurrentCmd(CUserCmd* pCmd)
	{
		DWORD dwOffset = g_NetVars.get_offset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + dwOffset - 0x4) = pCmd;
	}

	inline float GetOldSimulationTime()
	{
		DWORD dwOffset = g_NetVars.get_offset("DT_BaseEntity", "m_flSimulationTime");
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + dwOffset + 0x4);
	}

	inline float GetOldAnimTime( )
	{
		DWORD dwOffset = g_NetVars.get_offset( "DT_BaseEntity", "m_flSimulationTime" );
		return *reinterpret_cast< float * >( reinterpret_cast< DWORD >( this ) + dwOffset - 0x4);
	}

	inline Vec3 GetVelocity() {
		static auto FN = reinterpret_cast<void(__thiscall*)(CBaseEntity*, Vec3&)>(g_Pattern.Find((L"client.dll"), (L"55 8B EC 83 EC ? 56 8B F1 E8 ? ? ? ? 3B F0")));
		Vec3 v;
		FN(this, v);
		return v;
	}

	int GetAmmoCount( int iAmmoIndex )
	{
		static auto FN = reinterpret_cast< int( __thiscall * )( CBaseEntity *, int ) >( g_Pattern.Find( L"client.dll", L"55 8B EC 56 8B 75 08 57 8B F9 83 FE FF 75 08 5F 33 C0 5E 5D C2 04 00" ) );
		return FN( this, iAmmoIndex );
	}

	//M_OFFSETGET(PipebombType, int, 0x8FC)
};