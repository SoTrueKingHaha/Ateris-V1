#pragma once
#include "tf_mvm_boss_progress_user.h"
#include "econ_item_inventory.h"
#include "ihasattributes.h"
#include "globalvars_base.h"
#include "iclientmode.h"
#include <array>

class CTFWeaponBase;
class CTFPlayerAnimState;
class CTFPlayer : public CBasePlayer, public IHasAttributes, public IInventoryUpdateListener, public CTFMvMBossProgressUser
{
public:
	NETVAR(m_bSaveMeParity, bool, "CTFPlayer", "m_bSaveMeParity");
	NETVAR(m_bIsMiniBoss, bool, "CTFPlayer", "m_bIsMiniBoss");
	NETVAR(m_bIsABot, bool, "CTFPlayer", "m_bIsABot");
	NETVAR(m_nBotSkill, int, "CTFPlayer", "m_nBotSkill");
	//NETVAR(m_nWaterLevel, int, "CTFPlayer", "m_nWaterLevel");
	NETVAR(m_hRagdoll, int, "CTFPlayer", "m_hRagdoll");
	NETVAR(m_PlayerClass, void *, "CTFPlayer", "m_PlayerClass");
	NETVAR(m_iClass, int, "CTFPlayer", "m_iClass");
	NETVAR(m_iszClassIcon, const char *, "CTFPlayer", "m_iszClassIcon");
	NETVAR(m_iszCustomModel, const char *, "CTFPlayer", "m_iszCustomModel");
	NETVAR(m_vecCustomModelOffset, Vec3, "CTFPlayer", "m_vecCustomModelOffset");
	NETVAR(m_angCustomModelRotation, Vec3, "CTFPlayer", "m_angCustomModelRotation");
	NETVAR(m_bCustomModelRotates, bool, "CTFPlayer", "m_bCustomModelRotates");
	NETVAR(m_bCustomModelRotationSet, bool, "CTFPlayer", "m_bCustomModelRotationSet");
	NETVAR(m_bCustomModelVisibleToSelf, bool, "CTFPlayer", "m_bCustomModelVisibleToSelf");
	NETVAR(m_bUseClassAnimations, bool, "CTFPlayer", "m_bUseClassAnimations");
	NETVAR(m_iClassModelParity, int, "CTFPlayer", "m_iClassModelParity");
	NETVAR(m_Shared, void *, "CTFPlayer", "m_Shared");
	NETVAR(m_nPlayerCond, int, "CTFPlayer", "m_nPlayerCond");
	NETVAR(m_bJumping, bool, "CTFPlayer", "m_bJumping");
	NETVAR(m_nNumHealers, int, "CTFPlayer", "m_nNumHealers");
	NETVAR(m_iCritMult, int, "CTFPlayer", "m_iCritMult");
	NETVAR(m_iAirDash, int, "CTFPlayer", "m_iAirDash");
	NETVAR(m_nAirDucked, int, "CTFPlayer", "m_nAirDucked");
	NETVAR(m_flDuckTimer, float, "CTFPlayer", "m_flDuckTimer");
	NETVAR(m_nPlayerState, int, "CTFPlayer", "m_nPlayerState");
	NETVAR(m_iDesiredPlayerClass, int, "CTFPlayer", "m_iDesiredPlayerClass");
	NETVAR(m_flMovementStunTime, float, "CTFPlayer", "m_flMovementStunTime");
	NETVAR(m_iMovementStunAmount, int, "CTFPlayer", "m_iMovementStunAmount");
	NETVAR(m_iMovementStunParity, int, "CTFPlayer", "m_iMovementStunParity");
	NETVAR(m_hStunner, int, "CTFPlayer", "m_hStunner");
	NETVAR(m_iStunFlags, int, "CTFPlayer", "m_iStunFlags");
	NETVAR(m_nArenaNumChanges, int, "CTFPlayer", "m_nArenaNumChanges");
	NETVAR(m_bArenaFirstBloodBoost, bool, "CTFPlayer", "m_bArenaFirstBloodBoost");
	NETVAR(m_iWeaponKnockbackID, int, "CTFPlayer", "m_iWeaponKnockbackID");
	NETVAR(m_bLoadoutUnavailable, bool, "CTFPlayer", "m_bLoadoutUnavailable");
	NETVAR(m_iItemFindBonus, int, "CTFPlayer", "m_iItemFindBonus");
	NETVAR(m_bShieldEquipped, bool, "CTFPlayer", "m_bShieldEquipped");
	NETVAR(m_bParachuteEquipped, bool, "CTFPlayer", "m_bParachuteEquipped");
	NETVAR(m_iNextMeleeCrit, int, "CTFPlayer", "m_iNextMeleeCrit");
	NETVAR(m_iDecapitations, int, "CTFPlayer", "m_iDecapitations");
	NETVAR(m_iRevengeCrits, int, "CTFPlayer", "m_iRevengeCrits");
	NETVAR(m_iDisguiseBody, int, "CTFPlayer", "m_iDisguiseBody");
	NETVAR(m_hCarriedObject, int, "CTFPlayer", "m_hCarriedObject");
	NETVAR(m_bCarryingObject, bool, "CTFPlayer", "m_bCarryingObject");
	NETVAR(m_flNextNoiseMakerTime, float, "CTFPlayer", "m_flNextNoiseMakerTime");
	NETVAR(m_iSpawnRoomTouchCount, int, "CTFPlayer", "m_iSpawnRoomTouchCount");
	NETVAR(m_iKillCountSinceLastDeploy, int, "CTFPlayer", "m_iKillCountSinceLastDeploy");
	NETVAR(m_flFirstPrimaryAttack, float, "CTFPlayer", "m_flFirstPrimaryAttack");
	NETVAR(m_flEnergyDrinkMeter, float, "CTFPlayer", "m_flEnergyDrinkMeter");
	NETVAR(m_flHypeMeter, float, "CTFPlayer", "m_flHypeMeter");
	NETVAR(m_flChargeMeter, float, "CTFPlayer", "m_flChargeMeter");
	NETVAR(m_flInvisChangeCompleteTime, float, "CTFPlayer", "m_flInvisChangeCompleteTime");
	NETVAR(m_nDisguiseTeam, int, "CTFPlayer", "m_nDisguiseTeam");
	NETVAR(m_nDisguiseClass, int, "CTFPlayer", "m_nDisguiseClass");
	NETVAR(m_nDisguiseSkinOverride, int, "CTFPlayer", "m_nDisguiseSkinOverride");
	NETVAR(m_nMaskClass, int, "CTFPlayer", "m_nMaskClass");
	NETVAR(m_iDisguiseTargetIndex, int, "CTFPlayer", "m_iDisguiseTargetIndex");
	NETVAR(m_iDisguiseHealth, int, "CTFPlayer", "m_iDisguiseHealth");
	NETVAR(m_bFeignDeathReady, bool, "CTFPlayer", "m_bFeignDeathReady");
	NETVAR(m_hDisguiseWeapon, int, "CTFPlayer", "m_hDisguiseWeapon");
	NETVAR(m_nTeamTeleporterUsed, int, "CTFPlayer", "m_nTeamTeleporterUsed");
	NETVAR(m_flCloakMeter, float, "CTFPlayer", "m_flCloakMeter");
	NETVAR(m_flSpyTranqBuffDuration, float, "CTFPlayer", "m_flSpyTranqBuffDuration");
	NETVAR(m_nDesiredDisguiseTeam, int, "CTFPlayer", "m_nDesiredDisguiseTeam");
	NETVAR(m_nDesiredDisguiseClass, int, "CTFPlayer", "m_nDesiredDisguiseClass");
	NETVAR(m_flStealthNoAttackExpire, float, "CTFPlayer", "m_flStealthNoAttackExpire");
	NETVAR(m_flStealthNextChangeTime, float, "CTFPlayer", "m_flStealthNextChangeTime");
	NETVAR(m_bLastDisguisedAsOwnTeam, bool, "CTFPlayer", "m_bLastDisguisedAsOwnTeam");
	NETVAR(m_flRageMeter, float, "CTFPlayer", "m_flRageMeter");
	NETVAR(m_bRageDraining, bool, "CTFPlayer", "m_bRageDraining");
	NETVAR(m_flNextRageEarnTime, float, "CTFPlayer", "m_flNextRageEarnTime");
	NETVAR(m_bInUpgradeZone, bool, "CTFPlayer", "m_bInUpgradeZone");
	NETVAR(m_flItemChargeMeter, void *, "CTFPlayer", "m_flItemChargeMeter");
	NETVAR(m_bPlayerDominated, void *, "CTFPlayer", "m_bPlayerDominated");
	NETVAR(m_bPlayerDominatingMe, void *, "CTFPlayer", "m_bPlayerDominatingMe");
	NETVAR(m_ScoreData, void *, "CTFPlayer", "m_ScoreData");
	NETVAR(m_iCaptures, int, "CTFPlayer", "m_iCaptures");
	NETVAR(m_iDefenses, int, "CTFPlayer", "m_iDefenses");
	NETVAR(m_iKills, int, "CTFPlayer", "m_iKills");
	NETVAR(m_iDeaths, int, "CTFPlayer", "m_iDeaths");
	NETVAR(m_iSuicides, int, "CTFPlayer", "m_iSuicides");
	NETVAR(m_iDominations, int, "CTFPlayer", "m_iDominations");
	NETVAR(m_iRevenge, int, "CTFPlayer", "m_iRevenge");
	NETVAR(m_iBuildingsBuilt, int, "CTFPlayer", "m_iBuildingsBuilt");
	NETVAR(m_iBuildingsDestroyed, int, "CTFPlayer", "m_iBuildingsDestroyed");
	NETVAR(m_iHeadshots, int, "CTFPlayer", "m_iHeadshots");
	NETVAR(m_iBackstabs, int, "CTFPlayer", "m_iBackstabs");
	NETVAR(m_iHealPoints, int, "CTFPlayer", "m_iHealPoints");
	NETVAR(m_iInvulns, int, "CTFPlayer", "m_iInvulns");
	NETVAR(m_iTeleports, int, "CTFPlayer", "m_iTeleports");
	NETVAR(m_iResupplyPoints, int, "CTFPlayer", "m_iResupplyPoints");
	NETVAR(m_iKillAssists, int, "CTFPlayer", "m_iKillAssists");
	NETVAR(m_iPoints, int, "CTFPlayer", "m_iPoints");
	NETVAR(m_iBonusPoints, int, "CTFPlayer", "m_iBonusPoints");
	NETVAR(m_iDamageDone, int, "CTFPlayer", "m_iDamageDone");
	NETVAR(m_iCrits, int, "CTFPlayer", "m_iCrits");
	NETVAR(m_RoundScoreData, void *, "CTFPlayer", "m_RoundScoreData");
	NETVAR(m_ConditionList, void *, "CTFPlayer", "m_ConditionList");
	NETVAR(_condition_bits, int, "CTFPlayer", "_condition_bits");
	NETVAR(m_iTauntIndex, int, "CTFPlayer", "m_iTauntIndex");
	NETVAR(m_iTauntConcept, int, "CTFPlayer", "m_iTauntConcept");
	NETVAR(m_nPlayerCondEx, int, "CTFPlayer", "m_nPlayerCondEx");
	NETVAR(m_iStunIndex, int, "CTFPlayer", "m_iStunIndex");
	NETVAR(m_nHalloweenBombHeadStage, int, "CTFPlayer", "m_nHalloweenBombHeadStage");
	NETVAR(m_nPlayerCondEx2, int, "CTFPlayer", "m_nPlayerCondEx2");
	NETVAR(m_nPlayerCondEx3, int, "CTFPlayer", "m_nPlayerCondEx3");
	NETVAR(m_nStreaks, void *, "CTFPlayer", "m_nStreaks");
	NETVAR(m_unTauntSourceItemID_Low, int, "CTFPlayer", "m_unTauntSourceItemID_Low");
	NETVAR(m_unTauntSourceItemID_High, int, "CTFPlayer", "m_unTauntSourceItemID_High");
	NETVAR(m_flRuneCharge, float, "CTFPlayer", "m_flRuneCharge");
	NETVAR(m_bHasPasstimeBall, bool, "CTFPlayer", "m_bHasPasstimeBall");
	NETVAR(m_bIsTargetedForPasstimePass, bool, "CTFPlayer", "m_bIsTargetedForPasstimePass");
	NETVAR(m_hPasstimePassTarget, int, "CTFPlayer", "m_hPasstimePassTarget");
	NETVAR(m_askForBallTime, float, "CTFPlayer", "m_askForBallTime");
	NETVAR(m_bKingRuneBuffActive, bool, "CTFPlayer", "m_bKingRuneBuffActive");
	NETVAR(m_nPlayerCondEx4, int, "CTFPlayer", "m_nPlayerCondEx4");
	NETVAR(m_flHolsterAnimTime, float, "CTFPlayer", "m_flHolsterAnimTime");
	NETVAR(m_hSwitchTo, int, "CTFPlayer", "m_hSwitchTo");
	NETVAR(m_hItem, int, "CTFPlayer", "m_hItem");
	NETVAR(m_bIsCoaching, bool, "CTFPlayer", "m_bIsCoaching");
	NETVAR(m_hCoach, int, "CTFPlayer", "m_hCoach");
	NETVAR(m_hStudent, int, "CTFPlayer", "m_hStudent");
	NETVAR(m_nCurrency, int, "CTFPlayer", "m_nCurrency");
	NETVAR(m_nExperienceLevel, int, "CTFPlayer", "m_nExperienceLevel");
	NETVAR(m_nExperienceLevelProgress, int, "CTFPlayer", "m_nExperienceLevelProgress");
	NETVAR(m_bMatchSafeToLeave, bool, "CTFPlayer", "m_bMatchSafeToLeave");
	NETVAR(m_vecOrigin, Vec3, "CTFPlayer", "m_vecOrigin");
	NETVAR(m_angEyeAnglesX, float, "CTFPlayer", "m_angEyeAngles[0]");
	NETVAR(m_angEyeAnglesY, float, "CTFPlayer", "m_angEyeAngles[1]");
	NETVAR(m_angEyeAngles, Vec3, "CTFPlayer", "m_angEyeAngles[0]");
	NETVAR(m_bAllowMoveDuringTaunt, bool, "CTFPlayer", "m_bAllowMoveDuringTaunt");
	NETVAR(m_bIsReadyToHighFive, bool, "CTFPlayer", "m_bIsReadyToHighFive");
	NETVAR(m_hHighFivePartner, int, "CTFPlayer", "m_hHighFivePartner");
	NETVAR(m_nForceTauntCam, int, "CTFPlayer", "m_nForceTauntCam");
	NETVAR(m_flTauntYaw, float, "CTFPlayer", "m_flTauntYaw");
	NETVAR(m_nActiveTauntSlot, int, "CTFPlayer", "m_nActiveTauntSlot");
	NETVAR(m_iTauntItemDefIndex, int, "CTFPlayer", "m_iTauntItemDefIndex");
	NETVAR(m_flCurrentTauntMoveSpeed, float, "CTFPlayer", "m_flCurrentTauntMoveSpeed");
	NETVAR(m_flVehicleReverseTime, float, "CTFPlayer", "m_flVehicleReverseTime");
	NETVAR(m_flMvMLastDamageTime, float, "CTFPlayer", "m_flMvMLastDamageTime");
	NETVAR(m_flLastDamageTime, float, "CTFPlayer", "m_flLastDamageTime");
	NETVAR(m_bInPowerPlay, bool, "CTFPlayer", "m_bInPowerPlay");
	NETVAR(m_iSpawnCounter, int, "CTFPlayer", "m_iSpawnCounter");
	NETVAR(m_bArenaSpectator, bool, "CTFPlayer", "m_bArenaSpectator");
	NETVAR(m_AttributeManager, void *, "CTFPlayer", "m_AttributeManager");
	NETVAR(m_hOuter, int, "CTFPlayer", "m_hOuter");
	NETVAR(m_ProviderType, int, "CTFPlayer", "m_ProviderType");
	NETVAR(m_iReapplyProvisionParity, int, "CTFPlayer", "m_iReapplyProvisionParity");
	NETVAR(m_flHeadScale, float, "CTFPlayer", "m_flHeadScale");
	NETVAR(m_flTorsoScale, float, "CTFPlayer", "m_flTorsoScale");
	NETVAR(m_flHandScale, float, "CTFPlayer", "m_flHandScale");
	NETVAR(m_bUseBossHealthBar, bool, "CTFPlayer", "m_bUseBossHealthBar");
	NETVAR(m_bUsingVRHeadset, bool, "CTFPlayer", "m_bUsingVRHeadset");
	NETVAR(m_bForcedSkin, bool, "CTFPlayer", "m_bForcedSkin");
	NETVAR(m_nForcedSkin, int, "CTFPlayer", "m_nForcedSkin");
	NETVAR(m_bGlowEnabled, bool, "CTFPlayer", "m_bGlowEnabled");
	NETVAR(m_nActiveWpnClip, int, "CTFPlayer", "m_nActiveWpnClip");
	NETVAR(m_flKartNextAvailableBoost, float, "CTFPlayer", "m_flKartNextAvailableBoost");
	NETVAR(m_iKartHealth, int, "CTFPlayer", "m_iKartHealth");
	NETVAR(m_iKartState, int, "CTFPlayer", "m_iKartState");
	NETVAR(m_hGrapplingHookTarget, int, "CTFPlayer", "m_hGrapplingHookTarget");
	NETVAR(m_hSecondaryLastWeapon, int, "CTFPlayer", "m_hSecondaryLastWeapon");
	NETVAR(m_bUsingActionSlot, bool, "CTFPlayer", "m_bUsingActionSlot");
	NETVAR(m_flInspectTime, float, "CTFPlayer", "m_flInspectTime");
	NETVAR(m_flHelpmeButtonPressTime, float, "CTFPlayer", "m_flHelpmeButtonPressTime");
	NETVAR(m_iCampaignMedals, int, "CTFPlayer", "m_iCampaignMedals");
	NETVAR(m_iPlayerSkinOverride, int, "CTFPlayer", "m_iPlayerSkinOverride");
	NETVAR(m_bViewingCYOAPDA, bool, "CTFPlayer", "m_bViewingCYOAPDA");

