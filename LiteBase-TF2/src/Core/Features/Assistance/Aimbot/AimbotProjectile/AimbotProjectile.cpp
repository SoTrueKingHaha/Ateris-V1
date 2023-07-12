#include "AimbotProjectile.h"
#include "../../../Vars.h"
#include "../MoveSim/MoveSim.h"
#include "../../../../SDK/Include/EntityCache/EntityCache.h"
#include "../../../Exploits/CritHack/CritHack.h"

Vec3 CAimbotProjectile::Predictor_t::Extrapolate( float time )
{
	g_GlobalInfo.linearPredLine = m_vPosition;

	Vec3 vecOut;
	if ( m_pEntity->As<CTFPlayer>()->IsOnGround( ) )
	{
		vecOut = ( m_vPosition + ( m_vVelocity * time ) );
	}
	else
	{
		vecOut = ( m_vPosition + ( m_vVelocity * time ) - m_vAcceleration * 0.5f * time * time );
	}

	return vecOut;
}

bool CAimbotProjectile::GetProjectileInfo( CTFWeaponBase *pWeapon, ProjectileInfo_t &out )
{
	switch ( pWeapon->m_iItemDefinitionIndex( ) )
	{
		case Soldier_m_RocketLauncher:
		case Soldier_m_RocketLauncherR:
		case Soldier_m_TheBlackBox:
		case Soldier_m_TheCowMangler5000:
		case Soldier_m_TheOriginal:
		case Soldier_m_FestiveRocketLauncher:
		case Soldier_m_TheBeggarsBazooka:
		case Soldier_m_SilverBotkillerRocketLauncherMkI:
		case Soldier_m_GoldBotkillerRocketLauncherMkI:
		case Soldier_m_RustBotkillerRocketLauncherMkI:
		case Soldier_m_BloodBotkillerRocketLauncherMkI:
		case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
		case Soldier_m_DiamondBotkillerRocketLauncherMkI:
		case Soldier_m_SilverBotkillerRocketLauncherMkII:
		case Soldier_m_GoldBotkillerRocketLauncherMkII:
		case Soldier_m_FestiveBlackBox:
		case Soldier_m_TheAirStrike:
		case Soldier_m_WoodlandWarrior:
		case Soldier_m_SandCannon:
		case Soldier_m_AmericanPastoral:
		case Soldier_m_SmalltownBringdown:
		case Soldier_m_ShellShocker:
		case Soldier_m_AquaMarine:
		case Soldier_m_Autumn:
		case Soldier_m_BlueMew:
		case Soldier_m_BrainCandy:
		case Soldier_m_CoffinNail:
		case Soldier_m_HighRollers:
		case Soldier_m_Warhawk:
		{
			out = { 1100.0f, 0.0f };
			m_bIsBoosted = true;
			break;
		}

		case Soldier_m_TheDirectHit:
		{
			out = { 1980.0f, 0.0f };
			m_bIsBoosted = true;
			break;
		}

		case Soldier_m_TheLibertyLauncher:
		{
			out = { 1540.0f, 0.0f };
			m_bIsBoosted = true;
			break;
		}

		case Demoman_m_GrenadeLauncher:
		case Demoman_m_GrenadeLauncherR:
		case Demoman_m_FestiveGrenadeLauncher:
		case Demoman_m_TheIronBomber:
		case Demoman_m_Autumn:
		case Demoman_m_MacabreWeb:
		case Demoman_m_Rainbow:
		case Demoman_m_SweetDreams:
		case Demoman_m_CoffinNail:
		case Demoman_m_TopShelf:
		case Demoman_m_Warhawk:
		case Demoman_m_ButcherBird:
		{
			out = { 1216.f, 0.4f };
			m_bIsBoosted = true;
			break;
		}

		case Soldier_s_TheRighteousBison:
		case Engi_m_ThePomson6000:
		{
			out = { 1200.0f, 0.0f };
			break;
		}

		case Demoman_m_TheLooseCannon:
		{
			out = { 1453.9f, 0.4f };
			m_bIsBoosted = true;
			break;
		}

		case Demoman_m_TheLochnLoad:
		{
			out = { 1513.3f, 0.4f };
			m_bIsBoosted = true;
			break;
		}

		case Engi_m_TheRescueRanger:
		case Medic_m_FestiveCrusadersCrossbow:
		case Medic_m_CrusadersCrossbow:
		{
			out = { 2400.0f, 0.2f };
			break;
		}

		case Pyro_m_DragonsFury:
		{
			out = { 3000.0f, 0.0f, 0.1753f };
			m_bIsFlameThrower = true;
			break;
		}

		case Pyro_m_FlameThrower:
		case Pyro_m_FlameThrowerR:
		case Pyro_m_TheBackburner:
		case Pyro_m_TheDegreaser:
		case Pyro_m_ThePhlogistinator:
		case Pyro_m_FestiveFlameThrower:
		case Pyro_m_TheRainblower:
		case Pyro_m_SilverBotkillerFlameThrowerMkI:
		case Pyro_m_GoldBotkillerFlameThrowerMkI:
		case Pyro_m_RustBotkillerFlameThrowerMkI:
		case Pyro_m_BloodBotkillerFlameThrowerMkI:
		case Pyro_m_CarbonadoBotkillerFlameThrowerMkI:
		case Pyro_m_DiamondBotkillerFlameThrowerMkI:
		case Pyro_m_SilverBotkillerFlameThrowerMkII:
		case Pyro_m_GoldBotkillerFlameThrowerMkII:
		case Pyro_m_FestiveBackburner:
		case Pyro_m_ForestFire:
		case Pyro_m_BarnBurner:
		case Pyro_m_BovineBlazemaker:
		case Pyro_m_EarthSkyandFire:
		case Pyro_m_FlashFryer:
		case Pyro_m_TurbineTorcher:
		case Pyro_m_Autumn:
		case Pyro_m_PumpkinPatch:
		case Pyro_m_Nutcracker:
		case Pyro_m_Balloonicorn:
		case Pyro_m_Rainbow:
		case Pyro_m_CoffinNail:
		case Pyro_m_Warhawk:
		case Pyro_m_NostromoNapalmer:
		{
			out = { 1000.0f, 0.0f, 0.33f };
			m_bIsFlameThrower = true;
			break;
		}

		case Pyro_s_TheDetonator:
		case Pyro_s_TheFlareGun:
		case Pyro_s_FestiveFlareGun:
		case Pyro_s_TheScorchShot:
		{
			out = { 2000.0f, 0.3f };
			break;
		}

		case Pyro_s_TheManmelter:
		case Scout_s_TheFlyingGuillotine:
		case Scout_s_TheFlyingGuillotineG:
		{
			out = { 3000.0f, 0.2f };
			break;
		}

		case Medic_m_SyringeGun:
		case Medic_m_SyringeGunR:
		case Medic_m_TheBlutsauger:
		case Medic_m_TheOverdose:
		{
			out = { 1000.0f, 0.2f };
			break;
		}

		case Sniper_m_TheHuntsman:
		case Sniper_m_FestiveHuntsman:
		case Sniper_m_TheFortifiedCompound:
		{
			const float charge = ( I::GlobalVars->curtime - pWeapon->As<CTFPipebombLauncher>( )->m_flChargeBeginTime( ) );
			out = {
				Utils::RemapValClamped( charge, 0.0f, 1.f, 1800.f, 2600.f ),
				Utils::RemapValClamped( charge, 0.0f, 1.f, 0.5f, 0.1f )
			};
			break;
		}
		case Demoman_s_StickybombLauncher:
		case Demoman_s_StickybombLauncherR:
		case Demoman_s_FestiveStickybombLauncher:
		case Demoman_s_TheScottishResistance:
		{
			//Probably wrong
			const float charge = ( I::GlobalVars->curtime - pWeapon->As<CTFPipebombLauncher>( )->m_flChargeBeginTime( ) );
			out = {
				Utils::RemapValClamped( charge, 0.0f, 4.f, 900.f, 2400.f ),
				Utils::RemapValClamped( charge, 0.0f, 4.f, 0.483f, 0.085f )
			};
			break;
		}
		case Demoman_s_TheQuickiebombLauncher:
		{
			//Probably wrong
			const float charge = ( I::GlobalVars->curtime - pWeapon->As<CTFPipebombLauncher>( )->m_flChargeBeginTime( ) );
			out = {
				Utils::RemapValClamped( charge, 0.0f, 1.f, 930.88f, 2409.2f ),
				Utils::RemapValClamped( charge, 0.0f, 1.f, 0.483f, 0.085f )
			};

			break;
		}
	}

	return out.m_flVelocity;
}

