#pragma once
#include "../../../SDK/SDK.h"

struct PlayerData
{
	//Data
	float flJoinTime = 0.f;
	Vec2 vLastAngle = { 0, 0 };
	int iPlayerSuspicion = 0;
	int iNonDormantCleanQueries = 0;
	std::pair<int, int> pBhop = { 0, 0 };
	std::pair<bool, Vec2> pTrustAngles = { false, {0, 0} };
	std::pair<bool, bool> pDetections = { false, false };	//	high hitchance, high avg score

	//Immediate Data
	bool bDidDamage = false;
	bool bSuspicious = false;

	//Tickbase Manipulation
	std::pair<int, int> pChoke = { 0, 0 };

	//Analytics
	std::pair<int, float> pDuckInfo = { 0, 0 };
	std::pair<int, int> pShots = { 0, 0 };
	float flHitchance = 0.f;
	float flScorePerSecond = 0.f;
	int iLastDamageEventTick = 0;	// lets not recalculate hitchance for a player multiple times (explosive dmg, weird shell weapon behaviour)
};

struct ServerData
{
	float flAverageScorePerSecond = 0.f;					//	used to determine if our player is in the top 99% or just average within the server
	int iMisses = 0;										//	total shots fired
	int iHits = 0;											//	total shots hit
	float flHighAccuracy;									//	highest acceptable accuracy
	int iTickRate = 0;										//	used for data
	float flMultiplier = 1.f;								//	66.7 / iTickRate, real players should have shorter flicks on servers where the tickrate is higher, and larger flicks on lower tickrate servers
	float flFloorScore = 0.05f;								//	assume this as the minimum average score, what if a majority of the server is friendly, instead of a minimum score to be included, just set this as the average
};

// TODO: make these vars
constexpr int INT_MAX_SUSPICION = 25;
constexpr int INT_MAX_BHOP = 5;

class CCheaterDetection
{
	// data
	int iLastScanTick = 0;
	float flLastFrameTime = 0.f;
	float flFirstScanTime = 0.f;
	float flScanningTime = 0.f;
	float flLastScoreTime = 0.f;
	float flLastAccuracyTime = 0.f;
	ServerData server;
public:
	std::unordered_map<CTFPlayer *, PlayerData> mData;
private:
	std::unordered_map<CTFPlayer *, bool> mFired;

	// is it safe for the client to be doing this, is the process drunk & unable to make proper judgement, has it been bribed???/?
	bool ShouldScan( );

	// is the player fit for scanning on this tick
	bool ShouldScanEntity( CTFPlayer *pEntity );

	// player checks
	bool IsPitchLegal( CTFPlayer *pEntity );
	bool CheckBHop( CTFPlayer *pEntity );
	bool AreAnglesSuspicious( CTFPlayer *pEntity );
	void SimTime( CTFPlayer *pEntity );
	void AimbotCheck( CTFPlayer *pEntity );
	bool IsDuckSpeed( CTFPlayer *pEntity );

	// player utils
	void CalculateHitChance( CTFPlayer *pEntity );
	void OnDormancy( CTFPlayer *pEntity );

	// server functions
	void FindScores( );
	void FindHitchances( );
	void FillServerInfo( );

	//
	void Reset( );
public:
	void OnTick( );
	void ReportTickCount( std::pair<CTFPlayer *, int> pReport );
	void ReportShot( int iIndex );
	void ReportDamage( IGameEvent *pEvent );
	void OnLoad( );
};

inline CCheaterDetection g_BadActors;