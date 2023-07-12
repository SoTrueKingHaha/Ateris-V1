#include "MoveSim.h"

//we'll use this to set current player's command, without it CGameMovement::CheckInterval will try to access a nullptr
static CUserCmd DummyCmd = {};

//since we're going to call game functions some entity data will be modified (we'll modify it too), we'll have to restore it after running
class CPlayerDataBackup
{
public:
	Vec3 m_vecOrigin = {};
	Vec3 m_vecVelocity = {};
	Vec3 m_vecBaseVelocity = {};
	Vec3 m_vecViewOffset = {};
	Vec3 m_vecWorldSpaceCenter = {};
	int m_hGroundEntity = 0;
	int m_fFlags = 0;
	float m_flDucktime = 0.0f;
	float m_flDuckJumpTime = 0.0f;
	bool m_bDucked = false;
	bool m_bDucking = false;
	bool m_bInDuckJump = false;
	float m_flModelScale = 0.0f;

public:
	void Store( CTFPlayer *pPlayer )
	{
		m_vecOrigin = pPlayer->m_vecOrigin( );
		m_vecVelocity = pPlayer->m_vecVelocity( );
		m_vecBaseVelocity = pPlayer->m_vecBaseVelocity( );
		m_vecViewOffset = pPlayer->m_vecViewOffset( );
		m_hGroundEntity = pPlayer->m_hGroundEntity( );
		m_vecWorldSpaceCenter = pPlayer->GetWorldSpaceCenter( );
		m_fFlags = pPlayer->m_fFlags( );
		m_flDucktime = pPlayer->m_flDucktime( );
		m_flDuckJumpTime = pPlayer->m_flDuckJumpTime( );
		m_bDucked = pPlayer->m_bDucked( );
		m_bDucking = pPlayer->m_bDucking( );
		m_bInDuckJump = pPlayer->m_bInDuckJump( );
		m_flModelScale = pPlayer->m_flModelScale( );
	}

	void Restore( CTFPlayer *pPlayer )
	{
		pPlayer->m_vecOrigin( ) = m_vecOrigin;
		pPlayer->m_vecVelocity( ) = m_vecVelocity;
		pPlayer->m_vecBaseVelocity( ) = m_vecBaseVelocity;
		pPlayer->m_vecViewOffset( ) = m_vecViewOffset;
		pPlayer->m_hGroundEntity( ) = m_hGroundEntity;
		pPlayer->m_fFlags( ) = m_fFlags;
		pPlayer->m_flDucktime( ) = m_flDucktime;
		pPlayer->m_flDuckJumpTime( ) = m_flDuckJumpTime;
		pPlayer->m_bDucked( ) = m_bDucked;
		pPlayer->m_bDucking( ) = m_bDucking;
		pPlayer->m_bInDuckJump( ) = m_bInDuckJump;
		pPlayer->m_flModelScale( ) = m_flModelScale;
	}
};

static CPlayerDataBackup PlayerDataBackup = {};

void CMovementSimulation::SetupMoveData( CTFPlayer *pPlayer, CMoveData *pMoveData )
{
	if ( !pPlayer || !pMoveData )
	{
		return;
	}

	pMoveData->m_bFirstRunOfFunctions = false;
	pMoveData->m_bGameCodeMovedPlayer = false;
	pMoveData->m_nPlayerHandle = reinterpret_cast< IHandleEntity * >( pPlayer )->GetRefEHandle( );
	pMoveData->m_vecVelocity = pPlayer->m_vecVelocity( );	//	m_vecBaseVelocity hits -1950?
	pMoveData->m_vecAbsOrigin = pPlayer->m_vecOrigin( );
	pMoveData->m_flMaxSpeed = pPlayer->CalculateMaxSpeed( );

	if ( PlayerDataBackup.m_fFlags & FL_DUCKING )
	{
		pMoveData->m_flMaxSpeed *= 0.3333f;
	}

	pMoveData->m_flClientMaxSpeed = pMoveData->m_flMaxSpeed;

	//need a better way to determine angles probably
	pMoveData->m_vecViewAngles = { 0.0f, Utils::VelocityToAngles( pMoveData->m_vecVelocity ).y, 0.0f };

	Vec3 vForward = {}, vRight = {};
	Utils::AngleVectors( pMoveData->m_vecViewAngles, &vForward, &vRight, nullptr );

	pMoveData->m_flForwardMove = ( pMoveData->m_vecVelocity.y - vRight.y / vRight.x * pMoveData->m_vecVelocity.x ) / ( vForward.y - vRight.y / vRight.x * vForward.x );
	pMoveData->m_flSideMove = ( pMoveData->m_vecVelocity.x - vForward.x * pMoveData->m_flForwardMove ) / vRight.x;
}