bool CAimbotProjectile::CalcProjAngle( const Vec3 &vLocalPos, const Vec3 &vTargetPos, const ProjectileInfo_t &projInfo, Solution_t &out )
{
	const Vec3 v = vTargetPos - vLocalPos;
	const float dx = sqrt( v.x * v.x + v.y * v.y );
	const float dy = v.z;
	const float v0 = projInfo.m_flVelocity;

	//Ballistics
	if ( const float g = g_ConVars.sv_gravity->GetFloat( ) * projInfo.m_flGravity )
	{
		const float root = v0 * v0 * v0 * v0 - g * ( g * dx * dx + 2.0f * dy * v0 * v0 );

		if ( root < 0.0f )
		{
			return false;
		}

		out.m_flPitch = atan( ( v0 * v0 - sqrt( root ) ) / ( g * dx ) );
		out.m_flYaw = atan2( v.y, v.x );
	}

	//Straight trajectory (the time stuff later doesn't make sense with this but hey it works!)
	else
	{
		const Vec3 vecAngle = Utils::CalcAngle( vLocalPos, vTargetPos );
		out.m_flPitch = -DEG2RAD( vecAngle.x );
		out.m_flYaw = DEG2RAD( vecAngle.y );
	}

	out.m_flTime = dx / ( cos( out.m_flPitch ) * v0 );

	return true;
}

