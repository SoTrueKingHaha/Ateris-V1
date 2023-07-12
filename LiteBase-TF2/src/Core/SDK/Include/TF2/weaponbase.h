#pragma once
#include "basecombatweapon.h"
#include "GameEventListener.h"
#include "ihasowner.h"
#include "globalvars_base.h"
#include "tf_player.h"
#include "../../../Utils/VFunc/VFunc.h"
#include "globalinfo.h"

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

inline DWORD calcisattackcritical = 0;

struct WeaponData_t
{
	int		m_nDamage;
	int		m_nBulletsPerShot;
	float	m_flRange;
	float	m_flSpread;
	float	m_flPunchAngle;
	float	m_flTimeFireDelay;				// Time to delay between firing
	float	m_flTimeIdle;					// Time to idle after firing
	float	m_flTimeIdleEmpty;				// Time to idle after firing last bullet in clip
	float	m_flTimeReloadStart;			// Time to start into a reload (ie. shotgun)
	float	m_flTimeReload;					// Time to reload
	bool	m_bDrawCrosshair;				// Should the weapon draw a crosshair
	int		m_iProjectile;					// The type of projectile this mode fires
	int		m_iAmmoPerShot;					// How much ammo each shot consumes
	float	m_flProjectileSpeed;			// Start speed for projectiles (nail, etc.); NOTE: union with something non-projectile
	float	m_flSmackDelay;					// how long after swing should damage happen for melee weapons
	bool	m_bUseRapidFireCrits;

	void Init(void)
	{
		m_nDamage = 0;
		m_nBulletsPerShot = 0;
		m_flRange = 0.0f;
		m_flSpread = 0.0f;
		m_flPunchAngle = 0.0f;
		m_flTimeFireDelay = 0.0f;
		m_flTimeIdle = 0.0f;
		m_flTimeIdleEmpty = 0.0f;
		m_flTimeReloadStart = 0.0f;
		m_flTimeReload = 0.0f;
		m_iProjectile = 0;
		m_iAmmoPerShot = 0;
		m_flProjectileSpeed = 0.0f;
		m_flSmackDelay = 0.0f;
		m_bUseRapidFireCrits = false;
	};
};

typedef enum {
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	DEPLOY,

	// Add new shoot sound types here

	NUM_SHOOT_SOUND_TYPES,
} WeaponSound_t;

class CFileWeaponInfo
{
public:

	CFileWeaponInfo();

	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse(void* pKeyValuesData, const char* szWeaponName);


public:
	bool					bParsedScript;
	bool					bLoadedHudElements;

	// SHARED
	char					szClassName[MAX_WEAPON_STRING];
	char					szPrintName[MAX_WEAPON_STRING];			// Name for showing in HUD, etc.

	char					szViewModel[MAX_WEAPON_STRING];			// View model of this weapon
	char					szWorldModel[MAX_WEAPON_STRING];		// Model of this weapon seen carried by the player
	char					szAnimationPrefix[MAX_WEAPON_PREFIX];	// Prefix of the animations that should be used by the player carrying this weapon
	int						iSlot;									// inventory slot.
	int						iPosition;								// position in the inventory slot.
	int						iMaxClip1;								// max primary clip size (-1 if no clip)
	int						iMaxClip2;								// max secondary clip size (-1 if no clip)
	int						iDefaultClip1;							// amount of primary ammo in the gun when it's created
	int						iDefaultClip2;							// amount of secondary ammo in the gun when it's created
	int						iWeight;								// this value used to determine this weapon's importance in autoselection.
	int						iRumbleEffect;							// Which rumble effect to use when fired? (xbox)
	bool					bAutoSwitchTo;							// whether this weapon should be considered for autoswitching to
	bool					bAutoSwitchFrom;						// whether this weapon can be autoswitched away from when picking up another weapon or ammo
	int						iFlags;									// miscellaneous weapon flags
	char					szAmmo1[MAX_WEAPON_AMMO_NAME];			// "primary" ammo type
	char					szAmmo2[MAX_WEAPON_AMMO_NAME];			// "secondary" ammo type