	inline float m_flInvisibility( )
	{
		static DWORD offset = g_NetVars.get_offset( "DT_TFPlayer", "m_Shared", "m_flInvisChangeCompleteTime" ) - 0x8;
		return *reinterpret_cast< float * >( reinterpret_cast< DWORD >( this ) + offset );
	}

	inline void SetInvisibility( float flInvis )
	{
		static DWORD offset = g_NetVars.get_offset( "DT_TFPlayer", "m_Shared", "m_flInvisChangeCompleteTime" ) - 0x8;
		*reinterpret_cast< float * >( reinterpret_cast< DWORD >( this ) + offset ) = flInvis;
	}

	enum thinkmethods_t {
		THINK_FIRE_ALL_FUNCTIONS,
		THINK_FIRE_BASE_ONLY,
		THINK_FIRE_ALL_BUT_BASE
	};

	inline bool IsSwimming() {
		return (m_nWaterLevel() > 1);
	}

	inline bool OnSolid() {
		return m_hGroundEntity() >= 0 || IsOnGround();
	}

	inline Vec3 GetEyePosition() {
		return (m_vecViewOffset() + GetAbsOrigin());
	}

	inline float CalculateMaxSpeed(bool bIgnoreSpecialAbility = false)
	{
		typedef float(__thiscall* CalculateMaxSpeedFn)(CBaseEntity*, bool);
		static DWORD dwFn = g_Pattern.Find(L"client.dll", L"E8 ? ? ? ? D9 96 ? ? ? ? D9 EE DB F1") + 0x1;
		static DWORD dwEstimate = ((*(PDWORD)(dwFn)) + dwFn + 0x4);
		CalculateMaxSpeedFn maxSpeed = (CalculateMaxSpeedFn)dwEstimate;
		return maxSpeed(this, bIgnoreSpecialAbility);
	}

