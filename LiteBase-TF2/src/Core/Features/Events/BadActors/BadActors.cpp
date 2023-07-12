#include "BadActors.h"
#include "../Events.h"

bool CCheaterDetection::ShouldScan( )
{
	if ( I::EngineClient->IsPlayingTimeDemo( ) ) { return false; }

	if ( iLastScanTick == I::GlobalVars->tickcount ) { return false; }

	if ( flLastFrameTime )
	{
		const float flRealFrameTime = I::GlobalVars->realtime - flLastFrameTime;
		flLastFrameTime = I::GlobalVars->realtime;
		const int iRealFPS = static_cast< int >( 1.0f / flRealFrameTime );

		if ( iRealFPS < server.iTickRate )
		{
			return false;
		}
	}

	if ( const INetChannelInfo *NetChannel = I::EngineClient->GetNetChannelInfo( ) )
	{
		const float flLastReceive = NetChannel->GetTimeSinceLastReceived( );
		const float flMaxReceive = I::GlobalVars->interval_per_tick * 2;
		const bool bIsTimeOut = NetChannel->IsTimingOut( );
		const bool bShouldSkip = ( flLastReceive > flMaxReceive ) || bIsTimeOut;
		if ( bShouldSkip ) { return false; }
	}

	return true;
}

bool CCheaterDetection::ShouldScanEntity( CTFPlayer *pEntity )
{
	const int iIndex = pEntity->entindex( );

	if ( fabsf( I::GlobalVars->curtime - mData[ pEntity ].flJoinTime ) < 10.f ) { return false; }	//	if they haven't been in the server for 10 seconds dont scan them

	// dont scan invalid players
	if ( pEntity->deadflag( ) || pEntity->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) || pEntity->InCond( TF_COND_TAUNTING ) ) { return false; }

	// dont scan players that arent simulated this tick
	if ( pEntity->m_flSimulationTime( ) == pEntity->GetOldSimulationTime( ) ) { return false; }

	// dont scan if we can't get playerinfo
	player_info_t pInfo{};
	if ( !I::EngineClient->GetPlayerInfo( iIndex, &pInfo ) ) { return false; }

	// dont scan ignored players, friends, or players already marked as cheaters
	switch ( g_GlobalInfo.PlayerPriority[ pInfo.friendsID ].Mode )
	{
		case 0: case 1: case 4:
		{ return false; }
	}

	if ( !mData[ pEntity ].flJoinTime ) { mData[ pEntity ].flJoinTime = I::GlobalVars->curtime; return false; }

	return true;
}

bool CCheaterDetection::IsPitchLegal( CTFPlayer *pSuspect )
{
	const Vec3 vAngles = pSuspect->m_angEyeAngles( );
	return fabsf( vAngles.x ) < 90.f;
}

void CCheaterDetection::ReportTickCount( std::pair<CTFPlayer *, int> pReport )
{
	if ( mData[ pReport.first ].pChoke.first = 0 ) { mData[ pReport.first ].pChoke.second = pReport.second; return; }
	mData[ pReport.first ].pChoke.first++;
	mData[ pReport.first ].pChoke.second += pReport.second;
}

bool CCheaterDetection::CheckBHop( CTFPlayer *pEntity )
{
	const bool bOnGround = pEntity->OnSolid( );	//	NOTE: groundentity isn't networked properly sometimes i think
	if ( bOnGround ) { mData[ pEntity ].pBhop.first++; }
	else if ( mData[ pEntity ].pBhop.first <= 1 && mData[ pEntity ].pBhop.first > 0 ) { mData[ pEntity ].pBhop.second++; mData[ pEntity ].pBhop.first = 0; }
	else { mData[ pEntity ].pBhop = { 0, 0 }; }

	if ( mData[ pEntity ].pBhop.second >= 5 ) { mData[ pEntity ].iPlayerSuspicion++; mData[ pEntity ].pBhop = { 0, 0 }; return true; }
	return false;
}