bool CMovementSimulation::Initialize( CTFPlayer *pPlayer )
{
	if ( !I::CTFGameMovement || !pPlayer || pPlayer->deadflag( ) )
	{
		if ( !I::CTFGameMovement )
			I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "nigger gamemovement" );
		return false;
	}

	//set player
	m_pPlayer = pPlayer;

	//set current command
	m_pPlayer->SetCurrentCmd( &DummyCmd );
	//m_pPlayer->SetCurrentCommand(&DummyCmd);

	//store player's data
	PlayerDataBackup.Store( m_pPlayer );

	//store vars
	m_bOldInPrediction = I::Prediction->m_bInPrediction;
	m_bOldFirstTimePredicted = I::Prediction->m_bFirstTimePredicted;
	m_flOldFrametime = I::GlobalVars->frametime;
	bDontPredict = false;

	//the hacks that make it work
	{
		if ( pPlayer->m_fFlags( ) & FL_DUCKING )
		{
			pPlayer->m_fFlags( ) &= ~FL_DUCKING; //breaks origin's z if FL_DUCKING is not removed
			pPlayer->m_bDucked( ) = true; //(mins/maxs will be fine when ducking as long as m_bDucked is true)
			pPlayer->m_flDucktime( ) = 0.0f;
			pPlayer->m_flDuckJumpTime( ) = 0.0f;
			pPlayer->m_bDucking( ) = false;
			pPlayer->m_bInDuckJump( ) = false;
		}

		if ( pPlayer != LOCAL_PLAYER )
		{
			pPlayer->m_hGroundEntity( ) = 0; //without this nonlocal players get snapped to the floor
		}

		pPlayer->m_flModelScale( ) -= 0.03125f; //fixes issues with corners

		if ( pPlayer->m_fFlags( ) & FL_ONGROUND )
		{
			pPlayer->m_vecOrigin( ).z += 0.03125f; //to prevent getting stuck in the ground
		}

		//for some reason if xy vel is zero it doesn't predict
		if ( fabsf( pPlayer->m_vecVelocity( ).x ) < 0.01f )
		{
			pPlayer->m_vecVelocity( ).x = 0.015f;
		}

		if ( fabsf( pPlayer->m_vecVelocity( ).y ) < 0.01f )
		{
			pPlayer->m_vecVelocity( ).y = 0.015f;
		}
	}

	//setup move data
	SetupMoveData( m_pPlayer, &m_MoveData );

	return true;
}

void CMovementSimulation::Restore( )
{
	if ( !m_pPlayer )
	{
		return;
	}

	m_pPlayer->SetCurrentCmd( nullptr );

	PlayerDataBackup.Restore( m_pPlayer );

	I::Prediction->m_bInPrediction = m_bOldInPrediction;
	I::Prediction->m_bFirstTimePredicted = m_bOldFirstTimePredicted;
	I::GlobalVars->frametime = m_flOldFrametime;

	m_pPlayer = nullptr;

	memset( &m_MoveData, 0, sizeof( CMoveData ) );
	memset( &PlayerDataBackup, 0, sizeof( CPlayerDataBackup ) );
}

