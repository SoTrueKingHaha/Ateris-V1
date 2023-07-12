#pragma once
#include "basecombatcharacter.h"
#include "basecombatweapon.h"
#include "GameEventListener.h"
class IRagdoll;
class CHintSystem;
struct surfacedata_t;
struct CSoundParameters;
enum struct PLAYER_ANIM;

class CBasePlayer : public CBaseCombatCharacter, public CGameEventListener
{
public:
	virtual						~CBasePlayer( ) = 0;

	virtual void				SharedSpawn( ) = 0;
	virtual bool				GetSteamID( CSteamID *pID ) = 0;
	virtual float				GetPlayerMaxSpeed( ) = 0;
	virtual void				CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov ) = 0;
	virtual void				CalcViewModelView( const Vector &eyeOrigin, const QAngle &eyeAngles ) = 0;
	virtual float				CalcRoll( const QAngle &angles, const Vector &velocity, float rollangle, float rollspeed ) = 0;
	virtual void				SetPlayerUnderwater( bool state ) = 0;
	virtual Vector				Weapon_ShootPosition( ) = 0;
	virtual void				Weapon_DropPrimary( void ) = 0;
	virtual Vector				GetAutoaimVector( float flScale ) = 0;
	virtual bool				CreateMove( float flInputSampleTime, CUserCmd *pCmd ) = 0;
	virtual void				AvoidPhysicsProps( CUserCmd *pCmd ) = 0;
	virtual void				PlayerUse( void ) = 0;
	virtual bool				IsUseableEntity( CBaseEntity *pEntity, unsigned int requiredCaps ) = 0;
	virtual int					GetObserverMode( ) const = 0;
	virtual CBaseEntity *GetObserverTarget( ) const = 0;
	virtual IRagdoll *GetRepresentativeRagdoll( ) const = 0;
	virtual void				TeamChange( int iNewTeam ) = 0;
	virtual bool				IsAllowedToSwitchWeapons( void ) = 0;
	virtual CBaseCombatWeapon *GetActiveWeaponForSelection( void ) = 0;
	virtual CBaseAnimating *GetRenderedWeaponModel( ) = 0;
	virtual bool				IsOverridingViewmodel( void ) = 0;
	virtual int					DrawOverriddenViewmodel( C_BaseViewModel *pViewmodel, int flags ) = 0;
	virtual float				GetDefaultAnimSpeed( void ) = 0;
	virtual void				ThirdPersonSwitch( bool bThirdperson ) = 0;
	virtual bool				CanSetSoundMixer( void ) = 0;
	virtual int					GetVisionFilterFlags( bool bWeaponsCheck = false ) = 0;
	virtual void				CalculateVisionUsingCurrentFlags( void ) = 0;
	virtual bool				ViewModel_IsTransparent( void ) = 0;
	virtual bool				ViewModel_IsUsingFBTexture( void ) = 0;
	virtual void				PreThink( void ) = 0;
	virtual void				PostThink( void ) = 0;
	virtual void				ItemPreFrame( void ) = 0;
	virtual void				ItemPostFrame( void ) = 0;
	virtual void				AbortReload( void ) = 0;
	virtual void				SelectLastItem( void ) = 0;
	virtual void				Weapon_SetLast( CBaseCombatWeapon *pWeapon ) = 0;
	virtual bool				Weapon_ShouldSetLast( CBaseCombatWeapon *pOldWeapon, CBaseCombatWeapon *pNewWeapon ) = 0;
	virtual bool				Weapon_ShouldSelectItem( CBaseCombatWeapon *pWeapon ) = 0;
	virtual CBaseCombatWeapon *GetLastWeapon( void ) = 0;
	virtual void 				SelectItem( const char *pstr, int iSubType = 0 ) = 0;
	virtual void				UpdateClientData( void ) = 0;
	virtual float				GetFOV( void ) = 0;
	virtual bool				IsZooming( void ) = 0;
	virtual void				ViewPunch( const QAngle &angleOffset ) = 0;
	virtual void				OverrideView( CViewSetup *pSetup ) = 0;
	virtual const Vector		GetPlayerMins( void ) const = 0;
	virtual const Vector		GetPlayerMaxs( void ) const = 0;
	virtual void	            SetVehicleRole( int nRole ) = 0;
	virtual void			    SetAnimation( PLAYER_ANIM playerAnim ) = 0;
	virtual float				GetMinFOV( ) const = 0;
	virtual void				PlayPlayerJingle( ) = 0;
	virtual void				UpdateStepSound( surfacedata_t *psurface, const Vector &vecOrigin, const Vector &vecVelocity ) = 0;
	virtual void				PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force ) = 0;
	virtual surfacedata_t *GetFootstepSurface( const Vector &origin, const char *surfaceName ) = 0;
	virtual void				GetStepSoundVelocities( float *velwalk, float *velrun ) = 0;
	virtual void				SetStepSoundTime( int iStepSoundTime, bool bWalking ) = 0; //stepsoundtimes_t
	virtual const char *GetOverrideStepSound( const char *pszBaseStepSoundName ) = 0;
	virtual void				OnEmitFootstepSound( const CSoundParameters &params, const Vector &vecOrigin, float fVolume ) = 0;
	virtual void				ExitLadder( ) = 0;
	virtual CHintSystem *Hints( void ) = 0;
	virtual	IMaterial *GetHeadLabelMaterial( void ) = 0;
	virtual void				OnAchievementAchieved( int iAchievement ) = 0;
	virtual void			    UpdateWearables( ) = 0;
	virtual bool			    CanUseFirstPersonCommand( void ) = 0;
	virtual void				CalcObserverView( Vector &eyeOrigin, QAngle &eyeAngles, float &fov ) = 0;
	virtual Vector				GetChaseCamViewOffset( CBaseEntity *target ) = 0;
	virtual void				CalcInEyeCamView( Vector &eyeOrigin, QAngle &eyeAngles, float &fov ) = 0;
	virtual float				GetDeathCamInterpolationTime( ) = 0;
	virtual void				CalcDeathCamView( Vector &eyeOrigin, QAngle &eyeAngles, float &fov ) = 0;
	virtual void				CalcFreezeCamView( Vector &eyeOrigin, QAngle &eyeAngles, float &fov ) = 0;
	virtual void				SetLocalViewAngles( const QAngle &viewAngles ) = 0;
	virtual void				SetViewAngles( const QAngle &ang ) = 0;
	virtual bool				IsDucked( void ) const = 0;
	virtual bool				IsDucking( void ) const = 0;
	virtual float				GetFallVelocity( ) = 0;

