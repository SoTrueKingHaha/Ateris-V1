#pragma once
#include "baseprojectile.h"
#include "../../../Utils/VFunc/VFunc.h"
#include "Activity.h"

class CHudTexture;
class WeaponProficiencyInfo_t;
class CBasePlayer;
class C_BaseViewModel;
typedef struct
{
	int			baseAct;
	int			weaponAct;
	bool		required;
} acttable_t;

class CBaseCombatWeapon : public CBaseAnimating
{
public:
	virtual						~CBaseCombatWeapon( ) = 0;

private:
	//TODO: Check what the fuck this is, or where it's even missing from.
	virtual void *__pad00( ) = 0;

public:
	virtual bool				IsPredicted( void ) const = 0;
	virtual int					GetSubType( void ) = 0;
	virtual void				SetSubType( int iType ) = 0;
	virtual void				Equip( CBaseCombatCharacter *pOwner ) = 0;
	virtual void				Drop( const Vector &vecVelocity ) = 0;
	virtual	int					UpdateClientData( CBasePlayer *pPlayer ) = 0;
	virtual bool				IsAllowedToSwitch( void ) = 0;
	virtual bool				CanBeSelected( void ) = 0;
	virtual bool				VisibleInWeaponSelection( void ) = 0;
	virtual bool				HasAmmo( void ) = 0;
	virtual void				SetPickupTouch( void ) = 0;
	virtual void 				DefaultTouch( CBaseEntity *pOther ) = 0;
	virtual void				GiveTo( CBaseEntity *pOther ) = 0;
	virtual bool				ShouldDisplayAltFireHUDHint( ) = 0;
	virtual void				DisplayAltFireHudHint( ) = 0;
	virtual void				RescindAltFireHudHint( ) = 0;
	virtual bool				ShouldDisplayReloadHUDHint( ) = 0;
	virtual void				DisplayReloadHudHint( ) = 0;
	virtual void				RescindReloadHudHint( ) = 0;
	virtual void				SetViewModelIndex( int index = 0 ) = 0;
	virtual bool				SendWeaponAnim( int iActivity ) = 0;
	virtual void				SendViewModelAnim( int nSequence ) = 0;
	virtual void				SetViewModel( ) = 0;
	virtual bool				HasWeaponIdleTimeElapsed( void ) = 0;
	virtual void				SetWeaponIdleTime( float time ) = 0;
	virtual float				GetWeaponIdleTime( void ) = 0;
	virtual bool				HasAnyAmmo( void ) = 0;
	virtual bool				HasPrimaryAmmo( void ) = 0;
	virtual bool				HasSecondaryAmmo( void ) = 0;
	virtual bool				CanHolster( void ) const = 0;
	virtual bool				DefaultDeploy( char *szViewModel, char *szWeaponModel, int iActivity, char *szAnimExt ) = 0;
	virtual bool				CanDeploy( void ) = 0;
	virtual bool				Deploy( void ) = 0;
	virtual bool				Holster( CBaseCombatWeapon *pSwitchingTo = NULL ) = 0;
	virtual CBaseCombatWeapon *GetLastWeapon( void ) = 0;
	virtual void				SetWeaponVisible( bool visible ) = 0;
	virtual bool				IsWeaponVisible( void ) = 0;
	virtual bool				ReloadOrSwitchWeapons( void ) = 0;
	virtual void				OnActiveStateChanged( int iOldState ) = 0;
	virtual bool				HolsterOnDetach( ) = 0;
	virtual bool				IsHolstered( ) = 0;
	virtual void				Detach( ) = 0;
	virtual void				ItemPreFrame( void ) = 0;
	virtual void				ItemPostFrame( void ) = 0;
	virtual void				ItemBusyFrame( void ) = 0;
	virtual void				ItemHolsterFrame( void ) = 0;
	virtual void				WeaponIdle( void ) = 0;
	virtual void				HandleFireOnEmpty( ) = 0;
	virtual bool				CanPerformSecondaryAttack( ) const = 0;
	virtual bool				ShouldBlockPrimaryFire( ) = 0;
	virtual void				CreateMove( float flInputSampleTime, CUserCmd *pCmd, const QAngle &vecOldViewAngles ) = 0;
	virtual bool				IsWeaponZoomed( ) = 0;
	virtual	void				CheckReload( void ) = 0;
	virtual void				FinishReload( void ) = 0;
	virtual void				AbortReload( void ) = 0;
	virtual bool				Reload( void ) = 0;
	virtual bool				AutoFiresFullClip( void ) const = 0;
	virtual void				UpdateAutoFire( void ) = 0;
	virtual void				PrimaryAttack( void ) = 0;
	virtual void				SecondaryAttack( void ) = 0;
	virtual Activity			GetPrimaryAttackActivity( void ) = 0;
	virtual Activity			GetSecondaryAttackActivity( void ) = 0;
	virtual Activity			GetDrawActivity( void ) = 0;
	virtual float				GetDefaultAnimSpeed( void ) = 0;
	virtual int					GetBulletType( void ) = 0;
	virtual const Vector &GetBulletSpread( void ) = 0;
	virtual Vector				GetBulletSpread( int proficiency ) = 0; //WeaponProficiency_t
	virtual float				GetSpreadBias( int proficiency ) = 0; //WeaponProficiency_t
	virtual float				GetFireRate( void ) = 0;
	virtual int					GetMinBurst( ) = 0;
	virtual int					GetMaxBurst( ) = 0;
	virtual float				GetMinRestTime( ) = 0;
	virtual float				GetMaxRestTime( ) = 0;
	virtual int					GetRandomBurst( ) = 0;
	virtual void				WeaponSound( int sound_type, float soundtime = 0.0f ) = 0; //WeaponSound_t
	virtual void				StopWeaponSound( int sound_type ) = 0; //WeaponSound_t
	virtual const WeaponProficiencyInfo_t *GetProficiencyValues( ) = 0;
	virtual float				GetMaxAutoAimDeflection( ) = 0;
	virtual float				WeaponAutoAimScale( ) = 0;
	virtual bool				StartSprinting( void ) = 0;
	virtual bool				StopSprinting( void ) = 0;
	virtual float				GetDamage( float flDistance, int iLocation ) = 0;
	virtual void				SetActivity( Activity act, float duration ) = 0;
	virtual void				AddViewKick( void ) = 0;
	virtual char *GetDeathNoticeName( void ) = 0;
	virtual void				OnPickedUp( CBaseCombatCharacter *pNewOwner ) = 0;
	virtual void				AddViewmodelBob( C_BaseViewModel *viewmodel, Vector &origin, QAngle &angles ) = 0;
	virtual float				CalcViewmodelBob( void ) = 0;
	virtual void 				GetControlPanelInfo( int nPanelIndex, const char *&pPanelName ) = 0;
	virtual void				GetControlPanelClassName( int nPanelIndex, const char *&pPanelName ) = 0;
	virtual bool				ShouldShowControlPanels( void ) = 0;
	virtual bool				CanBePickedUpByNPCs( void ) = 0;
	virtual int					GetSkinOverride( ) const = 0;
	virtual const char *GetViewModel( int viewmodelindex = 0 ) const = 0;
	virtual const char *GetWorldModel( void ) const = 0;
	virtual const char *GetAnimPrefix( void ) const = 0;
	virtual int					GetMaxClip1( void ) const = 0;
	virtual int					GetMaxClip2( void ) const = 0;
	virtual int					GetDefaultClip1( void ) const = 0;
	virtual int					GetDefaultClip2( void ) const = 0;
	virtual int					GetWeight( void ) const = 0;
	virtual bool				AllowsAutoSwitchTo( void ) const = 0;
	virtual bool				AllowsAutoSwitchFrom( void ) const = 0;
	virtual bool				ForceWeaponSwitch( void ) const = 0;
	virtual int					GetWeaponFlags( void ) const = 0;
	virtual int					GetSlot( void ) const = 0;
	virtual int					GetPosition( void ) const = 0;
	virtual char const *GetWeaponName( void ) const = 0;
	virtual char const *GetPrintName( void ) const = 0;
	virtual char const *GetShootSound( int iIndex ) const = 0;
	virtual int					GetRumbleEffect( ) const = 0;
	virtual bool				UsesClipsForAmmo1( void ) const = 0;
	virtual bool				UsesClipsForAmmo2( void ) const = 0;
	virtual const unsigned char *GetEncryptionKey( void ) = 0;
	virtual int					GetPrimaryAmmoType( void ) const = 0;
	virtual int					GetSecondaryAmmoType( void ) const = 0;
	virtual int					Clip1( ) = 0;
	virtual int					Clip2( ) = 0;
	virtual CHudTexture const *GetSpriteActive( void ) const = 0;
	virtual CHudTexture const *GetSpriteInactive( void ) const = 0;
	virtual CHudTexture const *GetSpriteAmmo( void ) const = 0;
	virtual CHudTexture const *GetSpriteAmmo2( void ) const = 0;
	virtual CHudTexture const *GetSpriteCrosshair( void ) const = 0;
	virtual CHudTexture const *GetSpriteAutoaim( void ) const = 0;
	virtual CHudTexture const *GetSpriteZoomedCrosshair( void ) const = 0;
	virtual CHudTexture const *GetSpriteZoomedAutoaim( void ) const = 0;
	virtual Activity			ActivityOverride( Activity baseAct, bool *pRequired ) = 0;
	virtual	acttable_t *ActivityList( int &iActivityCount ) = 0;
	virtual bool				ShouldUseLargeViewModelVROverride( ) = 0;
	virtual void				RestartParticleEffect( void ) = 0;
	virtual void				Redraw( void ) = 0;
	virtual void				ViewModelDrawn( C_BaseViewModel *pViewModel ) = 0;
	virtual void				DrawCrosshair( void ) = 0;
	virtual bool				ShouldDrawCrosshair( void ) = 0;
	virtual bool				IsCarriedByLocalPlayer( void ) = 0;
	virtual bool				ShouldDrawUsingViewModel( void ) = 0;
	virtual bool				IsActiveByLocalPlayer( void ) = 0;
	virtual bool				ShouldDrawPickup( void ) = 0;
	virtual void				HandleInput( void ) = 0;
	virtual void				OverrideMouseInput( float *x, float *y ) = 0;
	virtual int					KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding ) = 0;
	virtual bool				AddLookShift( void ) = 0;
	virtual void				GetViewmodelBoneControllers( C_BaseViewModel *pViewModel, float controllers[ 4 ] ) = 0;
	virtual int					GetWorldModelIndex( void ) = 0;
	virtual void				GetWeaponCrosshairScale( float &flScale ) = 0;
	virtual bool				CanLower( void ) = 0;
	virtual bool				Ready( void ) = 0;
	virtual bool				Lower( void ) = 0;
	virtual void				HideThink( void ) = 0;
	virtual bool				CanReload( void ) = 0;
	virtual float				GetNextSecondaryAttackDelay( ) = 0; //I just guessed float, so might be incorrect.
	virtual const Vector &GetViewmodelOffset( ) = 0;