	// Sound blocks
	char					aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int						iAmmoType;
	int						iAmmo2Type;
	bool					m_bMeleeWeapon;		// Melee weapons can always "fire" regardless of ammo.

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool					m_bBuiltRightHanded;
	bool					m_bAllowFlipping;	// False to disallow flipping the model, regardless of whether
												// it is built left or right handed.

// CLIENT DLL
	// Sprite data, read from the data file
	int						iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;

	// TF2 specific
	bool					bShowUsageHint;							// if true, then when you receive the weapon, show a hint about it

// SERVER DLL

};

class CTFWeaponInfo : public CFileWeaponInfo
{
public:

	CTFWeaponInfo();
	~CTFWeaponInfo();

	virtual void Parse(void* pKeyValuesData, const char* szWeaponName);

	WeaponData_t const& GetWeaponData(int iWeapon) const { return m_WeaponData[iWeapon]; }

public:

	WeaponData_t	m_WeaponData[2];

	int		m_iWeaponType;

	// Grenade.
	bool	m_bGrenade;
	float	m_flDamageRadius;
	float	m_flPrimerTime;
	bool	m_bLowerWeapon;
	bool	m_bSuppressGrenTimer;

	// Skins
	bool	m_bHasTeamSkins_Viewmodel;
	bool	m_bHasTeamSkins_Worldmodel;

	// Muzzle flash
	char	m_szMuzzleFlashModel[128];
	float	m_flMuzzleFlashModelDuration;
	char	m_szMuzzleFlashParticleEffect[128];

	// Tracer
	char	m_szTracerEffect[128];

	// Eject Brass
	bool	m_bDoInstantEjectBrass;
	char	m_szBrassModel[128];

	// Explosion Effect
	char	m_szExplosionSound[128];
	char	m_szExplosionEffect[128];
	char	m_szExplosionPlayerEffect[128];
	char	m_szExplosionWaterEffect[128];

	bool	m_bDontDrop;
};

class CTFWeaponBase : public CBaseCombatWeapon, public IHasOwner, public CGameEventListener
{
public:
	NETVAR(m_bLowered, bool, "CTFWeaponBase", "m_bLowered");
	NETVAR(m_iReloadMode, int, "CTFWeaponBase", "m_iReloadMode");
	NETVAR(m_bResetParity, bool, "CTFWeaponBase", "m_bResetParity");
	NETVAR(m_bReloadedThroughAnimEvent, bool, "CTFWeaponBase", "m_bReloadedThroughAnimEvent");
	NETVAR(m_bDisguiseWeapon, bool, "CTFWeaponBase", "m_bDisguiseWeapon");
	NETVAR(m_flLastCritCheckTime, float, "CTFWeaponBase", "m_flLastCritCheckTime");
	NETVAR(m_flReloadPriorNextFire, float, "CTFWeaponBase", "m_flReloadPriorNextFire");
	NETVAR(m_flLastFireTime, float, "CTFWeaponBase", "m_flLastFireTime");
	NETVAR(m_flEffectBarRegenTime, float, "CTFWeaponBase", "m_flEffectBarRegenTime");
	NETVAR(m_flObservedCritChance, float, "CTFWeaponBase", "m_flObservedCritChance");
	NETVAR(m_flEnergy, float, "CTFWeaponBase", "m_flEnergy");
	NETVAR(m_hExtraWearable, int, "CTFWeaponBase", "m_hExtraWearable");
	NETVAR(m_hExtraWearableViewModel, int, "CTFWeaponBase", "m_hExtraWearableViewModel");
	NETVAR(m_bBeingRepurposedForTaunt, bool, "CTFWeaponBase", "m_bBeingRepurposedForTaunt");
	NETVAR(m_nKillComboClass, int, "CTFWeaponBase", "m_nKillComboClass");
	NETVAR(m_nKillComboCount, int, "CTFWeaponBase", "m_nKillComboCount");
	NETVAR(m_flInspectAnimEndTime, float, "CTFWeaponBase", "m_flInspectAnimEndTime");
	NETVAR(m_nInspectStage, int, "CTFWeaponBase", "m_nInspectStage");
	NETVAR(m_iConsecutiveShots, int, "CTFWeaponBase", "m_iConsecutiveShots");