	inline void SetAbsOrigin( const Vec3 &vOrigin ) {
		typedef void( __thiscall *FN )( CBaseEntity *, const Vec3 & );
		static DWORD dwFN = g_Pattern.Find( ( L"client.dll" ), ( L"55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07" ) );
		FN func = ( FN )dwFN;
		func( this, vOrigin );
	}

	inline Vec3 GetHitboxPosMatrix( const int nHitbox, matrix3x4_t BoneMatrix[ 128 ] ) {
		if ( const auto &pModel = GetModel( ) ) {
			if ( const auto &pHdr = I::ModelInfo->GetStudioModel( pModel ) ) {
				if ( const auto &pSet = pHdr->GetHitboxSet( m_nHitboxSet( ) ) ) {
					if ( const auto &pBox = pSet->pHitbox( nHitbox ) ) {
						Vec3 vPos = ( pBox->bbmin + pBox->bbmax ) * 0.5f, vOut;
						Utils::VectorTransform( vPos, BoneMatrix[ pBox->bone ], vOut );
						return vOut;
					}
				}
			}
		}

		return Vec3( );
	}

	/*inline CTFWeaponBase* GetWeaponFromSlot(const int nSlot) {
		static DWORD dwMyWeapons = g_NetVars.get_offset("CBaseCombatCharacter", "m_hMyWeapons");
		int hWeapon = *reinterpret_cast<int*>(this + (dwMyWeapons + (nSlot * 0x4)));
		return reinterpret_cast<CTFWeaponBase*>(I::ClientEntityList->GetClientEntityFromHandle(hWeapon));
	}*/