bool CAimbotProjectile::SolveProjectile( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, Predictor_t &predictor, const ProjectileInfo_t &projInfo, Solution_t &out )
{
	auto pNetChannel = I::EngineClient->GetNetChannelInfo( );
	static ConVar *cl_flipviewmodels = g_ConVars.cl_flipviewmodels;

	g_GlobalInfo.PredictionLines.clear( );

	if ( !g_GlobalInfo.m_bWeaponCanAttack ) {
		return true;
	}

	if ( !pNetChannel || !cl_flipviewmodels )
	{
		return false;
	}

	static bool oValue = cl_flipviewmodels->GetBool( ); // assume false

	Ray_t traceRay = {};
	CGameTrace trace = {};
	static CTraceFilterWorldAndPropsOnly traceFilter = {};
	traceFilter.pSkip = predictor.m_pEntity;

	Vec3 vLocalPos = pLocal->GetEyePosition( );
	const float maxTime = predictor.m_pEntity->IsPlayer( )
		? ( projInfo.m_flMaxTime == 0.f ? 3.0f : projInfo.m_flMaxTime ) 
		: ( projInfo.m_flMaxTime == 0.f ? 1024.f : projInfo.m_flMaxTime );
	const float fLatency = pNetChannel->GetLatency( MAX_FLOWS );

	/*
			This should now be able to predict anything that moves.
			Should also stop wasting time predicting static players.
	*/
	const bool useTPred = !predictor.m_pEntity->As<CTFPlayer>( )->m_vecVelocity( ).IsZero( ) ? true : false;

	if ( !useTPred ) {

		Vec3 staticPos = predictor.m_pEntity->IsPlayer( ) ? GetAimPos( pLocal, predictor.m_pEntity, predictor.m_vPosition ) : GetAimPosBuilding( pLocal, predictor.m_pEntity );
		if ( staticPos.IsZero( ) ) {
			return false;
		}

		// get angle offsets for demoman weapons?weew
		switch ( pWeapon->GetWeaponID( ) )
		{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			case TF_WEAPON_STICKBOMB:
			case TF_WEAPON_STICKY_BALL_LAUNCHER:
			{
				Vec3 vDelta = ( staticPos - vLocalPos );
				const float fRange = Utils::VectorNormalize( vDelta );
				const float fElevationAngle = std::min( fRange * ( pWeapon->m_iItemDefinitionIndex( ) == Demoman_m_TheLochnLoad ? 0.0075f : 0.013f ), 45.f ); // if our angle is above 45 degree will we even hit them? shouldn't we just return???

				float s = 0.0f, c = 0.0f;
				Utils::SinCos( ( fElevationAngle * PI / 180.0f ), &s, &c );

				const float fElevation = ( fRange * ( s / c ) );
				staticPos.z += ( c > 0.0f ? fElevation : 0.0f );
				break;
			}

			default: break;
		}

		// trace hull of projectile
		Utils::TraceHull( predictor.m_vPosition, staticPos, Vec3( -3.8f, -3.8f, -3.8f ), Vec3( 3.8f, 3.8f, 3.8f ), MASK_SOLID_BRUSHONLY, &traceFilter, &trace );
		if ( trace.DidHit( ) )
		{
			staticPos.z = trace.endpos.z;
		}

		switch ( pWeapon->GetWeaponID( ) )
		{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			case TF_WEAPON_STICKBOMB:
			case TF_WEAPON_STICKY_BALL_LAUNCHER:
			{
				Vec3 vecOffset( 16.0f, 8.0f, -6.0f );
				Utils::GetProjectileFireSetup( pLocal, pCmd->viewangles, vecOffset, &vLocalPos );
				break;
			}

			default: break;
		}

		if ( !CalcProjAngle( vLocalPos, staticPos, projInfo, out ) )
		{
			return false;
		}

		if ( out.m_flTime > maxTime ) {
			return false;
		}

		if ( WillProjectileHit( pLocal, pWeapon, pCmd, staticPos, out, projInfo, predictor ) ) {
			g_GlobalInfo.PredictedPos = staticPos;
			return true;
		}
	}
	else {
		Vec3 vPredictedPos = {};
		CMoveData moveData = {};
		Vec3 absOrigin = {};

		if ( g_MoveSim.Initialize( predictor.m_pEntity->As<CTFPlayer>( ) ) )
		{
			if ( !predictor.m_pEntity ) {
				g_MoveSim.Restore( );
				return false;
			}

			for ( int n = 0; n < TIME_TO_TICKS( fLatency ); n++ ) {
				//ik this causes issues with maxtime but I prefer this behaviour
				g_MoveSim.RunTick( moveData, absOrigin );
			}

			for ( int n = 0; n < TIME_TO_TICKS( maxTime ); n++ )
			{
				if ( predictor.m_pEntity == nullptr )
				{
					break;
				}
				g_MoveSim.RunTick( moveData, absOrigin );
				vPredictedPos = absOrigin;

				const Vec3 aimPosition = GetAimPos( pLocal, predictor.m_pEntity, vPredictedPos );
				if ( aimPosition.IsZero( ) ) {
					break;
				} // don't remove.

				//const Vec3 vAimDelta = predictor.m_pEntity->GetAbsOrigin() - aimPosition;
				//vPredictedPos.x += abs(vAimDelta.x);
				//vPredictedPos.y += abs(vAimDelta.y);
				//vPredictedPos.z += abs(vAimDelta.z);
				vPredictedPos = aimPosition;

				//Utils::RotateVec2( *( Vec2 * )&vPredictedPos, *( Vec2 * )&absOrigin, DEG2RAD( 0 * ( vPredictedPos - absOrigin ).Length2D( ) ) );
				//g_GlobalInfo.PredictionLines.push_back( vPredictedPos );

				// get angle offsets for demoman weapons?
				switch ( pWeapon->GetWeaponID( ) )
				{
					case TF_WEAPON_GRENADELAUNCHER:
					case TF_WEAPON_PIPEBOMBLAUNCHER:
					case TF_WEAPON_STICKBOMB:
					case TF_WEAPON_STICKY_BALL_LAUNCHER:
					{
						Vec3 vDelta = ( vPredictedPos - vLocalPos );
						const float fRange = Utils::VectorNormalize( vDelta );
						const float fElevationAngle = std::min( fRange * ( pWeapon->m_iItemDefinitionIndex( ) == Demoman_m_TheLochnLoad ? 0.0075f : 0.013f ), 45.f ); // if our angle is above 45 degree will we even hit them? shouldn't we just return???

						float s = 0.0f, c = 0.0f;
						Utils::SinCos( ( fElevationAngle * PI / 180.0f ), &s, &c );

						const float fElevation = ( fRange * ( s / c ) );
						vPredictedPos.z += ( c > 0.0f ? fElevation : 0.0f );
						break;
					}
					default: break;
				}

				//Utils::TraceHull(predictor.m_vPosition, vPredictedPos, Vec3(-3.8f, -3.8f, -3.8f), Vec3(3.8f, 3.8f, 3.8f),
				//	MASK_SOLID_BRUSHONLY, &traceFilter, &trace);

				//if (trace.DidHit())
				//{
				//	vPredictedPos.z = trace.vEndPos.z;
				g_GlobalInfo.PredictedPos = vPredictedPos;
				//}

				switch ( pWeapon->GetWeaponID( ) )
				{
					case TF_WEAPON_GRENADELAUNCHER:
					case TF_WEAPON_PIPEBOMBLAUNCHER:
					case TF_WEAPON_STICKBOMB:
					case TF_WEAPON_STICKY_BALL_LAUNCHER:
					{
						Vec3 vecOffset( 16.0f, 8.0f, -6.0f );
						Utils::GetProjectileFireSetup( pLocal, pCmd->viewangles, vecOffset, &vLocalPos );
						break;
					}

					default: break;
				}

				if ( !CalcProjAngle( vLocalPos, vPredictedPos, projInfo, out ) )
				{
					break;
				}

				out.m_flTime += fLatency;

				if ( out.m_flTime < TICKS_TO_TIME( n ) )
				{
					if ( WillProjectileHit( pLocal, pWeapon, pCmd, vPredictedPos, out, projInfo, predictor ) ) {

						g_GlobalInfo.PredictedPos = vPredictedPos;
						g_MoveSim.Restore( );
						return true;
					}
				}
			}
			g_MoveSim.Restore( );
		}
	}
	return false;
}