	inline int GetSlot() {
		using fn = int(__thiscall *)(void *);
		fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(this, 330));
		return FN(this);
	}

	inline int GetWeaponID() {
		using fn = int(__thiscall *)(void *);
		fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(this, 381));
		return FN(this);
	}

	/*inline int GetName( ) {
		using fn = int(__thiscall*)(void*);
		fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(this, 331));
		return FN(this);
	}*/

	inline int GetDamageType() {
		using fn = int(__thiscall *)(void *);
		fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(this, 340));
		return FN(this);
	}

	inline bool IsInReload() {
		return (m_bInReload() || m_iReloadMode() != 0);
	}

	inline WeaponData_t GetWeaponData() {
		static int offset = g_Pattern.Find(L"client.dll", L"55 8B EC 66 8B ? ? 66 3B 05 ? ? ? ? 73");
		static auto get_tf_weapon_data_fn = reinterpret_cast<CTFWeaponInfo * (__cdecl*)(int)>(offset);
		return get_tf_weapon_data_fn(GetWeaponID())->m_WeaponData[0];
	}

	inline CTFWeaponInfo* GetWeaponInfo( )
	{
		static int offset = g_Pattern.Find( L"client.dll", L"55 8B EC 66 8B ? ? 66 3B 05 ? ? ? ? 73" );
		static auto get_tf_weapon_data_fn = reinterpret_cast< CTFWeaponInfo * ( __cdecl * )( int ) >( offset );
		return get_tf_weapon_data_fn( GetWeaponID( ) );
	}

	inline bool CalcIsAttackCritical( )
	{
		static auto func = g_Pattern.Find( L"client.dll", L"53 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B D8 83 C4 14 85 DB 0F 84 ? ? ? ?" );
		typedef bool( __thiscall *fn )( void * );
		return reinterpret_cast< fn >( func )( this );
	}

	inline int &m_iWeaponMode( )
	{
		static DWORD offset = g_NetVars.get_offset( "DT_TFWeaponBase", "m_iReloadMode" ) - 0x4;
		return *reinterpret_cast< int * >( reinterpret_cast< DWORD >( this ) + offset );
	}

	__inline bool CalcIsAttackCriticalHelper( )
	{
		using FN = bool( __thiscall * )( CTFWeaponBase * );
		static FN pCalcIsAttackCriticalHelper = reinterpret_cast< FN >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 18 56 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 14 89 75 EC" ) );
		if ( !pCalcIsAttackCriticalHelper )
		{
			pCalcIsAttackCriticalHelper = ( FN )calcisattackcritical;
			return false;
		}
		return pCalcIsAttackCriticalHelper( this );
	}

	__inline bool CalcIsAttackCriticalHelperMelee( )
	{
		using FN = bool( __thiscall * )( CTFWeaponBase * );
		static FN pCalcIsAttackCriticalHelper = reinterpret_cast< FN >( g_Pattern.Find( L"client.dll", L"55 8B EC A1 ? ? ? ? 83 EC 08 83 78 30 00 57" ) );
		return pCalcIsAttackCriticalHelper( this );
	}

	__inline bool CalcIsAttackCriticalHelperNoCrits( CTFWeaponBase *pWeapon )
	{
		typedef bool ( *fn_t )( CTFWeaponBase * );
		return GetVFunc<fn_t>( pWeapon, 463, 0 )( pWeapon );
	}

	inline bool WillCrit()
	{
		return this->GetSlot() == SLOT_MELEE ? this->CalcIsAttackCriticalHelperMelee() : this->CalcIsAttackCriticalHelper();
	}

	inline float GetWeaponSpread() {
		static auto GetWeaponSpreadFn = reinterpret_cast<float(__thiscall*)(decltype(this))>(g_Pattern.Find(L"client.dll", L"55 8B EC 83 EC 08 56 8B F1 57 6A 01 6A 00 8B 96 ? ? ? ? 8B 86 ? ? ? ? C1 E2 06 56 68 ? ? ? ? 51"));
		return GetWeaponSpreadFn(this);
	}

	inline bool AmbassadorCanHeadshot()
	{
		if (m_iItemDefinitionIndex() == Spy_m_TheAmbassador || m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador)
		{
			if ((I::GlobalVars->curtime - m_flLastFireTime()) <= 1.0)
			{
				return false;
			}
		}
		return true;
	}

	/*inline bool CanFireCriticalShot(CTFPlayer* pOwner, bool bIsHeadshot = false)
	{
		bool bOut = false;

		//the fov shit might be only needed for when checking bIsHeadshot but whatever
		if (pOwner)
		{
			using fn = bool(__thiscall *)(void *, bool, void *);
			fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(this, 424));
			int &iFOV = pOwner->m_iFOV(), nFovBackup = iFOV;

			iFOV = 70;
			bOut = FN(this, bIsHeadshot, nullptr);
			iFOV = nFovBackup;
		}
	
		return bOut;
	}*/

	inline bool CanFireRandomCriticalShot(const float flCritChance) {
		return GetVFunc<bool(__thiscall*)(decltype(this), float)>(this, 423)(this, flCritChance);
	}

	inline bool CanHeadShot(CTFPlayer *pOwner)
	{
		bool bOut = false;

		if (pOwner)
			bOut = (GetDamageType() & DMG_USE_HITLOCATIONS) && CanFireCriticalShot(true);

		return bOut;
	}

	inline bool CanShoot(CTFPlayer* pLocal) {
		if (!pLocal->IsAlive()
			|| pLocal->InCond(TF_COND_TAUNTING)
			|| pLocal->InCond(TF_COND_STUNNED)
			//|| pLocal->GetFeignDeathReady()
			|| pLocal->InCond(TF_COND_STEALTHED)
			|| pLocal->InCond(TF_COND_HALLOWEEN_KART)
			|| pLocal->InCond(TF_COND_HALLOWEEN_GHOST_MODE))
			return false;

		if (pLocal->m_iClass() == SPY)
		{
			{ //DR
				static float flTimer = 0.0f;

				if (pLocal->m_bFeignDeathReady()) {
					flTimer = 0.0f;
					return false;
				}
				else {
					if (!flTimer)
						flTimer = I::GlobalVars->curtime;

					if ((I::GlobalVars->curtime - flTimer) < 0.4f)
						return false;
				}
			}

			{ //Invis
				static float flTimer = 0.0f;

				if (pLocal->InCond(TF_COND_STEALTHED)) {
					flTimer = 0.0f;
					return false;
				}
				else {
					if (!flTimer)
						flTimer = I::GlobalVars->curtime;

					if ((I::GlobalVars->curtime - flTimer) < 2.0f)
						return false;
				}
			}
		}

		float flCurTime = static_cast<float>(pLocal->m_nTickBase()) * I::GlobalVars->interval_per_tick;

		return m_flNextPrimaryAttack() <= flCurTime && pLocal->m_flNextAttack() <= flCurTime;
	}

	inline bool CanAttack(CTFPlayer* pLocal)
	{
		return CanPrimaryAttack(pLocal) && CanSecondaryAttack(pLocal);
	}

	inline bool CanPrimaryAttack(CTFPlayer *pOwner)
	{
		bool bOut = false;
		
		if (pOwner)
			bOut = m_flNextPrimaryAttack() < (static_cast<float>(pOwner->m_nTickBase()) * I::GlobalVars->interval_per_tick);

		return bOut;
	}

	inline bool CanSecondaryAttack(CTFPlayer *pOwner)
	{
		bool bOut = false;

		if (pOwner)
			bOut = m_flNextSecondaryAttack() < (static_cast<float>(pOwner->m_nTickBase()) * I::GlobalVars->interval_per_tick);

		return bOut;
	}

	inline bool CanFireCriticalShot(const bool bHeadShot) {
		bool bResult = false;
		if (const auto& pEntity = I::ClientEntityList->GetClientEntityFromHandle(m_hOwnerEntity())) {
			const auto& pOwner = pEntity->As<CBasePlayer>();
			const int nOldFov = pOwner->m_iFOV(); pOwner->m_iFOV() = -1;
			bResult = GetVFunc<bool(__thiscall*)(decltype(this), bool, CBaseEntity*)>(this, 425)(this, bHeadShot, nullptr);
			pOwner->m_iFOV() = nOldFov;
		} return bResult;
	}

	inline CAttributeList *GetAttributeList( ) {
		static auto dwOff = g_NetVars.get_offset( "DT_EconEntity", "m_AttributeManager", "m_AttributeList" );
		return reinterpret_cast< CAttributeList * >( reinterpret_cast<DWORD>( this ) + dwOff );
	}

	inline void SetItemDefIndex( const int nIndex ) {
		static auto dwOff = g_NetVars.get_offset( "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex" );
		*reinterpret_cast< int * >( reinterpret_cast<DWORD>( this ) + dwOff ) = nIndex;
	}
	/*inline float GetChargeDamage()
	{
		auto pWeapon = g_EntityCache.GetWeapon();
		auto pSniper = pWeapon->As<CTFSniperRifle>();
		return pSniper->m_flChargedDamage();
	}*/

	CHudTexture* GetWeaponIcon();
	std::wstring GetWeaponName( );
};