bool CCheaterDetection::AreAnglesSuspicious( CTFPlayer *pEntity )
{
	// first check if we should scan this player at all
	if ( g_GlobalInfo.ChokeMap[ pEntity->entindex( ) ] > 0 || pEntity->GetVelocity( ).Length( ) < 10.f || mData[ pEntity ].vLastAngle.IsZero( ) ) { mData[ pEntity ].pTrustAngles = { false, {0, 0} }; return false; } //	angles don't update the way I WANT them to if the player is not moving.
	if ( !mData[ pEntity ].pTrustAngles.first )
	{	//	we are not suspicious of this player yet
		const Vec3 vCurAngles = pEntity->m_angEyeAngles( );
		const Vec2 vDelta = Vec2{ vCurAngles.x, vCurAngles.y } - mData[ pEntity ].vLastAngle;
		const float flDelta = vDelta.Length( );

		if ( flDelta > ( 20 ) ) { mData[ pEntity ].pTrustAngles = { true, {vCurAngles.x, vCurAngles.y} }; }
	}
	else
	{
		//	check for noise on this player (how much their mouse moves after the initial flick)
		const Vec3 vCurAngles = pEntity->m_angEyeAngles( );
		const Vec2 vDelta = Vec2{ vCurAngles.x, vCurAngles.y } - mData[ pEntity ].pTrustAngles.second;
		const float flDelta = vDelta.Length( );

		if ( flDelta < ( 5.f * server.flMultiplier ) ) { mData[ pEntity ].pTrustAngles = { false, {0, 0} }; return true; }
		else { mData[ pEntity ].pTrustAngles = { false, {0, 0} }; }
	}
	return false;
}

void CCheaterDetection::AimbotCheck( CTFPlayer *pEntity )
{
	const Vec2 vCurAngle = { pEntity->m_angEyeAngles( ).x, pEntity->m_angEyeAngles( ).y };
	const Vec2 vCurAngle2 = { vCurAngle.x, vCurAngle.y };
	const auto &pLocal = g_EntityCache.GetLocal( );
	const float flDelta = vCurAngle.DistTo( mData[ pEntity ].vLastAngle );	//	aimbot flick (unscaled)
	const float flScaled = std::clamp( flDelta * g_GlobalInfo.ChokeMap[ pEntity->entindex( ) ], 0.f, 20.f );	//	aimbot flick scaled

	if ( flScaled > 3.f )
	{	//	this person is aimbotting in our eyes.
		player_info_t pInfo{};
		if ( !I::EngineClient->GetPlayerInfo( pEntity->entindex( ), &pInfo ) ) { return; }

		mData[ pEntity ].iPlayerSuspicion++;
		////Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for aimbot.", pInfo.name ).c_str( ), { 224, 255, 131, 255 } );
		std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
		std::string szName = yellow + pInfo.m_sName + white;
		std::string szAngle = yellow + ( "%.1f | %.1f" ) + white;
		std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for aimbot. [" + szAngle + "]";
		I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ), flScaled, flDelta );
	}
}

bool CCheaterDetection::IsDuckSpeed( CTFPlayer *pEntity )
{
	const bool bIsDucking = pEntity->IsDucking( ) && pEntity->OnSolid( );	//	this may break on movement sim?? (ty spook >:C)
	if ( !bIsDucking ) { mData[ pEntity ].pDuckInfo = { 0, 0 }; return false; }
	if ( mData[ pEntity ].pDuckInfo.first == -1 ) { return false; }	//	dont double infract

	if ( TICKS_TO_TIME( mData[ pEntity ].pDuckInfo.first ) > 1.f )
	{	//	only check once we have a full second of data.
		const float flAvg = mData[ pEntity ].pDuckInfo.second / mData[ pEntity ].pDuckInfo.first;
		if ( flAvg > pEntity->m_flMaxspeed( ) * .5 )
		{	//	this is meant to be exactly 1/3rd but since we're averaging we should be nicer...
			mData[ pEntity ].pDuckInfo.first = -1;
			return true;
		}
	}
	else
	{
		mData[ pEntity ].pDuckInfo.first++;
		mData[ pEntity ].pDuckInfo.second += pEntity->GetVelocity( ).Length2D( );
	}
	return false;
}

void CCheaterDetection::SimTime( CTFPlayer *pEntity )
{
	if ( mData[ pEntity ].iNonDormantCleanQueries < 6 ) { return; }
	const float flSimDelta = pEntity->m_flSimulationTime( ) - pEntity->GetOldSimulationTime( );
	const int iTickDelta = TIME_TO_TICKS( flSimDelta );
	if ( mData[ pEntity ].pChoke.first = 0 ) { mData[ pEntity ].pChoke.second = iTickDelta; return; }
	mData[ pEntity ].pChoke.first++;
	mData[ pEntity ].pChoke.second += iTickDelta;
	return;
}