void CMovementSimulation::FillVelocities( )
{
	const auto &pLocal = g_EntityCache.GetLocal( );

	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
	{
		const auto &pPlayer = pEntity->As<CTFPlayer>( );
		const int iEntIndex = pPlayer->entindex( );

		if ( pPlayer == pLocal || pPlayer->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
		{
			continue;
		}

		if ( pPlayer->deadflag( ) || pPlayer->IsDormant( ) || pPlayer->m_vecVelocity( ).IsZero( ) )
		{
			m_Velocities[ iEntIndex ].clear( );
			continue;
		}

		const Vec3 vVelocity = pPlayer->m_vecVelocity( );
		m_Velocities[ iEntIndex ].push_front( vVelocity );

		while ( m_Velocities[ iEntIndex ].size( ) > 64 /*Vars::Aimbot::Projectile::StrafePredictionSamples.var*/ )
		{
			m_Velocities[ iEntIndex ].pop_back( );
		}
	}
}

bool CMovementSimulation::StrafePrediction( )
{
	static float flAverageYaw = 0.f;
	static float flInitialYaw = 0.f;
	/*const bool shouldPredict = m_pPlayer->IsOnGround( ) ? false : true;
	if ( !shouldPredict ) { return false; }*/

	if ( bFirstRunTick )
	{			//	we've already done the math, don't do it again
		flAverageYaw = 0.f;
		flInitialYaw = 0.f;
		bFirstRunTick = false;	//	if we fail the math here, don't try it again, it won't work.

		if ( const auto &pLocal = g_EntityCache.GetLocal( ) )
		{
			if ( pLocal->GetAbsOrigin( ).DistTo( m_pPlayer->GetAbsOrigin( ) ) > 1000.f /*Vars::Aimbot::Projectile::StrafePredictionMaxDistance.var*/ )
			{
				return false;
			}
		}

		const int iEntIndex = m_pPlayer->entindex( );

		const auto &mVelocityRecord = m_Velocities[ iEntIndex ];

		if ( static_cast< int >( mVelocityRecord.size( ) ) < 1 )
		{
			return false;
		}

		const int iSamples = 64;
		flInitialYaw = Utils::VelocityToAngles( m_MoveData.m_vecVelocity ).y;
		float flCompareYaw = flInitialYaw;

		int i = 0;
		for ( ; i < iSamples; i++ )
		{
			const float flRecordYaw = Utils::VelocityToAngles( mVelocityRecord.at( i ) ).y;

			/*float flFinal = ( flCompareYaw - flRecordYaw );
			flFinal = ( ( flFinal + 180 ) - floor( flFinal / 360 ) * 360 ) - 180;
			flAverageYaw += flFinal;*/
			const float flDelta = Utils::AngleDiffRad( flCompareYaw * ( ( float )PI / 180.f ), flRecordYaw * ( ( float )PI / 180.f ) ) * ( 180.f / ( float )PI );
			flAverageYaw += flDelta;

			flCompareYaw = flRecordYaw;
		}

		flAverageYaw /= i;

		while ( flAverageYaw > 360.f ) { flAverageYaw -= 360.f; }
		while ( flAverageYaw < -360.f ) { flAverageYaw += 360.f; }

		/*if ( fabsf( flAverageYaw ) < 30 /*Vars::Aimbot::Projectile::StrafePredictionMinDifference.var/ )
		{
			return false;
		}*/

		/*if ( Vars::Debug::DebugInfo.var )
		{
			Utils::ConLog( "MovementSimulation", tfm::format( "flAverageYaw calculated to %f", flAverageYaw ).c_str( ), { 83, 255, 83, 255 } );
		}*/

		const float flMaxDelta = ( 60.f / fmaxf( ( float )iSamples / 2.f, 1.f ) );
		if ( fabsf( flAverageYaw ) > flMaxDelta ) {
			m_Velocities[ m_pPlayer->entindex( ) ].clear( );
			return false;
		}

		//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "[MoveSim] flAverageYaw calculated to %f \n", flAverageYaw );
	}
	if ( flAverageYaw == 0.f ) { return false; }	//	fix

	flInitialYaw += flAverageYaw;
	m_MoveData.m_vecViewAngles.y = flInitialYaw;

	return true;
}

void CMovementSimulation::RunTick( CMoveData &moveDataOut, Vec3 &m_vecAbsOrigin )
{
	if ( !I::CTFGameMovement || !m_pPlayer || bDontPredict )
	{
		return;
	}

	if ( m_pPlayer->GetClassId( ) != EClassIds::CTFPlayer )
	{
		return;
	}

	bFirstRunTick = true;

	//make sure frametime and prediction vars are right
	I::Prediction->m_bInPrediction = true;
	I::Prediction->m_bFirstTimePredicted = false;
	I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	//if ( !StrafePrediction( ) )
		m_MoveData.m_vecViewAngles = { 0.0f, Utils::VelocityToAngles( m_MoveData.m_vecVelocity ).y, 0.0f };
	
	//call CTFGameMovement::ProcessMovement
	using ProcessMovement_FN = void( __thiscall * )( void *, CBaseEntity *, CMoveData * );
	reinterpret_cast< ProcessMovement_FN >( Utils::GetVFuncPtr( I::CTFGameMovement, 1 ) )( I::CTFGameMovement, m_pPlayer, &m_MoveData );

	g_GlobalInfo.PredictionLines.push_back( m_MoveData.m_vecAbsOrigin );

	moveDataOut = m_MoveData;
	m_vecAbsOrigin = m_MoveData.m_vecAbsOrigin;
}