bool IsPointAllowed( int nHitbox ) {
	switch ( nHitbox ) {
		case 0: return Vars::Aimbot::Projectile::HeadAllowed.var;
		case 1: return Vars::Aimbot::Projectile::BodyAllowed.var;
		case 2: return Vars::Aimbot::Projectile::FeetAllowed.var;
		case 3:
		case 4:
		case 5:
		case 6: return Vars::Aimbot::Projectile::HeadAllowed.var;
		case 7:
		case 8:
		case 9:
		case 10: return Vars::Aimbot::Projectile::BodyAllowed.var;
		case 11:
		case 12:
		case 13:
		case 14: return Vars::Aimbot::Projectile::FeetAllowed.var;
		default: return true;
	}
	return true; // never
}

Vec3 getHeadOffset( CBaseEntity *pEntity ) {
	const Vec3 headPos = pEntity->As<CTFPlayer>( )->GetHitboxPos( HITBOX_HEAD );
	const Vec3 entPos = pEntity->GetAbsOrigin( );
	const Vec3 delta = entPos - headPos;
	return delta * -1.f;
}

//	Tries to find the best position to aim at on our target.
Vec3 CAimbotProjectile::GetAimPos( CTFPlayer *pLocal, CBaseEntity *pEntity, const Vec3 &targetPredPos )
{
	Vec3 retVec = pLocal->GetAbsOrigin( );
	Vec3 localPos = pLocal->GetAbsOrigin( );

	const Vec3 vLocalPos = pLocal->GetShootPos( );

	const bool bIsDucking = ( pEntity->As<CTFPlayer>( )->m_bDucked( ) || pEntity->As<CTFPlayer>( )->m_fFlags( ) & FL_DUCKING );

	const float bboxScale = 1.0f; // stop shoot flor (:D)

	// this way overshoots players that are crouching and I don't know why.
	const Vec3 vMaxs = I::GameMovement->GetPlayerMaxs( bIsDucking ) * bboxScale;
	const Vec3 vMins = Vec3( -vMaxs.x, -vMaxs.y, vMaxs.z - vMaxs.z * bboxScale );

	const Vec3 headDelta = getHeadOffset( pEntity );

	const std::vector vecPoints = {									// oh you don't like 15 points because it fucks your fps??? TOO BAD!//
		Vec3( headDelta.x, headDelta.y, vMaxs.z ),					//	head bone probably
		Vec3( 0, 0, ( vMins.z + vMaxs.z ) / 2 ),					//	middles (scan first bc they are more accurate)
		Vec3( 0, 0, vMins.z ),										//	-
		Vec3( vMins.x, vMins.y, vMaxs.z ),							//	top four corners
		Vec3( vMins.x, vMaxs.y, vMaxs.z ),							//	-
		Vec3( vMaxs.x, vMaxs.y, vMaxs.z ),							//	-
		Vec3( vMaxs.x, vMins.y, vMaxs.z ),							//	-
		Vec3( vMins.x, vMins.y, ( vMins.z + vMaxs.z ) / 2 ),		//	middle four corners
		Vec3( vMins.x, vMaxs.y, ( vMins.z + vMaxs.z ) / 2 ),		//	-
		Vec3( vMaxs.x, vMaxs.y, ( vMins.z + vMaxs.z ) / 2 ),		//	-
		Vec3( vMaxs.x, vMins.y, ( vMins.z + vMaxs.z ) / 2 ),		//	-
		Vec3( vMins.x, vMins.y, vMins.z ),							//	bottom four corners
		Vec3( vMins.x, vMaxs.y, vMins.z ),							//	-
		Vec3( vMaxs.x, vMaxs.y, vMins.z ),							//	-
		Vec3( vMaxs.x, vMins.y, vMins.z )							//	-
	};

	std::vector<Vec3> visiblePoints{};
	const matrix3x4_t transform = {
		{1.f, 0, 0, targetPredPos.x},
		{0, 1.f, 0, targetPredPos.y},
		{0, 0, 1.f, pEntity->As<CTFPlayer>( )->m_vecVelocity( ).IsZero( ) ? pEntity->GetAbsOrigin( ).z : targetPredPos.z}
	};

	int aimMethod = Vars::Aimbot::Projectile::AimPosition.var;
	int curPoint = 0, testPoints = 0; //maybe better way to do this
	for ( const auto &point : vecPoints )
	{
		if ( testPoints > 10 ) { break; }
		if ( static_cast< int >( visiblePoints.size( ) ) >= 3 ) { break; }
		if ( !IsPointAllowed( curPoint ) ) { curPoint++; continue; }

		Vec3 vTransformed = {};
		Utils::VectorTransform( point, transform, vTransformed );

		if ( Utils::VisPosMask( pLocal, pEntity, vLocalPos, vTransformed, MASK_SHOT_HULL ) )
		{
			if ( curPoint == aimMethod && aimMethod < 3 ) { return vTransformed; }	// return this value now if it is going to get returned anyway, avoid useless scanning.
			visiblePoints.push_back( vTransformed );
		}
		curPoint++;
		testPoints++;	// Only increment this if we actually tested.
	}
	if ( visiblePoints.empty( ) ) {
		return { 0, 0, 0 };
	}

	Vec3 HeadPoint, TorsoPoint, FeetPoint;

	const int classNum = pLocal->m_iClass( );

	switch ( classNum ) {
		case SOLDIER:
		{
			if ( g_EntityCache.GetWeapon( )->GetSlot( ) == SLOT_PRIMARY )
			{
				if ( Vars::Aimbot::Projectile::FeetAimIfOnGround.var && pEntity->As<CTFPlayer>( )->IsOnGround( ) )
				{
					if ( g_Crits.ShouldCrit( ) )
						aimMethod = 1;
					else
						aimMethod = 2;
				}
			}
			break;
		}
		case DEMOMAN:
		{
			if ( Vars::Aimbot::Projectile::FeetAimIfOnGround.var && pEntity->As<CTFPlayer>( )->IsOnGround( ) ) {
				if ( g_Crits.ShouldCrit( ) )
					aimMethod = 1;
				else
					aimMethod = 2;
			}
			break;
		}
	}

	if ( aimMethod == 3 && classNum ) { // auto
		switch ( classNum ) {
			case SOLDIER:
			case DEMOMAN:
			{
				aimMethod = 1;
				break;
			}
			case SNIPER:
			{
				aimMethod = 0;
				break;
			}
			default:
			{
				aimMethod = 1;
				break;
			}
		}
	}

	switch ( aimMethod ) {
		case 0: {	//head
			Utils::VectorTransform( vecPoints.at( 0 ), transform, HeadPoint );			//	get transformed location of our "best point" for our selected prio hitbox
			for ( const auto &aimPoint : visiblePoints ) {							//	iterate through visible points
				if ( aimPoint.DistTo( HeadPoint ) < retVec.DistTo( HeadPoint ) ) {		//	if the distance to our best point is lower than the previous selected point,
					retVec = aimPoint;												//	set the new point to our currently selected point
				}
			}
			break;
		}
		case 1: {	//torso
			Utils::VectorTransform( vecPoints.at( 1 ), transform, TorsoPoint );
			for ( const auto &aimPoint : visiblePoints ) {
				if ( aimPoint.DistTo( TorsoPoint ) < retVec.DistTo( TorsoPoint ) ) {
					retVec = aimPoint;
				}
			}
			break;
		}
		case 2: {	//feet
			Utils::VectorTransform( vecPoints.at( 2 ), transform, FeetPoint );
			for ( const auto &aimPoint : visiblePoints ) {
				if ( aimPoint.DistTo( FeetPoint ) < retVec.DistTo( FeetPoint ) ) {
					retVec = aimPoint;
				}
			}
			break;
		}
	}
	return retVec;
}