	inline int SaveData( const char *const context, const int slot, const int type )
	{
		using fn = int( __thiscall * )( void *, const char *, int, int );
		static fn FN = reinterpret_cast< fn >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 7C 53 57 8B 7D 0C 8B D9 83 FF FF 75 0B" ) );
		return FN( this, context, slot, type );
	}

	inline void RemoveCond( int cond )
	{
		using fn = void( __thiscall * )( void *, int, int );

		static fn FN = reinterpret_cast< fn >( g_Pattern.Find( L"client.dll", L"55 8B EC 53 8B 5D 08 57 53 8B F9 E8 ? ? ? ? 84 C0 0F 84 ? ? ? ?" ) );
		FN( this, cond, 0 );
	}

	inline bool PhysicsRunThink( const thinkmethods_t thinkMethod = THINK_FIRE_ALL_FUNCTIONS )
	{
		using fn = bool( __thiscall * )( void *, thinkmethods_t );
		static fn FN = reinterpret_cast< fn >( g_Pattern.Find( L"client.dll", L"55 8B EC 53 8B D9 56 57 8B 83 ? ? ? ? C1 E8 16 A8 01 B0 01 0F 85 ? ? ? ?" ) );
		return FN( this, thinkMethod );
	}

	inline bool IsInValidTeam() {
		const int nTeam = m_iTeamNum();
		return (nTeam == 2 || nTeam == 3);
	}