void CCheaterDetection::OnDormancy( CTFPlayer *pEntity )
{
	mData[ pEntity ].pTrustAngles = { false, {0, 0} };
	mData[ pEntity ].iNonDormantCleanQueries = 0;
	mData[ pEntity ].vLastAngle = {};
	mData[ pEntity ].pBhop = { false, 0 };
	mFired[ pEntity ] = false;
}

void CCheaterDetection::OnTick( )
{
	const auto pLocal = g_EntityCache.GetLocal( );
	if ( !pLocal || !I::EngineClient->IsConnected( ) || g_GlobalInfo.m_bShouldShift || g_GlobalInfo.m_bRecharging || !ShouldScan( ) ) { return; }
	iLastScanTick = I::GlobalVars->tickcount;
	flScanningTime = I::GlobalVars->curtime - flFirstScanTime;
	FindScores( );
	FindHitchances( );

	for ( const auto& pPlayer : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
	{
		//CTFPlayer *pEntity = I::ClientEntityList->GetClientEntity( n )->As<CTFPlayer>( );
		const auto &pEntity = pPlayer->As<CTFPlayer>( );
		if ( !pEntity ) { continue; }
		if ( pEntity->IsDormant( ) ) { OnDormancy( pEntity ); continue; }
		if ( !ShouldScanEntity( pEntity ) ) { OnDormancy( pEntity ); continue; }
		if ( pEntity == pLocal ) { continue; }	//	i think for this code to run the local player has to be cheating anyway :thinking:

		player_info_t pInfo{};
		if ( !I::EngineClient->GetPlayerInfo( pEntity->entindex( ), &pInfo ) ) { continue; }
		if ( pInfo.fakeplayer ) { continue; }

		CalculateHitChance( pEntity );
		SimTime( pEntity );

		if ( !IsPitchLegal( pEntity ) ) 
		{ 
			mData[ pEntity ].iPlayerSuspicion = 10; 
			/*Utils::ConLog( "CheaterDetection", tfm::format( "%s marked for OOB angles.", pInfo.name ).c_str( ), { 224, 255, 131, 255 } );*/
			std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
			std::string szName = yellow + pInfo.m_sName + white;
			std::string szAngle = yellow + ( "%.1f | %.1f" ) + white;
			std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for OOB angles.";
			I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ) );
		}

		/*if ( AreAnglesSuspicious( pEntity ) )
		{
			mData[ pEntity ].iPlayerSuspicion++;
			/*Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for suspicious angles.", pInfo.name ).c_str( ), { 224, 255, 131, 255 } );
			std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
			std::string szName = yellow + pInfo.m_sName + white;
			std::string szAngle = yellow + ( "%.1f | %.1f" ) + white;
			std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for suspicious angles.";
			I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ) );
		}*/

		if ( CheckBHop( pEntity ) ) 
		{ 
			/*Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for bunny-hopping.", pInfo.name ).c_str( ), { 224, 255, 131, 255 } );*/
			std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
			std::string szName = yellow + pInfo.m_sName + white;
			std::string szAngle = yellow + ( "%.1f | %.1f" ) + white;
			std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for BHop.";
			I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ) );
		}

		/*if ( IsDuckSpeed( pEntity ) )
		{ 
			/*Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for suspected duck speed.", pInfo.name ).c_str( ), { 224, 255, 131, 255 } );
			mData[ pEntity ].iPlayerSuspicion++;
			std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
			std::string szName = yellow + pInfo.m_sName + white;
			std::string szAngle = yellow + ( "%.1f | %.1f" ) + white;
			std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for suspected duck speed.";
			I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ) );
		}*/

		//analytical analysis
		if ( mData[ pEntity ].pChoke.second && mData[ pEntity ].pChoke.first )
		{
			if ( ( ( float )mData[ pEntity ].pChoke.second / ( float )mData[ pEntity ].pChoke.first ) > ( 14 / server.flMultiplier ) && mData[ pEntity ].pChoke.first > 10 )
			{
				mData[ pEntity ].iPlayerSuspicion++;
				/*Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for high avg packet choking {%.1f / %.1f}.", pInfo.name, mData[ pEntity ].pChoke.second, ( 14.f / server.flMultiplier ) ).c_str( ), { 224, 255, 131, 255 } );*/
				std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
				std::string szName = yellow + pInfo.m_sName + white;
				std::string szChoke = yellow + ( "%.1f | %.1f" ) + white;
				std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for choking packets. [" + szChoke + "]";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ), mData[ pEntity ].pChoke.second, ( 14.f / server.flMultiplier ) );
				mData[ pEntity ].pChoke = { 0, 0.f }; 
			}
		}
		if ( flScanningTime > 60.f )
		{
			if ( mData[ pEntity ].flHitchance && server.flHighAccuracy && mData[ pEntity ].pShots.first > 25 )
			{
				if ( mData[ pEntity ].flHitchance > ( server.flHighAccuracy ) && !mData[ pEntity ].pDetections.first )
				{
					mData[ pEntity ].iPlayerSuspicion += 5; 
					/*Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for extremely high accuracy {%.1f / %.1f}.", pInfo.name, mData[ pEntity ].flHitchance, server.flHighAccuracy ).c_str( ), { 224, 255, 131, 255 } ); */
					std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
					std::string szName = yellow + pInfo.m_sName + white;
					std::string szAccuracy = yellow + ( "%.1f | %.1f" ) + white;
					std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for high accuracy. [" + szAccuracy + "]";
					I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ), mData[ pEntity ].flHitchance, server.flHighAccuracy );
				}
			}
			if ( mData[ pEntity ].flScorePerSecond && server.flAverageScorePerSecond )
			{
				if ( mData[ pEntity ].flScorePerSecond > ( std::max( server.flAverageScorePerSecond, server.flFloorScore ) * 2.f ) && !mData[ pEntity ].pDetections.second )
				{
					mData[ pEntity ].iPlayerSuspicion += 5; 
					/*Utils::ConLog( "CheaterDetection", tfm::format( "%s infracted for extremely high score per second {%.1f / %.1f}.", pInfo.name, mData[ pEntity ].flScorePerSecond, server.flAverageScorePerSecond ).c_str( ), { 224, 255, 131, 255 } ); */
					std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
					std::string szName = yellow + pInfo.m_sName + white;
					std::string szScore = yellow + ( "%.1f | %.1f" ) + white;
					std::string aimbot = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked for high score. [" + szScore + "]";
					I::ClientModeShared->m_pChatElement->ChatPrintf( 0, aimbot.c_str( ), mData[ pEntity ].flScorePerSecond, server.flAverageScorePerSecond );
				}
			}
		}

		const Vec3 vAngles = pEntity->m_angEyeAngles( );
		mData[ pEntity ].vLastAngle = { vAngles.x, vAngles.y };

		if ( mData[ pEntity ].iPlayerSuspicion >= 5 && mData[ pEntity ].iPlayerSuspicion < 10 )
		{
			if ( !mData[ pEntity ].bSuspicious )
			{
				std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
				std::string szName = yellow + pInfo.m_sName + white;
				std::string szSuspicious = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked as suspicious.";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, szSuspicious.c_str( ) );
			}
			mData[ pEntity ].bSuspicious = true;
		}

		else if ( mData[ pEntity ].iPlayerSuspicion >= 10 ) 
		{
			mData[ pEntity ].bSuspicious = false;
			mData[ pEntity ].iPlayerSuspicion = 0; 
			g_GlobalInfo.PlayerPriority[ pInfo.friendsID ].Mode = 4;

			std::string szTeam = pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
			std::string szName = yellow + pInfo.m_sName + white;
			std::string szCheater = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " was marked as a cheater.";
			I::ClientModeShared->m_pChatElement->ChatPrintf( 0, szCheater.c_str( ) );
		}
	}
}