Vec3 CAimbotProjectile::GetAimPosBuilding( CTFPlayer *pLocal, CBaseEntity *pEntity ) {
	const Vec3 vLocalPos = pLocal->GetShootPos( );

	const float bboxScale = std::max( 1.0f - 0.05f, 0.5f );	// set the maximum scale for buildings at .95f

	const Vec3 vMins = pEntity->m_vecMins( ) * bboxScale;
	const Vec3 vMaxs = pEntity->m_vecMaxs( ) * bboxScale;

	const std::vector vecPoints = {
		Vec3( vMaxs.x / 2, vMaxs.y / 2, vMaxs.z / 2 ),								//	middle
		Vec3( vMins.x, vMins.y, vMaxs.z ),											//	top four corners
		Vec3( vMins.x, vMaxs.y, vMaxs.z ),											//	-
		Vec3( vMaxs.x, vMaxs.y, vMaxs.z ),											//	-
		Vec3( vMaxs.x, vMins.y, vMaxs.z ),											//	-
		Vec3( vMins.x, vMins.y, vMins.z ),											//	bottom four corners
		Vec3( vMins.x, vMaxs.y, vMins.z ),											//	-
		Vec3( vMaxs.x, vMaxs.y, vMins.z ),											//	-
		Vec3( vMaxs.x, vMins.y, vMins.z )											//	-
	};

	const matrix3x4_t &transform = pEntity->RenderableToWorldTransform( );

	for ( const auto &point : vecPoints )
	{
		Vec3 vTransformed = {};
		Utils::VectorTransform( point, transform, vTransformed );

		if ( Utils::VisPos( pLocal, pEntity, vLocalPos, vTransformed ) )
		{
			return vTransformed; // just return the first point we see
		}
	}

	return { 0, 0, 0 };
}

bool CAimbotProjectile::WillProjectileHit( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, const Vec3 &vPredictedPos, Solution_t &out, const ProjectileInfo_t &projInfo,
										   const Predictor_t &predictor )
{
	Vec3 hullSize = { 3.8f, 3.8f, 3.8f };
	Vec3 vVisCheck = pLocal->GetEyePosition( );
	const Vec3 predictedViewAngles = { -RAD2DEG( out.m_flPitch ), RAD2DEG( out.m_flYaw ), 0.0f };
	CGameTrace trace = {};
	static CTraceFilterWorldAndPropsOnly traceFilter = {};
	traceFilter.pSkip = predictor.m_pEntity;

	// this shit's messy
	{
		switch ( pWeapon->GetWeaponID( ) )
		{
			case TF_WEAPON_PARTICLE_CANNON: {
				hullSize = { 1.f, 1.f, 1.f };
				Vec3 vecOffset( 23.5f, 12.0f, -3.0f ); //tf_weaponbase_gun.cpp @L529 & @L760
				if ( pLocal->IsDucking( ) )
				{
					vecOffset.z = 8.0f;
				}
				Utils::GetProjectileFireSetup( pLocal, predictedViewAngles, vecOffset, &vVisCheck );
				break;
			}
			case TF_WEAPON_CROSSBOW: {
				hullSize = { 3.f, 3.f, 3.f };
				const Vec3 vecOffset( 23.5f, 12.0f, -3.0f );
				Utils::GetProjectileFireSetup( pLocal, predictedViewAngles, vecOffset, &vVisCheck );
				break;
			}
			case TF_WEAPON_FLAREGUN_REVENGE:
			case TF_WEAPON_ROCKETLAUNCHER:
			case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
			{
				hullSize = { 0.f, 3.7f, 3.7f };

				Vec3 vecOffset( 23.5f, 12.0f, -3.0f ); //tf_weaponbase_gun.cpp @L529 & @L760
				if ( pWeapon->m_iItemDefinitionIndex( ) == Soldier_m_TheOriginal )
				{
					vecOffset.y = 0.f;
				}
				if ( pLocal->IsDucking( ) )
				{
					vecOffset.z = 8.0f;
				}
				Utils::GetProjectileFireSetup( pLocal, predictedViewAngles, vecOffset, &vVisCheck );
				break;
			}
			case TF_WEAPON_SYRINGEGUN_MEDIC:
			{
				hullSize = { 0.f, 1.f, 1.f };

				const Vec3 vecOffset( 16.f, 6.f, -8.f ); //tf_weaponbase_gun.cpp @L628
				Utils::GetProjectileFireSetup( pLocal, predictedViewAngles, vecOffset, &vVisCheck );
				break;
			}
			case TF_WEAPON_COMPOUND_BOW:
			{
				hullSize = { 1.f, 1.f, 1.f };

				const Vec3 vecOffset( 23.5f, 12.0f, -3.0f ); //tf_weapon_grapplinghook.cpp @L355 ??
				Utils::GetProjectileFireSetup( pLocal, predictedViewAngles, vecOffset, &vVisCheck );
				break;
			}
			case TF_WEAPON_RAYGUN:
			case TF_WEAPON_DRG_POMSON:
			{
				hullSize = { 0.1f, 0.1f, 0.1f };
				Vec3 vecOffset( 23.5f, -8.0f, -3.0f ); //tf_weaponbase_gun.cpp @L568
				if ( pLocal->IsDucking( ) )
				{
					vecOffset.z = 8.0f;
				}
				Utils::GetProjectileFireSetup( pLocal, predictedViewAngles, vecOffset, &vVisCheck );
				break;
			}
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			case TF_WEAPON_STICKBOMB:
			case TF_WEAPON_STICKY_BALL_LAUNCHER:
			{

				hullSize = { 2.f, 2.f, 2.f };

				auto vecAngle = Vec3( ), vecForward = Vec3( ), vecRight = Vec3( ), vecUp = Vec3( );
				Utils::AngleVectors( { -RAD2DEG( out.m_flPitch ), RAD2DEG( out.m_flYaw ), 0.0f }, &vecForward, &vecRight, &vecUp );
				const Vec3 vecVelocity = ( ( vecForward * projInfo.m_flVelocity ) - ( vecUp * 200.0f ) );
				Utils::VectorAngles( vecVelocity, vecAngle );
				out.m_flPitch = -DEG2RAD( vecAngle.x );

				// see relevant code @tf_weaponbase_gun.cpp L684
				const float fRight = g_ConVars.cl_flipviewmodels->GetInt( ) ? -8.f : 8.f;
				vVisCheck = pLocal->GetShootPos( ) + vecForward * 16.0f + vecRight * fRight + vecUp * -6.0f;

				break;
			}
			default: break;
		}
	}

	//	TODO: find the actual hull size of projectiles
	//	maybe - https://www.unknowncheats.me/forum/team-fortress-2-a/475502-weapons-projectile-min-max-collideables.html
	//	UTIL_SetSize( this, -Vector( 1.0f, 1.0f, 1.0f ), Vector( 1.0f, 1.0f, 1.0f ) ); @tf_projectile_base.cpp L117
	//	UTIL_TraceHull( vecEye, vecSrc, -Vector(8,8,8), Vector(8,8,8), MASK_SOLID_BRUSHONLY, &traceFilter, &trace ); @tf_weaponbase_gun.cpp L696 pills
	Utils::TraceHull( vVisCheck, vPredictedPos, hullSize * 1.01f, hullSize * -1.01f, MASK_SHOT_HULL, &traceFilter, &trace );

	return !trace.DidHit( ) && !trace.startsolid;
}