	inline CTFWeaponBase *GetWeaponFromSlot( const int nSlot ) {
		static DWORD dwMyWeapons = g_NetVars.get_offset( "DT_BaseCombatCharacter", "m_hMyWeapons" );
		int hWeapon = *reinterpret_cast< int * >( reinterpret_cast<DWORD>(this) + ( dwMyWeapons + ( nSlot * 0x4 ) ) );
		return reinterpret_cast< CTFWeaponBase * >( I::ClientEntityList->GetClientEntityFromHandle( hWeapon ) );
	}

	//credits to KGB
	inline bool InCond(const ETFCond cond)
	{
		const int iCond = static_cast<int>(cond);
		switch (iCond / 32)
		{
			case 0:
			{
				const int bit = (1 << iCond);
				if ((m_nPlayerCond() & bit) == bit)
				{
					return true;
				}

				if ((_condition_bits() & bit) == bit)
				{
					return true;
				}

				break;
			}
			case 1:
			{
				const int bit = 1 << (iCond - 32);
				if ((m_nPlayerCondEx() & bit) == bit)
				{
					return true;
				}

				break;
			}
			case 2:
			{
				const int bit = 1 << (iCond - 64);
				if ((m_nPlayerCondEx2() & bit) == bit)
				{
					return true;
				}

				break;
			}
			case 3:
			{
				const int bit = 1 << (iCond - 96);
				if ((m_nPlayerCondEx3() & bit) == bit)
				{
					return true;
				}

				break;
			}
			case 4:
			{
				const int bit = 1 << (iCond - 128);
				if ((m_nPlayerCondEx4() & bit) == bit)
				{
					return true;
				}

				break;
			}
			default:
				break;
		}

		return false;
	}