void CCheaterDetection::FillServerInfo( )
{
	server.flAverageScorePerSecond = 0.f;
	server.iTickRate = 1.f / I::GlobalVars->interval_per_tick;
	server.flMultiplier = 66.7 / server.iTickRate;
	//Utils::ConLog( "CheaterDetection[UTIL]", tfm::format( "Calculated server tickrate & created appropriate multiplier {%.1f | %.1f}.", server.iTickRate, server.flMultiplier ).c_str( ), { 224, 255, 131, 255 } );
}

void CCheaterDetection::FindScores( )
{	//	runs every 30 seconds
	if ( I::GlobalVars->curtime - flLastScoreTime < 30.f ) { return; }
	CTFPlayerResource *cResource = g_EntityCache.GetPR( );
	if ( !cResource ) { return; }
	flLastScoreTime = I::GlobalVars->curtime;

	float flTotalAvg = 0;
	int iTotalPlayers = 0;
	for ( int n = 1; n < I::EngineClient->GetMaxClients( ); n++ )
	{
		CTFPlayer *pEntity = I::ClientEntityList->GetClientEntity( n )->As<CTFPlayer>( );
		if ( !pEntity || !cResource->GetValid( pEntity->entindex( ) ) ) { continue; }
		const float iScore = cResource->GetScore( pEntity->entindex( ) ) / cResource->GetConnectionTime( pEntity->entindex( ) );
		flTotalAvg += iScore; iTotalPlayers++;	//	used for calculating the average score
	}

	if ( !flTotalAvg || !iTotalPlayers ) { return; }

	// now that we've gone through all players (including local) find the avg
	server.flAverageScorePerSecond = ( flTotalAvg / ( float )iTotalPlayers );
	//Utils::ConLog( "CheaterDetection[UTIL]", tfm::format( "Calculated avg. server score per second at %.1f.", server.flAverageScorePerSecond ).c_str( ), { 224, 255, 131, 255 } );
}