ESortMethod CAimbotProjectile::GetSortMethod( )
{
	switch ( Vars::Aimbot::Projectile::SortMethod.var )
	{
		case 0: return ESortMethod::FOV;
		case 1: return ESortMethod::DISTANCE;
		default: return ESortMethod::UNKNOWN;
	}
}

/*
void ProjectileTracer( CTFPlayer *pLocal, const Target_t &target )
{
	if ( pLocal->InCond( TF_COND_TAUNTING ) )
	{
		return;
	}
	const Vec3 vecPos = g_GlobalInfo.m_WeaponType == EWeaponType::PROJECTILE ? g_GlobalInfo.PredictedPos : target.m_vPos;
	const Color_t tracerColor = { 0, 255, 0, 255 };
	Vec3 shootPos;
	const int iAttachment = LOCAL_WEAPON->LookupAttachment( "muzzle" );
	LOCAL_WEAPON->GetAttachment( iAttachment, shootPos );
	I::DebugOverlay->AddLineOverlayAlpha( shootPos, vecPos, tracerColor.r, tracerColor.g, tracerColor.b, tracerColor.a, true, 5 );
}
*/

bool CAimbotProjectile::GetTargets( CTFPlayer *pLocal, CTFWeaponBase *pWeapon )
{
	const ESortMethod SortMethod = GetSortMethod( );

	g_AimbotGlobal.m_vecTargets.clear( );

	const Vec3 vLocalPos = pLocal->GetShootPos( );
	const Vec3 vLocalAngles = I::EngineClient->GetViewAngles( );

	// Players
	if ( Vars::Aimbot::Global::AimPlayers.var )
	{
		const bool bIsCrossbow = pWeapon->GetWeaponID( ) == TF_WEAPON_CROSSBOW;

		for ( const auto &pEntity : g_EntityCache.GetGroup( bIsCrossbow ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES ) )
		{
			if ( pEntity->GetClassId( ) == EClassIds::CTFPlayer )
			{
				const auto &pPlayer = pEntity->As<CTFPlayer>( );

				if ( !pPlayer || pPlayer->deadflag( ) || pPlayer->IsDormant( ) || pPlayer->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) || pPlayer == pLocal )
					continue;

				if ( Vars::Aimbot::Global::IgnoreInvulnerable.var && ( pPlayer->InCond( TF_COND_INVULNERABLE ) || pPlayer->InCond( TF_COND_PHASE ) ) )
					continue;

				if ( Vars::Aimbot::Global::IgnoreCloaked.var && pPlayer->InCond( TF_COND_STEALTHED ) )
					continue;

				if ( Vars::Aimbot::Global::IgnoreTaunting.var && pPlayer->InCond( TF_COND_TAUNTING ) )
					continue;

				if ( Vars::Aimbot::Global::IgnoreFriends.var && pPlayer->PlayerOnFriendsList( ) && pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
					continue;

				player_info_t pi;
				if ( I::EngineClient->GetPlayerInfo( pPlayer->entindex( ), &pi ) )
				{
					if ( g_GlobalInfo.PlayerPriority[ pi.friendsID ].Mode == 1 )
					{
						continue;
					}
				}

				Vec3 vPos = pPlayer->GetWorldSpaceCenter( );
				Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
				float flFOVTo = ( SortMethod == ESortMethod::FOV ) ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;
				float flDistTo = ( SortMethod == ESortMethod::DISTANCE ) ? vLocalPos.DistTo( vPos ) : 0.0f;

				if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Projectile::AimFOV.var )
					continue;

				const auto &priority = g_AimbotGlobal.GetPriority( pPlayer->entindex( ) );

				g_AimbotGlobal.m_vecTargets.push_back( { pPlayer, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo, -1, false, priority } );
			}
		}
	}

	// Buildings
	if ( Vars::Aimbot::Global::AimBuildings.var )
	{
		const bool bIsRescueRanger = pWeapon->GetWeaponID( ) == TF_WEAPON_SHOTGUN_BUILDING_RESCUE;

		for ( const auto &pEntity : g_EntityCache.GetGroup( bIsRescueRanger ? EEntGroup::BUILDINGS_ALL : EEntGroup::SENTRY_ENEMIES ) )
		{
			const auto &pObject = pEntity->As<CBaseObject>( );

			if ( !pObject || pObject->IsDormant( ) )
				continue;

			Vec3 vPos = pObject->GetWorldSpaceCenter( );
			Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
			float flFOVTo = SortMethod == ESortMethod::FOV ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo( vPos ) : 0.0f;

			if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Projectile::AimFOV.var )
				continue;

			g_AimbotGlobal.m_vecTargets.push_back( { pObject, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo } );
		}
	}

	/*if ( !Vars::Aimbot::Global::IgnoreNPCs.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_NPC ) )
		{
			const auto &pNpc = pEntity->As<CBaseEntity>( );

			if ( !pNpc || pNpc->IsDormant( ) )
				continue;

			Vec3 vPos = pNpc->GetWorldSpaceCenter( );
			Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
			float flFOVTo = SortMethod == ESortMethod::FOV ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;

			if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.var )
				continue;

			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo( vPos ) : 0.0f;

			g_AimbotGlobal.m_vecTargets.push_back( { pNpc, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo } );
		}
	}*/

	return !g_AimbotGlobal.m_vecTargets.empty( );
}