	inline bool IsCritBoosted()
	{
		return InCond(TF_COND_CRITBOOSTED) ||
			InCond(TF_COND_OFFENSEBUFF) ||
			InCond(TF_COND_ENERGY_BUFF) ||
			InCond(TF_COND_CRITBOOSTED_BONUS_TIME) ||
			InCond(TF_COND_CRITBOOSTED_CARD_EFFECT) ||
			InCond(TF_COND_CRITBOOSTED_CTF_CAPTURE) ||
			InCond(TF_COND_CRITBOOSTED_DEMO_CHARGE) ||
			InCond(TF_COND_CRITBOOSTED_FIRST_BLOOD) ||
			InCond(TF_COND_CRITBOOSTED_ON_KILL) ||
			InCond(TF_COND_CRITBOOSTED_PUMPKIN) ||
			InCond(TF_COND_CRITBOOSTED_RAGE_BUFF) ||
			InCond(TF_COND_CRITBOOSTED_RUNE_TEMP) ||
			InCond(TF_COND_CRITBOOSTED_USER_BUFF);
	}

	inline bool IsCritBoostedNoMini()
	{
		return InCond(TF_COND_CRITBOOSTED) ||
			InCond(TF_COND_CRITBOOSTED_BONUS_TIME) ||
			InCond(TF_COND_CRITBOOSTED_CARD_EFFECT) ||
			InCond(TF_COND_CRITBOOSTED_CTF_CAPTURE) ||
			InCond(TF_COND_CRITBOOSTED_DEMO_CHARGE) ||
			InCond(TF_COND_CRITBOOSTED_FIRST_BLOOD) ||
			InCond(TF_COND_CRITBOOSTED_ON_KILL) ||
			InCond(TF_COND_CRITBOOSTED_PUMPKIN) ||
			InCond(TF_COND_CRITBOOSTED_RAGE_BUFF) ||
			InCond(TF_COND_CRITBOOSTED_RUNE_TEMP) ||
			InCond(TF_COND_CRITBOOSTED_USER_BUFF);
	}

