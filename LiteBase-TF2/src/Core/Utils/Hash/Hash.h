#pragma once

//Main hash function
//And prehashed strings allowing for fast comparisong.
namespace Hash
{
	void PrintHash();

	//TODO:
	//MISSING NEW SPELLBOOK, AND SPELL CHRYSTAL

	//Panels
	constexpr unsigned int FocusOverlayPanel = 2347069313;
	constexpr unsigned int HudScope = 4197140386;
	constexpr unsigned int HudMOTD = 263456517; // MOTD panel

	//Health packs
	constexpr unsigned int MedKitBanana = 48961140;
	constexpr unsigned int MedKitSmall = 1138108014;
	constexpr unsigned int MedKitMedium = 3591694894;
	constexpr unsigned int MedKitLarge = 2085245990;
	constexpr unsigned int MedKitSmallBday = 3780270629;
	constexpr unsigned int MedKitMediumBday = 2229940197;
	constexpr unsigned int MedKitLargeBday = 2131865389;
	constexpr unsigned int MedKitSandwich = 430753038;
	constexpr unsigned int MedKitSandwichXmas = 3376545636;
	constexpr unsigned int MedKitSandwichRobo = 1299884225;
	constexpr unsigned int MedKitSteak = 2817177407;
	constexpr unsigned int MedKitDalokohs = 4000983528;
	constexpr unsigned int MedKitFishCake = 3413381804;
	constexpr unsigned int MedKitSmallHalloween = 2634597576;
	constexpr unsigned int MedKitMediumHalloween = 4166466968;
	constexpr unsigned int MedKitLargeHalloween = 3893084692;
	constexpr unsigned int MedKitLargeMushroom = 980493366;
	constexpr unsigned int MedievalMeat = 969509793;

	//Ammo packs
	constexpr unsigned int AmmoSmall = 2534489767;
	constexpr unsigned int AmmoMedium = 455139957;
	constexpr unsigned int AmmoLarge = 1018735539;
	constexpr unsigned int AmmoLargeBday = 1501082414;
	constexpr unsigned int AmmoMediumBday = 1283759932;
	constexpr unsigned int AmmoSmallBday = 2832603626;

	//Models
	constexpr unsigned int SentryBuster = 2509256133;
	constexpr unsigned int AtomBomb = 3090011807;
	constexpr unsigned int BombCart = 991469546;
	constexpr unsigned int ResupplyLocker = 3947990550;
	constexpr unsigned int ControlPoint = 2794151446;
	constexpr unsigned int ControlPointHolo = 683521128;

	bool IsControlPoint(const char* szName);
	bool IsControlPointHolo(const char* szName);
	bool IsAtomBomb(const char* szName);
	bool IsBombCart(const char* szName);
	bool IsAmmo(const char* szName);
	bool IsHealth(const char* szName);
	//bool IsSpell(const char* szName);
	//bool IsResupply(const char* szName);
	bool IsHudScope(const char* szName);
}