bool CAimbotProjectile::VerifyTarget( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, Target_t &target )
{
	ProjectileInfo_t projInfo = {};
	if ( !GetProjectileInfo( pWeapon, projInfo ) )
	{
		return false;
	}

	if ( pLocal->InCond( TF_COND_RUNE_PRECISION ) && m_bIsBoosted ) {
		projInfo.m_flVelocity *= 2.5f;

	}

	auto vVelocity = Vec3( );
	auto vAcceleration = Vec3( );

	switch ( target.m_TargetType )
	{
		case ETargetType::PLAYER:
		{
			vVelocity = target.m_pEntity->GetVelocity( );
			vAcceleration = Vec3( 0.0f, 0.0f, g_ConVars.sv_gravity->GetFloat( ) * ( ( target.m_pEntity->As<CTFPlayer>( )->InCond( TF_COND_PARACHUTE_ACTIVE ) ) ? 0.224f : 1.0f ) );
			break;
		}

		default: break;
	}

	Predictor_t predictor = {
		target.m_pEntity,
		target.m_vPos,
		vVelocity,
		vAcceleration
	};

	Solution_t solution = {};

	if ( !SolveProjectile( pLocal, pWeapon, pCmd, predictor, projInfo, solution ) )
	{
		return false;
	}

	target.m_vAngleTo = { -RAD2DEG( solution.m_flPitch ), RAD2DEG( solution.m_flYaw ), 0.0f };

	return true;
}

// Returns the best target
bool CAimbotProjectile::GetTarget( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, Target_t &outTarget )
{
	if ( !GetTargets( pLocal, pWeapon ) )
	{
		return false;
	}

	g_AimbotGlobal.SortTargets( GetSortMethod( ) );

	//instead of this just limit to like 4-6 targets, should save perf without any noticeable changes in functionality
	for ( auto &target : g_AimbotGlobal.m_vecTargets )
	{
		if ( !VerifyTarget( pLocal, pWeapon, pCmd, target ) )
		{
			continue;
		}

		outTarget = target;
		return true;
	}

	return false;
}

// Aims at the given angles
void CAimbotProjectile::Aim( CUserCmd *pCmd, CTFWeaponBase *pWeapon, Vec3 &vAngle )
{
	//vAngle -= g_GlobalInfo.m_vPunchAngles;
	Utils::ClampAngles( vAngle );

	if ( Vars::Misc::StacBypass.var )
	{
		g_GlobalInfo.m_bNullCmd = true;
	}

	switch ( Vars::Aimbot::Projectile::AimMethod.var )
	{
		case 0:
		{
			// Plain
			pCmd->viewangles = vAngle;
			I::EngineClient->SetViewAngles( pCmd->viewangles );
			break;
		}

		case 1:
		{
			// Silent
			Utils::FixMovement( pCmd, vAngle );
			pCmd->viewangles = vAngle;
			break;
		}

		default: break;
	}

	if ( Vars::Misc::StacBypass.var )
	{
		g_GlobalInfo.m_bNullCmd = false;
	}
}

bool CAimbotProjectile::ShouldFire( CUserCmd *pCmd, Target_t target )
{
	//return ( Vars::Aimbot::Global::AutoShoot.var && g_GlobalInfo.m_bWeaponCanAttack );
	if ( !Vars::Aimbot::Global::AutoShoot.var )
		return false;

	const auto &pLocal = g_EntityCache.GetLocal( );
	if ( !target.m_pEntity->As<CTFPlayer>( )->m_vecVelocity( ).IsZero( ) )
	{
		if ( !Utils::VisPosMask( pLocal, target.m_pEntity, pLocal->GetShootPos( ), g_GlobalInfo.PredictedPos, MASK_SHOT_HULL ) )
		{
			return false;
		}
	}
	else
	{
		if ( !Utils::VisPosMask( pLocal, target.m_pEntity, pLocal->GetShootPos( ), target.m_vPos, MASK_SHOT_HULL ) )
		{
			return false;
		}
	}

	return true;
}