	inline int GetMaxHealth() {
		using fn = int(__thiscall *)(void *);
		fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(this, 107));
		return FN(this);
	}

	inline bool GetHitboxMinsAndMaxsAndMatrix(const int nHitbox, Vec3& vMins, Vec3& vMaxs, matrix3x4_t& matrix, Vec3* vCenter) {
		if (const auto& pModel = GetModel()) {
			if (const auto& pHdr = I::ModelInfo->GetStudioModel(pModel)) {
				matrix3x4_t BoneMatrix[128];
				if (SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime)) {
					if (const auto& pSet = pHdr->GetHitboxSet(m_nHitboxSet())) {
						if (const auto& pBox = pSet->pHitbox(nHitbox)) {
							vMins = pBox->bbmin; vMaxs = pBox->bbmax;
							memcpy(matrix, BoneMatrix[pBox->bone], sizeof(matrix3x4_t));
							if (vCenter) Utils::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *vCenter);
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	inline bool IsAlive() {
		return (m_lifeState() == LIFE_ALIVE);
	}

	inline bool PlayerOnFriendsList( )
	{
		using fn = bool( __stdcall * )( CTFPlayer * );
		static auto Pat = g_Pattern.Find( L"client.dll", L"55 8B EC 81 EC ? ? ? ? E8 ? ? ? ? 85 C0 74 7E 8B 4D 08 85 C9 74 77 A1 ? ? ? ? 83 78 08 00 74 6C 83 78 0C 00 74 66 A1 ? ? ? ? 83 C1 08 56 8B 30 8D 85 ? ? ? ? 50 8B 01" );
		return reinterpret_cast< fn >( Pat )( this );
	}

	inline Vec3 GetShootPos()
	{
		using fn = float *(__thiscall *)(void *, Vec3 *);
		static fn FN = reinterpret_cast<fn>(g_Pattern.Find(L"client.dll", L"55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90"));
		Vec3 vOut = Vec3();
		FN(this, &vOut);
		return vOut;
	}

	inline int GetNumOfHitboxes()
	{
		if (auto pModel = GetModel())
		{
			if (auto pHDR = I::ModelInfo->GetStudioModel(pModel))
			{
				if (auto pSet = pHDR->GetHitboxSet(m_nHitboxSet()))
					return pSet->numhitboxes;
			}
		}

		return 0;
	}

	inline Vec3 GetHitboxPos(int nHitbox)
	{
		if (auto pModel = GetModel())
		{
			if (auto pHDR = I::ModelInfo->GetStudioModel(pModel))
			{
				matrix3x4_t BoneMatrix[128];

				if (SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime))
				{
					if (auto pSet = pHDR->GetHitboxSet(m_nHitboxSet()))
					{
						if (auto pBox = pSet->pHitbox(nHitbox))
						{
							Vec3 vPos = (pBox->bbmin + pBox->bbmax) * 0.5f, vOut;
							Utils::VectorTransform(vPos, BoneMatrix[pBox->bone], vOut);
							return vOut;
						}
					}
				}
			}
		}
		return {};
	}

	inline CTFPlayerAnimState *GetAnimState( ) {
		return *reinterpret_cast< CTFPlayerAnimState ** >( reinterpret_cast<DWORD>( this ) + 0x1D00 );
	}

	inline void SetPoseParam( std::array<float, MAXSTUDIOPOSEPARAM> param ) {
		static DWORD dwOffset = g_NetVars.get_offset( "DT_BaseAnimating","m_flPoseParameter" );
		*reinterpret_cast< std::array<float, MAXSTUDIOPOSEPARAM> * >( reinterpret_cast< DWORD >( this ) + dwOffset ) = param;
	}

	inline std::array<float, MAXSTUDIOPOSEPARAM> GetPoseParam( ) {
		static DWORD dwOffset = g_NetVars.get_offset( "DT_BaseAnimating", "m_flPoseParameter" );
		return *reinterpret_cast< std::array<float, MAXSTUDIOPOSEPARAM>* >( reinterpret_cast< DWORD >( this ) + dwOffset );
	}
};

enum ETFPlayerState
{
	TF_STATE_ACTIVE = 0,		// Happily running around in the game.
	TF_STATE_WELCOME,			// First entering the server (shows level intro screen).
	TF_STATE_OBSERVER,			// Game observer mode.
	TF_STATE_DYING,				// Player is dying.
	TF_STATE_COUNT
};

enum EObserverModes
{
	OBS_MODE_NONE = 0,		// not in spectator mode
	OBS_MODE_DEATHCAM,		// special mode for death cam animation
	OBS_MODE_FREEZECAM,		// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,			// view from a fixed camera position
	OBS_MODE_FIRSTPERSON,	// follow a player in first person view
	OBS_MODE_THIRDPERSON,	// follow a player in third person view
	OBS_MODE_POI,			// PASSTIME point of interest - game objective, big fight, anything interesting; added in the middle of the enum due to tons of hard-coded "<ROAMING" enum compares
	OBS_MODE_ROAMING,		// free roaming
};