public:
	NETVAR(m_Local, void *, "CBasePlayer", "m_Local");
	NETVAR(m_chAreaBits, void *, "CBasePlayer", "m_chAreaBits");
	NETVAR(m_chAreaPortalBits, void *, "CBasePlayer", "m_chAreaPortalBits");
	NETVAR(m_iHideHUD, int, "CBasePlayer", "m_iHideHUD");
	NETVAR(m_flFOVRate, float, "CBasePlayer", "m_flFOVRate");
	NETVAR(m_bDucked, bool, "CBasePlayer", "m_bDucked");
	NETVAR(m_bDucking, bool, "CBasePlayer", "m_bDucking");
	NETVAR(m_bInDuckJump, bool, "CBasePlayer", "m_bInDuckJump");
	NETVAR(m_flDucktime, float, "CBasePlayer", "m_flDucktime");
	NETVAR(m_flDuckJumpTime, float, "CBasePlayer", "m_flDuckJumpTime");
	NETVAR(m_flJumpTime, float, "CBasePlayer", "m_flJumpTime");
	NETVAR(m_flFallVelocity, float, "CBasePlayer", "m_flFallVelocity");
	NETVAR(m_vecPunchAngle, Vec3, "CBasePlayer", "m_vecPunchAngle");
	NETVAR(m_vecPunchAngleVel, Vec3, "CBasePlayer", "m_vecPunchAngleVel");
	NETVAR(m_bDrawViewmodel, bool, "CBasePlayer", "m_bDrawViewmodel");
	NETVAR(m_bWearingSuit, bool, "CBasePlayer", "m_bWearingSuit");
	NETVAR(m_bPoisoned, bool, "CBasePlayer", "m_bPoisoned");
	NETVAR(m_flStepSize, float, "CBasePlayer", "m_flStepSize");
	NETVAR(m_bAllowAutoMovement, bool, "CBasePlayer", "m_bAllowAutoMovement");
	NETVAR(m_vecViewOffset, Vec3, "CBasePlayer", "m_vecViewOffset[0]");
	NETVAR(m_flFriction, float, "CBasePlayer", "m_flFriction");
	NETVAR(m_iAmmo, void *, "CBasePlayer", "m_iAmmo");
	NETVAR(m_fOnTarget, int, "CBasePlayer", "m_fOnTarget");
	NETVAR(m_nTickBase, int, "CBasePlayer", "m_nTickBase");
	NETVAR(m_nNextThinkTick, int, "CBasePlayer", "m_nNextThinkTick");
	NETVAR(m_hLastWeapon, int, "CBasePlayer", "m_hLastWeapon");
	NETVAR(m_hGroundEntity, int, "CBasePlayer", "m_hGroundEntity");
	NETVAR(m_vecVelocity, Vec3, "CBasePlayer", "m_vecVelocity[0]");
	NETVAR(m_vecBaseVelocity, Vec3, "CBasePlayer", "m_vecBaseVelocity");
	NETVAR(m_hConstraintEntity, int, "CBasePlayer", "m_hConstraintEntity");
	NETVAR(m_vecConstraintCenter, Vec3, "CBasePlayer", "m_vecConstraintCenter");
	NETVAR(m_flConstraintRadius, float, "CBasePlayer", "m_flConstraintRadius");
	NETVAR(m_flConstraintWidth, float, "CBasePlayer", "m_flConstraintWidth");
	NETVAR(m_flConstraintSpeedFactor, float, "CBasePlayer", "m_flConstraintSpeedFactor");
	NETVAR(m_flDeathTime, float, "CBasePlayer", "m_flDeathTime");
	NETVAR(m_nWaterLevel, BYTE, "CBasePlayer", "m_nWaterLevel");
	NETVAR(m_flLaggedMovementValue, float, "CBasePlayer", "m_flLaggedMovementValue");
	NETVAR(m_AttributeList, void *, "CBasePlayer", "m_AttributeList");
	NETVAR(pl, void *, "CBasePlayer", "pl");
	NETVAR(deadflag, int, "CBasePlayer", "deadflag");
	NETVAR(m_iFOV, int, "CBasePlayer", "m_iFOV");
	NETVAR(m_iFOVStart, int, "CBasePlayer", "m_iFOVStart");
	NETVAR(m_flFOVTime, float, "CBasePlayer", "m_flFOVTime");
	NETVAR(m_iDefaultFOV, int, "CBasePlayer", "m_iDefaultFOV");
	NETVAR(m_hZoomOwner, int, "CBasePlayer", "m_hZoomOwner");
	NETVAR(m_hVehicle, int, "CBasePlayer", "m_hVehicle");
	NETVAR(m_hUseEntity, int, "CBasePlayer", "m_hUseEntity");
	NETVAR(m_iHealth, int, "CBasePlayer", "m_iHealth");
	NETVAR(m_lifeState, byte, "CBasePlayer", "m_lifeState");
	NETVAR(m_iBonusProgress, int, "CBasePlayer", "m_iBonusProgress");
	NETVAR(m_iBonusChallenge, int, "CBasePlayer", "m_iBonusChallenge");
	NETVAR(m_flMaxspeed, float, "CBasePlayer", "m_flMaxspeed");
	NETVAR(m_fFlags, int, "CBasePlayer", "m_fFlags");
	NETVAR(m_iObserverMode, int, "CBasePlayer", "m_iObserverMode");
	NETVAR(m_hObserverTarget, int, "CBasePlayer", "m_hObserverTarget");
	NETVAR(m_hViewModel, int, "CBasePlayer", "m_hViewModel[0]");
	NETVAR(m_szLastPlaceName, const char *, "CBasePlayer", "m_szLastPlaceName");

	M_CONDGET(OnGround, m_fFlags(), FL_ONGROUND)
	M_CONDGET(InWater, m_fFlags(), FL_INWATER)
	M_CONDGET(Ducking, m_fFlags(), FL_DUCKING)

	inline void SetFov(const int nFov) {
		DYNVAR_SET(int, this, nFov, "CBasePlayer", "m_iFOV");
	}

	inline int& m_nFinalTick( )
	{
		DWORD dwOffset = g_NetVars.get_offset( "DT_BasePlayer", "localdata", "m_nTickBase" );
		return *reinterpret_cast< int * >( reinterpret_cast<DWORD>(this) + dwOffset + 0x4 );
	}

	/*inline void SetTickBase(const int nTickBase) {
		m_nTickBase() = nTickBase;
	}

	inline void SetCurrentCmd(CUserCmd* pCmd) {
		m_hConstraintEntity() = reinterpret_cast<int>(pCmd);
	}*/

	/*M_DYNVARGET(CurrentCommand, CUserCmd*, (this - 0x4), ("DT_BasePlayer"), ("localdata"), ("m_hConstraintEntity"))
	M_DYNVARGET(TickBase, int, this, ("DT_BasePlayer"), ("localdata"), ("m_nTickBase"))

	inline void SetCurrentCommand(CUserCmd* pCmd) {
		static int nOffset = g_NetVars.get_offset("CBasePlayer", "m_hConstraintEntity") - 0x4;
		*reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + nOffset) = pCmd;
	}

	inline void SetCurrentTickbase(const int nTickBase) {
		static int nOffset = g_NetVars.get_offset("CBasePlayer", "m_nTickBase");
		*reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + nOffset) = nTickBase;
	}*/
};