class CTFKnife : public CTFWeaponBase
{
public:
	NETVAR(m_bReadyToBackstab, bool, "CTFKnife", "m_bReadyToBackstab");
	NETVAR(m_bKnifeExists, bool, "CTFKnife", "m_bKnifeExists");
	NETVAR(m_flKnifeRegenerateDuration, float, "CTFKnife", "m_flKnifeRegenerateDuration");
	NETVAR(m_flKnifeMeltTimestamp, float, "CTFKnife", "m_flKnifeMeltTimestamp");
};

class CTFMinigun : public CTFWeaponBase
{
public:
	NETVAR(m_iWeaponState, int, "CTFMinigun", "m_iWeaponState");
	NETVAR(m_bCritShot, bool, "CTFMinigun", "m_bCritShot");
};

class CWeaponMedigun : public CTFWeaponBase
{
public:
	NETVAR(m_hHealingTarget, int, "CWeaponMedigun", "m_hHealingTarget");
	NETVAR(m_bHealing, bool, "CWeaponMedigun", "m_bHealing");
	NETVAR(m_bAttacking, bool, "CWeaponMedigun", "m_bAttacking");
	NETVAR(m_bChargeRelease, bool, "CWeaponMedigun", "m_bChargeRelease");
	NETVAR(m_bHolstered, bool, "CWeaponMedigun", "m_bHolstered");
	NETVAR(m_nChargeResistType, int, "CWeaponMedigun", "m_nChargeResistType");
	NETVAR(m_hLastHealingTarget, int, "CWeaponMedigun", "m_hLastHealingTarget");
	NETVAR(m_flChargeLevel, float, "CWeaponMedigun", "m_flChargeLevel");
};

class CTFPipebombLauncher : public CTFWeaponBase
{
public:
	NETVAR(m_iPipebombCount, int, "CTFPipebombLauncher", "m_iPipebombCount");
	NETVAR(m_flChargeBeginTime, float, "CTFPipebombLauncher", "m_flChargeBeginTime");
};

class CTFGrenadeLauncher : public CTFWeaponBase
{
public:
	NETVAR( m_flDetonateTime, float, "CTFGrenadeLauncher", "m_flDetonateTime" )
};

class CTFSniperRifle : public CTFWeaponBase
{
public:
	NETVAR(m_flChargedDamage, float, "CTFSniperRifle", "m_flChargedDamage");
};