void CCheaterDetection::FindHitchances( )
{	//	runs every 30 seconds
	if ( I::GlobalVars->curtime - flLastAccuracyTime < 30.f ) { return; }
	flLastAccuracyTime = I::GlobalVars->curtime;

	const float flAvg = ( float )server.iHits / ( float )server.iMisses;
	server.flHighAccuracy = std::clamp( flAvg * 2, .001f, .95f );

	//Utils::ConLog( "CheaterDetection[UTIL]", tfm::format( "Calculated server hitchance data {%.5f | %.5f}", flAvg, server.flHighAccuracy ).c_str( ), { 224, 255, 131, 255 } );
}

void CCheaterDetection::Reset( )
{
	server = ServerData{};
	mData.clear( );
	mFired.clear( );
	iLastScanTick = 0;
	flLastFrameTime = 0.f;
	flFirstScanTime = 0.f;
	flScanningTime = 0.f;
	flLastScoreTime = 0.f;
	flLastAccuracyTime = 0.f;
}

void CCheaterDetection::OnLoad( )
{
	Reset( );
	FillServerInfo( );
	flLastAccuracyTime = I::GlobalVars->curtime; flLastScoreTime = I::GlobalVars->curtime;
}

void CCheaterDetection::CalculateHitChance( CTFPlayer *pEntity )
{
	mData[ pEntity ].flHitchance = ( float )mData[ pEntity ].pShots.first / ( float )mData[ pEntity ].pShots.second;
}

void CCheaterDetection::ReportShot( int iIndex )
{
	CTFPlayer *pEntity = I::ClientEntityList->GetClientEntity( iIndex )->As<CTFPlayer>( );
	if ( !pEntity ) { return; }
	mData[ pEntity ].pShots.second++;
	server.iMisses++;
}

void CCheaterDetection::ReportDamage( IGameEvent *pEvent )
{
	const int iIndex = pEvent->GetInt( "attacker" );
	if ( iIndex == I::EngineClient->GetLocalPlayer( ) ) { return; }
	CTFPlayer *pEntity = I::ClientEntityList->GetClientEntity( iIndex )->As<CTFPlayer>( );
	if ( !pEntity || pEntity->IsDormant( ) ) { return; }
	CTFWeaponBase *pWeapon = GET_ENT_H( pEntity->m_hActiveWeapon( ) )->As<CTFWeaponBase>( );
	if ( !pWeapon ) { return; }
	AimbotCheck( pEntity );
	if ( I::GlobalVars->tickcount - mData[ pEntity ].iLastDamageEventTick <= 1 ) { return; }
	mData[ pEntity ].iLastDamageEventTick = I::GlobalVars->tickcount;
	mData[ pEntity ].pShots.first++; mData[ pEntity ].bDidDamage = true;
	server.iHits++;
}