bool CAimbotProjectile::IsAttacking( const CUserCmd *pCmd, CTFWeaponBase *pWeapon )
{
	if ( pWeapon->m_iItemDefinitionIndex( ) == Soldier_m_TheBeggarsBazooka )
	{
		static bool bLoading = false;

		if ( pWeapon->m_iClip1( ) > 0 )
		{
			bLoading = true;
		}

		if ( !( pCmd->buttons & IN_ATTACK ) && bLoading )
		{
			bLoading = false;
			return true;
		}
	}
	else
	{
		if ( pWeapon->GetWeaponID( ) == TF_WEAPON_COMPOUND_BOW || pWeapon->GetWeaponID( ) == TF_WEAPON_PIPEBOMBLAUNCHER )
		{
			static bool bCharging = false;

			if ( pWeapon->As<CTFPipebombLauncher>( )->m_flChargeBeginTime( ) > 0.0f )
			{
				bCharging = true;
			}

			if ( !( pCmd->buttons & IN_ATTACK ) && bCharging )
			{
				bCharging = false;
				return true;
			}
		}
		else if ( pWeapon->GetWeaponID( ) == TF_WEAPON_CANNON )
		{
			static bool Charging = false;

			if ( pWeapon->As<CTFGrenadeLauncher>( )->m_flDetonateTime( ) > 0.0f )
				Charging = true;

			if ( !( pCmd->buttons & IN_ATTACK ) && Charging )
			{
				Charging = false;
				return true;
			}
		}

		//pssst..
		//Dragon's Fury has a gauge (seen on the weapon model) maybe it would help for pSilent hmm..
		/*
		if (pWeapon->GetWeaponID() == 109) {
		}*/

		else
		{
			if ( ( pCmd->buttons & IN_ATTACK ) && g_GlobalInfo.m_bWeaponCanAttack )
			{
				return true;
			}
		}
	}

	return false;
}

void CAimbotProjectile::Run( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd )
{
	static int nLastTracerTick = pCmd->tick_count;

	m_bIsFlameThrower = false;

	if ( !Vars::Aimbot::Global::Active.var || !Utils::IsGameWindowInFocus( ) )
	{
		return;
	}

	const bool bShouldAim = ( Vars::Aimbot::Global::AimKey.var == VK_LBUTTON
							  ? ( pCmd->buttons & IN_ATTACK )
							  : g_AimbotGlobal.IsKeyDown( ) );
	if ( !bShouldAim ) { return; }

	Target_t target{ };
	if ( GetTarget( pLocal, pWeapon, pCmd, target ) && bShouldAim )
	{
		// Aim at the current target
		g_GlobalInfo.m_nCurrentTargetIdx = target.m_pEntity->entindex( );

		if ( Vars::Aimbot::Projectile::AimMethod.var == 1 )
		{
			g_GlobalInfo.m_vAimPos = g_GlobalInfo.PredictedPos;
		}

		if ( ShouldFire( pCmd, target ) )
		{
			if ( Vars::Misc::StacBypass.var )
			{
				g_GlobalInfo.m_bNullCmd = true;
			}
			pCmd->buttons |= IN_ATTACK;
			if ( Vars::Misc::StacBypass.var )
			{
				g_GlobalInfo.m_bNullCmd = false;
			}
			g_GlobalInfo.m_vPredictedPos = g_GlobalInfo.PredictedPos;

			/*if ( Vars::Misc::CL_Move::Enabled.var && Vars::Misc::CL_Move::Doubletap.var && ( pCmd->buttons & IN_ATTACK ) && !g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_nWaitForShift && pLocal->IsOnGround( ) )
			{
				g_GlobalInfo.m_bShouldShift = true;
			}*/

			if ( g_GlobalInfo.m_bAAActive && !g_GlobalInfo.m_bWeaponCanAttack )
				pCmd->buttons &= ~IN_ATTACK;

			if ( pWeapon->m_iItemDefinitionIndex( ) == Soldier_m_TheBeggarsBazooka )
			{
				if ( pWeapon->m_iClip1( ) > 0 )
					pCmd->buttons &= ~IN_ATTACK;
			}
			else
			{
				if ( ( pWeapon->GetWeaponID( ) == TF_WEAPON_COMPOUND_BOW || pWeapon->GetWeaponID( ) == TF_WEAPON_PIPEBOMBLAUNCHER )
					 && pWeapon->As<CTFPipebombLauncher>( )->m_flChargeBeginTime( ) > 0.0f )
				{
					pCmd->buttons &= ~IN_ATTACK;
				}
				else if ( pWeapon->GetWeaponID( ) == TF_WEAPON_CANNON && pWeapon->As<CTFGrenadeLauncher>( )->m_flDetonateTime( ) > 0.0f )
				{
					const Vec3 vEyePos = pLocal->GetShootPos( );
					float BestCharge = vEyePos.DistTo( g_GlobalInfo.PredictedPos ) / 1453.9f;

					if ( Vars::Aimbot::Projectile::ChargeLooseCannon.var )
					{
						if ( pWeapon->As<CTFGrenadeLauncher>( )->m_flDetonateTime( ) - I::GlobalVars->curtime <= BestCharge )
							pCmd->buttons &= ~IN_ATTACK;
					}
					else
						pCmd->buttons &= ~IN_ATTACK;
				}
			}
		}

		const bool bIsAttacking = IsAttacking( pCmd, pWeapon );

		if ( bIsAttacking )
		{
			g_GlobalInfo.m_bAttacking = true;
			if ( Vars::Visuals::BulletTracer.var && abs( pCmd->tick_count - nLastTracerTick ) > 1 )
			{
				//ProjectileTracer( pLocal, target );
				nLastTracerTick = pCmd->tick_count;
			}
			g_GlobalInfo.PredLinesBackup.clear( );
			g_GlobalInfo.PredLinesBackup = g_GlobalInfo.PredictionLines;

			//I::DebugOverlay->AddLineOverlayAlpha( target.m_vPos, g_GlobalInfo.PredictedPos, 0, 255, 0, 255, true, 2 ); // Predicted aim pos
		}

		if ( Vars::Aimbot::Projectile::AimMethod.var == 1 )
		{
			if ( m_bIsFlameThrower && ( target.m_vAngleTo.x && target.m_vAngleTo.y && target.m_vAngleTo.z ) )
			{
				Aim( pCmd, pWeapon, target.m_vAngleTo );
				g_GlobalInfo.m_bProjectileSilentActive = true;
			}

			else
			{
				if ( bIsAttacking )
				{
					Aim( pCmd, pWeapon, target.m_vAngleTo );
					g_GlobalInfo.m_bSilentTime = true;
				}
			}
		}
		else
		{
			Aim( pCmd, pWeapon, target.m_vAngleTo );
		}
	}
}