private:
	virtual void *NetworkStateChanged_m_nNextThinkTick( void ) = 0;
	virtual void *NetworkStateChanged_m_nNextThinkTick( void *pUnknown ) = 0;

public:
	typedef CHandle<CBaseCombatCharacter> CBaseCombatCharacterHandle;
public:
	NETVAR(m_iClip1, int, "CBaseCombatWeapon", "m_iClip1");
	NETVAR(m_iClip2, int, "CBaseCombatWeapon", "m_iClip2");
	NETVAR(m_iItemDefinitionIndex, int, "CEconEntity", "m_iItemDefinitionIndex");
	NETVAR(m_iPrimaryAmmoType, int, "CBaseCombatWeapon", "m_iPrimaryAmmoType");
	NETVAR(m_iSecondaryAmmoType, int, "CBaseCombatWeapon", "m_iSecondaryAmmoType");
	NETVAR(m_nViewModelIndex, int, "CBaseCombatWeapon", "m_nViewModelIndex");
	NETVAR(m_bFlipViewModel, bool, "CBaseCombatWeapon", "m_bFlipViewModel");
	NETVAR(m_flNextPrimaryAttack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(m_flNextSecondaryAttack, float, "CBaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(m_bInReload, bool, "CBaseCombatWeapon", "m_bInReload");
	NETVAR(m_nNextThinkTick, int, "CBaseCombatWeapon", "m_nNextThinkTick");
	NETVAR(m_flTimeWeaponIdle, float, "CBaseCombatWeapon", "m_flTimeWeaponIdle");
	NETVAR(m_iViewModelIndex, int, "CBaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(m_iWorldModelIndex, int, "CBaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(m_iState, int, "CBaseCombatWeapon", "m_iState");
	NETVAR(m_hOwner, int, "CBaseCombatWeapon", "m_hOwner");

	inline float GetSmackTime( )
	{
		//credits to KGB
		static auto dwOffset = g_NetVars.get_offset( "DT_TFWeaponBase", "m_nInspectStage" ) + 0x1C;
		return *reinterpret_cast< float * >( this + dwOffset );
	}

	inline bool GetAttachment( int number, Vec3 &origin ) {
		return GetVFunc<bool( __thiscall * )( void *, int, Vec3 & )>( this, 71 )( this, number, origin );
	}

	int GetBulletAmount( );
};