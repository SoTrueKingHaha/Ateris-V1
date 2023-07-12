#include "ConfigManager.h"

#include <string>

#include "../../../../../Vars.h"
#include "../../../../../../SDK/SDK.h"

#define SAVE_VAR(x) SaveVar((L#x), x.var)
#define LOAD_VAR(x) LoadVar((L#x), x.var)

#define SAVE_OTHER(x) SaveVar((L#x), x)
#define LOAD_OTHER(x) LoadVar((L#x), x)

//stfu
#pragma warning (disable : 6328)
#pragma warning (disable : 6031)
#pragma warning (disable : 4477)

bool CConfigManager::Find(const wchar_t *name, std::wstring &output)
{
	m_Read.clear();
	m_Read.seekg(0, std::ios::beg);

	while (std::getline(m_Read, output))
	{
		if (output.find(name) != std::wstring::npos)
			return true;
	}

	return false;
}

void CConfigManager::SaveVar(const wchar_t *name, bool val)
{
	char buffer[64];
	sprintf_s(buffer, "%ls: %d", name, val);
	m_Write << buffer << "\n";
}

void CConfigManager::SaveVar(const wchar_t *name, int val)
{
	char buffer[64];
	sprintf_s(buffer, "%ls: %d", name, val);
	m_Write << buffer << "\n";
}

void CConfigManager::SaveVar(const wchar_t *name, float val)
{
	char buffer[64];
	sprintf_s(buffer, "%ls: %f", name, val);
	m_Write << buffer << "\n";
}

void CConfigManager::SaveVar(const wchar_t *name, Color_t val)
{
	char buffer[64];
	sprintf_s(buffer, "%ls: %d %d %d %d", name, val.r, val.g, val.b, val.a);
	m_Write << buffer << "\n";
}

void CConfigManager::LoadVar(const wchar_t *name, bool &val)
{
	std::wstring line = {};

	if (Find(name, line))
		swscanf_s(line.c_str(), L"%*ls %d", &val);
}

void CConfigManager::LoadVar(const wchar_t *name, int &val)
{
	std::wstring line = {};

	if (Find(name, line))
		swscanf_s(line.c_str(), L"%*ls %d", &val);
}

void CConfigManager::LoadVar(const wchar_t *name, float &val)
{
	std::wstring line = {};

	if (Find(name, line))
		swscanf_s(line.c_str(), L"%*ls %f", &val);
}

void CConfigManager::LoadVar(const wchar_t *name, Color_t &val)
{
	std::wstring line = {};

	if (Find(name, line)) {
		int r = 0, g = 0, b = 0, a = 0;
		swscanf_s(line.c_str(), L"%*ls %d %d %d %d", &r, &g, &b, &a);
		val = { static_cast<byte>(r), static_cast<byte>(g), static_cast<byte>(b), static_cast<byte>(a) };
	}
}

CConfigManager::CConfigManager()
{
	m_sConfigPath = std::filesystem::current_path().wstring() + (L"\\ateris");

	if (!std::filesystem::exists(m_sConfigPath))
		std::filesystem::create_directory(m_sConfigPath);

	if ( !std::filesystem::exists( m_sConfigPath + L"\\Core" ) )
	{
		std::filesystem::create_directory( m_sConfigPath + L"\\Core" );
	}

	/*if ( !std::filesystem::exists( m_sConfigPath + ( L"\\CheatCore" ) ) )
		std::filesystem::create_directory(m_sConfigPath + (L"\\CheatCore"));*/
}

void CConfigManager::Save(const wchar_t *name)
{
	m_Write = std::wofstream(m_sConfigPath + L"\\" + name + (L".cfg"));

	if (m_Write.is_open())
	{
		//crithack
		{

			SAVE_VAR( Vars::CritHack::Active );
			SAVE_VAR( Vars::CritHack::Indicators );
			SAVE_VAR( Vars::CritHack::AvoidRandom );
			SAVE_VAR( Vars::CritHack::CritKey );

		}

		//antiaim
		/* {
			SAVE_VAR( Vars::AntiHack::AntiAim::Active );
			SAVE_VAR( Vars::AntiHack::AntiAim::ToggleKey );
			SAVE_VAR( Vars::AntiHack::AntiAim::Pitch ); //0 - None, 1 - Zero, 2 - Up, 3 - Down, 4 - Fake Up, 5 - Fake Down
			SAVE_VAR( Vars::AntiHack::AntiAim::YawReal ); //0 - None, 1 - Forward, 2 - Left, 3 - Right, 4 - Backwards
			SAVE_VAR( Vars::AntiHack::AntiAim::YawFake ); //0 - None, 1 - Forward, 2 - Left, 3 - Right, 4 - Backwards
			SAVE_VAR( Vars::AntiHack::AntiAim::SpinSpeed );
			SAVE_VAR( Vars::AntiHack::AntiAim::CustomRealPitch );
			SAVE_VAR( Vars::AntiHack::AntiAim::RandInterval );
			SAVE_VAR( Vars::AntiHack::AntiAim::AntiBackstab );
			SAVE_VAR( Vars::AntiHack::AntiAim::LegJitter );
			SAVE_VAR( Vars::AntiHack::AntiAim::AntiOverlap );
			SAVE_VAR( Vars::AntiHack::AntiAim::InvalidShootPitch );
		}*/

		//new aa
		{
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::Active );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::Rage );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::ToggleKey );
			//legit
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::LegitYaw );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::LegitYawMode );// 0 - Static,	1 - Edge
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::CycleKey );
			//rage
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeYaw );			// -180 / 180
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeMode );		// 0 - Static,	1 - Edge,	2 - Jitter,	3 - Spin
			//SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeRotate );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeDelta );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeSpin );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeAngle1 );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakeAngle2 );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::CycleFakeKey );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::CycleFakeJitter );

			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealYaw );			// -180 / 180
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealMode );		// 0 - Static,	1 - Edge,	2 - Jitter,	3 - Spin
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealRotate );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealDelta );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealSpin );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealAngle1 );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealAngle2 );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::CycleRealKey );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::CycleRealJitter );

			SAVE_VAR( Vars::AntiHack::AntiAimBeta::Pitch );			// 0 - None,	1 - Automatic,	2 - Manual
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::FakePitch );
			SAVE_VAR( Vars::AntiHack::AntiAimBeta::RealPitch );
		}

		//aimbot
		{
			SAVE_VAR(Vars::Aimbot::Global::Active );
			SAVE_VAR( Vars::Aimbot::Global::AimKey );
			SAVE_VAR( Vars::Aimbot::Global::AutoShoot );
			SAVE_VAR( Vars::Aimbot::Global::AimPlayers );
			SAVE_VAR( Vars::Aimbot::Global::AimBuildings );
			SAVE_VAR( Vars::Aimbot::Global::AimNPCs );
			SAVE_VAR( Vars::Aimbot::Global::AimStickies );
			//SAVE_VAR( Vars::Aimbot::Global::AmmoCheck );
			//SAVE_VAR( Vars::Aimbot::Global::AmmoCount );
			SAVE_VAR( Vars::Aimbot::Global::IgnoreInvulnerable );
			SAVE_VAR( Vars::Aimbot::Global::IgnoreCloaked );
			SAVE_VAR( Vars::Aimbot::Global::IgnoreFriends );
			SAVE_VAR( Vars::Aimbot::Global::IgnoreTaunting );

			//hitscan
			SAVE_VAR( Vars::Aimbot::Hitscan::Active );
			SAVE_VAR( Vars::Aimbot::Hitscan::SortMethod );	//0 - FOV,		1 - Distance
			SAVE_VAR( Vars::Aimbot::Hitscan::AimMethod );	//0 - Normal,	1 - Smooth, 2 - Silent
			SAVE_VAR( Vars::Aimbot::Hitscan::AimHitbox );	//0 - Head,		1 - Body,	2 - Auto
			SAVE_VAR( Vars::Aimbot::Hitscan::AimFOV );
			SAVE_VAR( Vars::Aimbot::Hitscan::SmoothingAmount );
			SAVE_VAR( Vars::Aimbot::Hitscan::TapFire );	//0 - Off, 1 - Distance, 2 - Always
			SAVE_VAR( Vars::Backtrack::Active );
			SAVE_VAR( Vars::Backtrack::Indicator );
			SAVE_VAR( Vars::Aimbot::Hitscan::ScanHitboxes );
			SAVE_VAR( Vars::Aimbot::Hitscan::ScanHead );
			SAVE_VAR( Vars::Aimbot::Hitscan::ScanBuildings );
			SAVE_VAR( Vars::Aimbot::Hitscan::WaitForHeadshot );
			SAVE_VAR( Vars::Aimbot::Hitscan::WaitForCharge );
			SAVE_VAR( Vars::Aimbot::Hitscan::SpectatedSmooth );
			SAVE_VAR( Vars::Aimbot::Hitscan::ScopedOnly );
			SAVE_VAR( Vars::Aimbot::Hitscan::AutoScope );
			SAVE_VAR( Vars::Aimbot::Hitscan::AutoRev );
			SAVE_VAR( Vars::Aimbot::Hitscan::ShowHitboxes );
			SAVE_VAR( Vars::Aimbot::Hitscan::HitboxDuration );

			SAVE_VAR( Vars::Aimbot::Projectile::Active );
			SAVE_VAR( Vars::Aimbot::Projectile::SortMethod );	//0 - FOV,		1 - Distance
			SAVE_VAR( Vars::Aimbot::Projectile::AimMethod );	//0 - Normal,	1 - Silent
			SAVE_VAR( Vars::Aimbot::Projectile::AimPosition );	//0 - Body,		1 - Feet,	2 - Auto
			SAVE_VAR( Vars::Aimbot::Projectile::AimFOV );
			SAVE_VAR( Vars::Aimbot::Projectile::RespectFOV );
			SAVE_VAR( Vars::Aimbot::Projectile::FeetAimIfOnGround );
			SAVE_VAR( Vars::Aimbot::Projectile::HeadAllowed );
			SAVE_VAR( Vars::Aimbot::Projectile::BodyAllowed );
			SAVE_VAR( Vars::Aimbot::Projectile::FeetAllowed );
			SAVE_VAR( Vars::Aimbot::Projectile::ChargeLooseCannon );
			SAVE_VAR( Vars::Aimbot::Projectile::PredictedPath );
			SAVE_VAR( Vars::Aimbot::Projectile::PathDuration );
			SAVE_VAR( Vars::Aimbot::Projectile::TargetBBox );
			SAVE_VAR( Vars::Aimbot::Projectile::BBoxDuration );
		}

		//triggerbot
		{
			SAVE_VAR( Vars::Triggerbot::Active);
			SAVE_VAR( Vars::Triggerbot::Key);
			SAVE_VAR( Vars::Triggerbot::TriggerPlayers );
			SAVE_VAR( Vars::Triggerbot::TriggerBuildings );
			SAVE_VAR( Vars::Triggerbot::HeadOnly );
			SAVE_VAR( Vars::Triggerbot::WaitForHeadshot );
			SAVE_VAR( Vars::Triggerbot::ScopeOnly );
			SAVE_VAR( Vars::Triggerbot::HeadScale );
		}

		//esp
		{
			SAVE_VAR( Vars::ESP::Players::esp_players );
			SAVE_VAR( Vars::ESP::Players::box );
			SAVE_VAR( Vars::ESP::Players::name );
			SAVE_VAR( Vars::ESP::Players::healthbar );
			SAVE_VAR( Vars::ESP::Players::gradient );
			SAVE_VAR( Vars::ESP::Players::lines );
			SAVE_VAR( Vars::ESP::Players::conds );
			SAVE_VAR( Vars::ESP::Players::weapons );
			SAVE_VAR( Vars::ESP::Players::skeleton );
			SAVE_VAR( Vars::ESP::Players::distance );
			SAVE_VAR( Vars::ESP::Buildings::esp_buildings );
			SAVE_VAR( Vars::ESP::Buildings::box );
			SAVE_VAR( Vars::ESP::Buildings::name );
			SAVE_VAR( Vars::ESP::Buildings::healthbar );
			SAVE_VAR( Vars::ESP::Buildings::gradient );
			SAVE_VAR( Vars::ESP::Buildings::lines );
			SAVE_VAR( Vars::ESP::Buildings::conds );
			SAVE_VAR( Vars::ESP::Buildings::distance );
			SAVE_VAR( Vars::ESP::World::esp_world );
			SAVE_VAR( Vars::ESP::World::box );
			SAVE_VAR( Vars::ESP::World::name );
			SAVE_VAR( Vars::ESP::World::distance );
			SAVE_VAR( Vars::ESP::Main::esp_global );
			SAVE_VAR( Vars::ESP::Main::esp_dormant );
			SAVE_VAR( Vars::ESP::Main::esp_cheaptext );
			SAVE_VAR( Vars::ESP::Main::esp_enemies );
		}

		//outlines
		{
			SAVE_VAR( Vars::Outline::Players::Enabled );
			SAVE_VAR( Vars::Outline::Players::AttachmentsOutline );
			SAVE_VAR( Vars::Outline::Players::ViewmodelOutline );
			SAVE_VAR( Vars::Outline::Players::OutlineScale );
			SAVE_VAR( Vars::Outline::Players::OutlineAlpha );
			SAVE_VAR( Vars::Outline::Buildings::Enabled );
			SAVE_VAR( Vars::Outline::Buildings::OutlineScale );
			SAVE_VAR( Vars::Outline::Buildings::OutlineAlpha );
			SAVE_VAR( Vars::Outline::World::Enabled );
			SAVE_VAR( Vars::Outline::World::OutlineScale );
			SAVE_VAR( Vars::Outline::World::OutlineAlpha );
			SAVE_VAR( Vars::Outline::Main::Enabled );
			SAVE_VAR( Vars::Outline::Main::BlurOutline );
			SAVE_VAR( Vars::Outline::Main::WireframeOutline );
			SAVE_VAR( Vars::Outline::Main::CheapOutline );
			SAVE_VAR( Vars::Outline::Main::Enemies );
		}

		//models
		{
			SAVE_VAR( Vars::Models::Players::Enabled );
			SAVE_VAR( Vars::Models::Players::Model );
			SAVE_VAR( Vars::Models::Players::IgnoreZ );
			SAVE_VAR( Vars::Models::Players::Overlay );
			SAVE_VAR( Vars::Models::Players::OverlayAlpha );
			SAVE_VAR( Vars::Models::Buildings::Enabled );
			SAVE_VAR( Vars::Models::Buildings::Model );
			SAVE_VAR( Vars::Models::Buildings::IgnoreZ );
			SAVE_VAR( Vars::Models::Buildings::Overlay );
			SAVE_VAR( Vars::Models::Buildings::OverlayAlpha );
			SAVE_VAR( Vars::Models::World::Enabled );
			SAVE_VAR( Vars::Models::World::Model );
			SAVE_VAR( Vars::Models::World::IgnoreZ );
			SAVE_VAR( Vars::Models::World::Overlay );
			SAVE_VAR( Vars::Models::World::OverlayAlpha );

			SAVE_VAR( Vars::Models::Main::Enabled );
			SAVE_VAR( Vars::Models::Main::Enemies );
			SAVE_VAR( Vars::Models::Main::ModelOrder );
		}

		//misc
		{
			SAVE_VAR( Vars::Misc::AutoStrafe );
			SAVE_VAR( Vars::Misc::AutoJump );
			SAVE_VAR( Vars::Misc::BreakJump );
			SAVE_VAR( Vars::Misc::ScoutJump );
			SAVE_VAR( Vars::Misc::ScoutJumpKey );
			SAVE_VAR( Vars::Misc::TomislavRev );
			SAVE_VAR( Vars::Misc::TauntSlide );
			SAVE_VAR( Vars::Misc::CheatsBypass );
			SAVE_VAR( Vars::Misc::PureBypass );
			SAVE_VAR( Vars::Misc::DisableInterpolation );
			SAVE_VAR( Vars::Misc::DisableTaunts );
			SAVE_VAR( Vars::Misc::AnimFix );
			SAVE_VAR( Vars::Misc::NoPush );
			SAVE_VAR( Vars::Misc::StacBypass );

			SAVE_VAR( Vars::Misc::TauntSlide );
			SAVE_VAR( Vars::Misc::TauntControl );
			SAVE_VAR( Vars::Misc::TauntSpin );
			SAVE_VAR( Vars::Misc::TauntSpinKey );
			SAVE_VAR( Vars::Misc::TauntSpinSpeed );
			SAVE_VAR( Vars::Misc::TauntFollowsCamera );

			SAVE_VAR( Vars::Misc::Steam::EnableRPC );
			SAVE_VAR( Vars::Misc::Steam::MatchGroup ); // 0 - Special Event; 1 - MvM Mann Up; 2 - Competitive; 3 - Casual; 4 - MvM Boot Camp;
			SAVE_VAR( Vars::Misc::Steam::OverrideMenu ); // Override matchgroup when in main menu
			SAVE_VAR( Vars::Misc::Steam::MapText );
			SAVE_VAR( Vars::Misc::Steam::GroupSize );
			//SAVE_VAR( Vars::Misc::Steam::CustomText );

			SAVE_VAR( Vars::Misc::CL_Move::Enabled );
			SAVE_VAR( Vars::Misc::CL_Move::Doubletap );
			SAVE_VAR( Vars::Misc::CL_Move::AntiWarp );
			SAVE_VAR( Vars::Misc::CL_Move::Indicator );
			SAVE_VAR( Vars::Misc::CL_Move::AutoPeek );
			SAVE_VAR( Vars::Misc::CL_Move::AutoPeekFree );
			SAVE_VAR( Vars::Misc::CL_Move::AutoPeekKey ); //F
			SAVE_VAR( Vars::Misc::CL_Move::TeleportKey ); //F
			SAVE_VAR( Vars::Misc::CL_Move::RechargeKey ); //R
			//SAVE_VAR( Vars::Misc::CL_Move::TeleportMode );
			SAVE_VAR( Vars::Misc::CL_Move::AutoPeek );
			SAVE_VAR( Vars::Misc::CL_Move::AutoPeekKey ); //Z
			SAVE_VAR( Vars::Misc::CL_Move::AutoPeekFree );
			SAVE_VAR( Vars::Misc::CL_Move::DTTicksMoving );
			SAVE_VAR( Vars::Misc::CL_Move::DTTicksStanding );
			SAVE_VAR( Vars::Misc::CL_Move::PassiveRecharge );
			SAVE_VAR( Vars::Misc::CL_Move::TickFrequency );

			SAVE_VAR( Vars::Misc::CL_Move::Fakelag );
			SAVE_VAR( Vars::Misc::CL_Move::FakelagSafe );
			SAVE_VAR( Vars::Misc::CL_Move::FakelagMode ); // 0 - plain, 1 - random, 2 - vel based
			SAVE_VAR( Vars::Misc::CL_Move::FakelagMin ); //	only show when FakelagMode=2
			SAVE_VAR( Vars::Misc::CL_Move::FakelagMax );
			SAVE_VAR( Vars::Misc::CL_Move::FakelagOnKey ); // dont show when fakelagmode=2|3
			SAVE_VAR( Vars::Misc::CL_Move::FakelagKey ); //T
			SAVE_VAR( Vars::Misc::CL_Move::FakelagValue ); // dont show when fakelagmode=2
		}

		//visuals
		{
			SAVE_VAR( Vars::Visuals::ParticleTracer );
			SAVE_VAR( Vars::Visuals::ParticlesTeamColor );
			SAVE_VAR( Vars::Visuals::ViewmodelSway );
			SAVE_VAR( Vars::Visuals::ViewmodelPos );
			SAVE_VAR( Vars::Visuals::ViewmodelFlip );
			SAVE_VAR( Vars::Visuals::ViewmodelFOV );
			SAVE_VAR( Vars::Visuals::ViewX );
			SAVE_VAR( Vars::Visuals::ViewY );
			SAVE_VAR( Vars::Visuals::ViewZ );
			SAVE_VAR( Vars::Visuals::ViewRoll );
			SAVE_VAR( Vars::Visuals::fov );
			SAVE_VAR( Vars::Visuals::AspectRatio );
			SAVE_VAR( Vars::Visuals::ThirdPersonKey );
			SAVE_VAR( Vars::Visuals::ThirdpersonOffset );
			SAVE_VAR( Vars::Visuals::ThirdpersonDist );
			SAVE_VAR( Vars::Visuals::ThirdpersonRight );
			SAVE_VAR( Vars::Visuals::ThirdpersonUp );
			SAVE_VAR( Vars::Visuals::ThirdPersonServerHitbox );
			SAVE_VAR( Vars::Visuals::RemoveScope );
			SAVE_VAR( Vars::Visuals::WorldModulation );

			SAVE_VAR( Vars::Visuals::OutOfFOVArrows );
			SAVE_VAR( Vars::Visuals::ArrowsIgnoreFriends );
			SAVE_VAR( Vars::Visuals::ArrowsEnemyOnly );
			SAVE_VAR( Vars::Visuals::ArrowsDormant );
			SAVE_VAR( Vars::Visuals::ArrowsDistanceAlpha );

			SAVE_VAR( Vars::Visuals::RagdollEffects::Active );
			SAVE_VAR( Vars::Visuals::RagdollEffects::EnemyOnly );
			SAVE_VAR( Vars::Visuals::RagdollEffects::Burning );
			SAVE_VAR( Vars::Visuals::RagdollEffects::Electrocuted );
			SAVE_VAR( Vars::Visuals::RagdollEffects::BecomeAsh );
			SAVE_VAR( Vars::Visuals::RagdollEffects::Dissolve );
			SAVE_VAR( Vars::Visuals::RagdollEffects::RagdollType );
		}

		//menu
		{
			SAVE_VAR( Vars::Menu::DarkMode );
			SAVE_VAR( Vars::Menu::LightMode );
			SAVE_VAR( Vars::Menu::CustomTheme );
		}

		//colors
		{
			SAVE_VAR( Vars::Colors::Scheme );
			SAVE_OTHER( Vars::Colors::LocalPlayer );
			SAVE_OTHER( Vars::Colors::LocalBuilding );
			SAVE_OTHER( Vars::Colors::Enemy );
			SAVE_OTHER( Vars::Colors::Red );
			SAVE_OTHER( Vars::Colors::EnemyBuilding );
			SAVE_OTHER( Vars::Colors::RedBuilding );
			SAVE_OTHER( Vars::Colors::Teammate );
			SAVE_OTHER( Vars::Colors::Blu );
			SAVE_OTHER( Vars::Colors::TeammateBuilding );
			SAVE_OTHER( Vars::Colors::BluBuilding );
			SAVE_OTHER( Vars::Colors::Health );
			SAVE_OTHER( Vars::Colors::Ammo );
			SAVE_OTHER( Vars::Colors::Bomb );
			SAVE_OTHER( Vars::Colors::Other );
			SAVE_OTHER( Vars::Colors::Default );
			SAVE_OTHER( Vars::Colors::WorldModulation );
			SAVE_OTHER( Vars::Colors::StaticPropModulation );
			SAVE_OTHER( Vars::Colors::ParticleModulation );
			SAVE_OTHER( Vars::Colors::ShowHitboxes );
			SAVE_OTHER( Vars::Colors::PredictedPath );
			SAVE_OTHER( Vars::Colors::TargetBBox );
			SAVE_OTHER( Vars::Menu::Colors::CustomTitle );
			SAVE_OTHER( Vars::Menu::Colors::WindowBackground );
			SAVE_OTHER( Vars::Menu::Colors::Outline );
			SAVE_OTHER( Vars::Menu::Colors::OutlineMenu );
			SAVE_OTHER( Vars::Menu::Colors::Widget );
			SAVE_OTHER( Vars::Menu::Colors::WidgetActive );
			SAVE_OTHER( Vars::Menu::Colors::TitleBar );
			SAVE_OTHER( Vars::Menu::Colors::Text );
		}
		
		m_Write.close();
	}
}

void CConfigManager::Load(const wchar_t *name)
{
	m_Read = std::wifstream(m_sConfigPath + L"\\" + name + (L".cfg"));

	if (m_Read.is_open())
	{
		//crithack
		{

			LOAD_VAR( Vars::CritHack::Active );
			LOAD_VAR( Vars::CritHack::Indicators );
			LOAD_VAR( Vars::CritHack::AvoidRandom );
			LOAD_VAR( Vars::CritHack::CritKey );

		}

		//antiaim
		/* {
			LOAD_VAR( Vars::AntiHack::AntiAim::Active );
			LOAD_VAR( Vars::AntiHack::AntiAim::ToggleKey );
			LOAD_VAR( Vars::AntiHack::AntiAim::Pitch ); //0 - None, 1 - Zero, 2 - Up, 3 - Down, 4 - Fake Up, 5 - Fake Down
			LOAD_VAR( Vars::AntiHack::AntiAim::YawReal ); //0 - None, 1 - Forward, 2 - Left, 3 - Right, 4 - Backwards
			LOAD_VAR( Vars::AntiHack::AntiAim::YawFake ); //0 - None, 1 - Forward, 2 - Left, 3 - Right, 4 - Backwards
			LOAD_VAR( Vars::AntiHack::AntiAim::SpinSpeed );
			LOAD_VAR( Vars::AntiHack::AntiAim::CustomRealPitch );
			LOAD_VAR( Vars::AntiHack::AntiAim::RandInterval );
			LOAD_VAR( Vars::AntiHack::AntiAim::AntiBackstab );
			LOAD_VAR( Vars::AntiHack::AntiAim::LegJitter );
			LOAD_VAR( Vars::AntiHack::AntiAim::AntiOverlap );
			LOAD_VAR( Vars::AntiHack::AntiAim::InvalidShootPitch );
		}*/

		{
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::Active );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::Rage );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::ToggleKey );
			//legit
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::LegitYaw );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::LegitYawMode );// 0 - Static,	1 - Edge
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::CycleKey );
			//rage
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeYaw );			// -180 / 180
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeMode );		// 0 - Static,	1 - Edge,	2 - Jitter,	3 - Spin
			//LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeRotate );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeDelta );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeSpin );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeAngle1 );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakeAngle2 );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::CycleFakeKey );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::CycleFakeJitter );

			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealYaw );			// -180 / 180
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealMode );		// 0 - Static,	1 - Edge,	2 - Jitter,	3 - Spin
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealRotate );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealDelta );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealSpin );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealAngle1 );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealAngle2 );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::CycleRealKey );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::CycleRealJitter );

			LOAD_VAR( Vars::AntiHack::AntiAimBeta::Pitch );			// 0 - None,	1 - Automatic,	2 - Manual
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::FakePitch );
			LOAD_VAR( Vars::AntiHack::AntiAimBeta::RealPitch );
		}

		//aimbot
		{
			LOAD_VAR( Vars::Aimbot::Global::Active );
			LOAD_VAR( Vars::Aimbot::Global::AimKey );
			LOAD_VAR( Vars::Aimbot::Global::AutoShoot );
			LOAD_VAR( Vars::Aimbot::Global::AimPlayers );
			LOAD_VAR( Vars::Aimbot::Global::AimBuildings );
			LOAD_VAR( Vars::Aimbot::Global::AimNPCs );
			LOAD_VAR( Vars::Aimbot::Global::AimStickies );
			//LOAD_VAR( Vars::Aimbot::Global::AmmoCheck );
			//LOAD_VAR( Vars::Aimbot::Global::AmmoCount );
			LOAD_VAR( Vars::Aimbot::Global::IgnoreInvulnerable );
			LOAD_VAR( Vars::Aimbot::Global::IgnoreCloaked );
			LOAD_VAR( Vars::Aimbot::Global::IgnoreFriends );
			LOAD_VAR( Vars::Aimbot::Global::IgnoreTaunting );

			//hitscan
			LOAD_VAR( Vars::Aimbot::Hitscan::Active );
			LOAD_VAR( Vars::Aimbot::Hitscan::SortMethod );	//0 - FOV,		1 - Distance
			LOAD_VAR( Vars::Aimbot::Hitscan::AimMethod );	//0 - Normal,	1 - Smooth, 2 - Silent
			LOAD_VAR( Vars::Aimbot::Hitscan::AimHitbox );	//0 - Head,		1 - Body,	2 - Auto
			LOAD_VAR( Vars::Aimbot::Hitscan::AimFOV );
			LOAD_VAR( Vars::Aimbot::Hitscan::SmoothingAmount );
			LOAD_VAR( Vars::Aimbot::Hitscan::TapFire );	//0 - Off, 1 - Distance, 2 - Always
			LOAD_VAR( Vars::Backtrack::Active );
			LOAD_VAR( Vars::Backtrack::Indicator );
			LOAD_VAR( Vars::Aimbot::Hitscan::ScanHitboxes );
			LOAD_VAR( Vars::Aimbot::Hitscan::ScanHead );
			LOAD_VAR( Vars::Aimbot::Hitscan::ScanBuildings );
			LOAD_VAR( Vars::Aimbot::Hitscan::WaitForHeadshot );
			LOAD_VAR( Vars::Aimbot::Hitscan::WaitForCharge );
			LOAD_VAR( Vars::Aimbot::Hitscan::SpectatedSmooth );
			LOAD_VAR( Vars::Aimbot::Hitscan::ScopedOnly );
			LOAD_VAR( Vars::Aimbot::Hitscan::AutoScope );
			LOAD_VAR( Vars::Aimbot::Hitscan::AutoRev );
			LOAD_VAR( Vars::Aimbot::Hitscan::ShowHitboxes );
			LOAD_VAR( Vars::Aimbot::Hitscan::HitboxDuration );

			//projectile
			LOAD_VAR( Vars::Aimbot::Projectile::Active );
			LOAD_VAR( Vars::Aimbot::Projectile::SortMethod );	//0 - FOV,		1 - Distance
			LOAD_VAR( Vars::Aimbot::Projectile::AimMethod );	//0 - Normal,	1 - Silent
			LOAD_VAR( Vars::Aimbot::Projectile::AimPosition );	//0 - Body,		1 - Feet,	2 - Auto
			LOAD_VAR( Vars::Aimbot::Projectile::AimFOV );
			LOAD_VAR( Vars::Aimbot::Projectile::RespectFOV );
			LOAD_VAR( Vars::Aimbot::Projectile::FeetAimIfOnGround );
			LOAD_VAR( Vars::Aimbot::Projectile::HeadAllowed );
			LOAD_VAR( Vars::Aimbot::Projectile::BodyAllowed );
			LOAD_VAR( Vars::Aimbot::Projectile::FeetAllowed );
			LOAD_VAR( Vars::Aimbot::Projectile::ChargeLooseCannon );
			LOAD_VAR( Vars::Aimbot::Projectile::PredictedPath );
			LOAD_VAR( Vars::Aimbot::Projectile::PathDuration );
			LOAD_VAR( Vars::Aimbot::Projectile::TargetBBox );
			LOAD_VAR( Vars::Aimbot::Projectile::BBoxDuration );
		}

		//triggerbot
		{
			LOAD_VAR( Vars::Triggerbot::Active );
			LOAD_VAR( Vars::Triggerbot::Key );
			LOAD_VAR( Vars::Triggerbot::TriggerPlayers );
			LOAD_VAR( Vars::Triggerbot::TriggerBuildings );
			LOAD_VAR( Vars::Triggerbot::HeadOnly );
			LOAD_VAR( Vars::Triggerbot::WaitForHeadshot );
			LOAD_VAR( Vars::Triggerbot::ScopeOnly );
			LOAD_VAR( Vars::Triggerbot::HeadScale );
		}

		//esp
		{
			LOAD_VAR( Vars::ESP::Players::esp_players );
			LOAD_VAR( Vars::ESP::Players::box );
			LOAD_VAR( Vars::ESP::Players::name );
			LOAD_VAR( Vars::ESP::Players::healthbar );
			LOAD_VAR( Vars::ESP::Players::gradient );
			LOAD_VAR( Vars::ESP::Players::lines );
			LOAD_VAR( Vars::ESP::Players::conds );
			LOAD_VAR( Vars::ESP::Players::weapons );
			LOAD_VAR( Vars::ESP::Players::skeleton );
			LOAD_VAR( Vars::ESP::Players::distance );
			LOAD_VAR( Vars::ESP::Buildings::esp_buildings );
			LOAD_VAR( Vars::ESP::Buildings::box );
			LOAD_VAR( Vars::ESP::Buildings::name );
			LOAD_VAR( Vars::ESP::Buildings::healthbar );
			LOAD_VAR( Vars::ESP::Buildings::gradient );
			LOAD_VAR( Vars::ESP::Buildings::lines );
			LOAD_VAR( Vars::ESP::Buildings::conds );
			LOAD_VAR( Vars::ESP::Buildings::distance );
			LOAD_VAR( Vars::ESP::World::esp_world );
			LOAD_VAR( Vars::ESP::World::box );
			LOAD_VAR( Vars::ESP::World::name );
			LOAD_VAR( Vars::ESP::World::distance );
			LOAD_VAR( Vars::ESP::Main::esp_global );
			LOAD_VAR( Vars::ESP::Main::esp_dormant );
			LOAD_VAR( Vars::ESP::Main::esp_cheaptext );
			LOAD_VAR( Vars::ESP::Main::esp_enemies );
		}

		//outlines
		{
			LOAD_VAR( Vars::Outline::Players::Enabled );
			LOAD_VAR( Vars::Outline::Players::AttachmentsOutline );
			LOAD_VAR( Vars::Outline::Players::ViewmodelOutline );
			LOAD_VAR( Vars::Outline::Players::OutlineScale );
			LOAD_VAR( Vars::Outline::Players::OutlineAlpha );
			LOAD_VAR( Vars::Outline::Buildings::Enabled );
			LOAD_VAR( Vars::Outline::Buildings::OutlineScale );
			LOAD_VAR( Vars::Outline::Buildings::OutlineAlpha );
			LOAD_VAR( Vars::Outline::World::Enabled );
			LOAD_VAR( Vars::Outline::World::OutlineScale );
			LOAD_VAR( Vars::Outline::World::OutlineAlpha );
			LOAD_VAR( Vars::Outline::Main::Enabled );
			LOAD_VAR( Vars::Outline::Main::BlurOutline );
			LOAD_VAR( Vars::Outline::Main::WireframeOutline );
			LOAD_VAR( Vars::Outline::Main::CheapOutline );
			LOAD_VAR( Vars::Outline::Main::Enemies );
		}

		//models
		{
			LOAD_VAR( Vars::Models::Players::Enabled );
			LOAD_VAR( Vars::Models::Players::Model );
			LOAD_VAR( Vars::Models::Players::IgnoreZ );
			LOAD_VAR( Vars::Models::Players::Overlay );
			LOAD_VAR( Vars::Models::Players::OverlayAlpha );
			LOAD_VAR( Vars::Models::Buildings::Enabled );
			LOAD_VAR( Vars::Models::Buildings::Model );
			LOAD_VAR( Vars::Models::Buildings::IgnoreZ );
			LOAD_VAR( Vars::Models::Buildings::Overlay );
			LOAD_VAR( Vars::Models::Buildings::OverlayAlpha );
			LOAD_VAR( Vars::Models::World::Enabled );
			LOAD_VAR( Vars::Models::World::Model );
			LOAD_VAR( Vars::Models::World::IgnoreZ );
			LOAD_VAR( Vars::Models::World::Overlay );
			LOAD_VAR( Vars::Models::World::OverlayAlpha );

			LOAD_VAR( Vars::Models::Main::Enabled );
			LOAD_VAR( Vars::Models::Main::Enemies );
			LOAD_VAR( Vars::Models::Main::ModelOrder );
		}

		//misc
		{
			LOAD_VAR( Vars::Misc::AutoStrafe );
			LOAD_VAR( Vars::Misc::AutoJump );
			LOAD_VAR( Vars::Misc::BreakJump );
			LOAD_VAR( Vars::Misc::ScoutJump );
			LOAD_VAR( Vars::Misc::ScoutJumpKey );
			LOAD_VAR( Vars::Misc::TomislavRev );
			LOAD_VAR( Vars::Misc::TauntSlide );
			LOAD_VAR( Vars::Misc::CheatsBypass );
			LOAD_VAR( Vars::Misc::PureBypass );
			LOAD_VAR( Vars::Misc::DisableInterpolation );
			LOAD_VAR( Vars::Misc::DisableTaunts );
			LOAD_VAR( Vars::Misc::AnimFix );
			LOAD_VAR( Vars::Misc::NoPush );
			LOAD_VAR( Vars::Misc::StacBypass );

			LOAD_VAR( Vars::Misc::TauntSlide );
			LOAD_VAR( Vars::Misc::TauntControl );
			LOAD_VAR( Vars::Misc::TauntSpin );
			LOAD_VAR( Vars::Misc::TauntSpinKey );
			LOAD_VAR( Vars::Misc::TauntSpinSpeed );
			LOAD_VAR( Vars::Misc::TauntFollowsCamera );

			LOAD_VAR( Vars::Misc::Steam::EnableRPC );
			LOAD_VAR( Vars::Misc::Steam::MatchGroup );		// 0 - Special Event; 1 - MvM Mann Up; 2 - Competitive; 3 - Casual; 4 - MvM Boot Camp;
			LOAD_VAR( Vars::Misc::Steam::OverrideMenu );	// Override matchgroup when in main menu
			LOAD_VAR( Vars::Misc::Steam::MapText );
			LOAD_VAR( Vars::Misc::Steam::GroupSize );

			LOAD_VAR( Vars::Misc::CL_Move::Enabled );
			LOAD_VAR( Vars::Misc::CL_Move::Doubletap );
			LOAD_VAR( Vars::Misc::CL_Move::AntiWarp );
			LOAD_VAR( Vars::Misc::CL_Move::Indicator );
			LOAD_VAR( Vars::Misc::CL_Move::AutoPeek );
			LOAD_VAR( Vars::Misc::CL_Move::AutoPeekFree );
			LOAD_VAR( Vars::Misc::CL_Move::AutoPeekKey ); //F
			LOAD_VAR( Vars::Misc::CL_Move::TeleportKey ); //F
			LOAD_VAR( Vars::Misc::CL_Move::RechargeKey ); //R
			//LOAD_VAR( Vars::Misc::CL_Move::TeleportMode );
			LOAD_VAR( Vars::Misc::CL_Move::AutoPeek );
			LOAD_VAR( Vars::Misc::CL_Move::AutoPeekKey ); //Z
			LOAD_VAR( Vars::Misc::CL_Move::AutoPeekFree );
			LOAD_VAR( Vars::Misc::CL_Move::DTTicksMoving );
			LOAD_VAR( Vars::Misc::CL_Move::DTTicksStanding );
			LOAD_VAR( Vars::Misc::CL_Move::PassiveRecharge );
			LOAD_VAR( Vars::Misc::CL_Move::TickFrequency );

			LOAD_VAR( Vars::Misc::CL_Move::Fakelag );
			LOAD_VAR( Vars::Misc::CL_Move::FakelagSafe );
			LOAD_VAR( Vars::Misc::CL_Move::FakelagMode ); // 0 - plain, 1 - random, 2 - vel based
			LOAD_VAR( Vars::Misc::CL_Move::FakelagMin ); //	only show when FakelagMode=2
			LOAD_VAR( Vars::Misc::CL_Move::FakelagMax );
			LOAD_VAR( Vars::Misc::CL_Move::FakelagOnKey ); // dont show when fakelagmode=2|3
			LOAD_VAR( Vars::Misc::CL_Move::FakelagKey ); //T
			LOAD_VAR( Vars::Misc::CL_Move::FakelagValue ); // dont show when fakelagmode=2
		}

		//visuals
		{
			LOAD_VAR( Vars::Visuals::ParticleTracer );
			LOAD_VAR( Vars::Visuals::ParticlesTeamColor );
			LOAD_VAR( Vars::Visuals::ViewmodelSway );
			LOAD_VAR( Vars::Visuals::ViewmodelPos );
			LOAD_VAR( Vars::Visuals::ViewmodelFlip );
			LOAD_VAR( Vars::Visuals::ViewmodelFOV );
			LOAD_VAR( Vars::Visuals::ViewX );
			LOAD_VAR( Vars::Visuals::ViewY );
			LOAD_VAR( Vars::Visuals::ViewZ );
			LOAD_VAR( Vars::Visuals::ViewRoll );
			LOAD_VAR( Vars::Visuals::fov );
			LOAD_VAR( Vars::Visuals::AspectRatio );
			LOAD_VAR( Vars::Visuals::ThirdPersonKey );
			LOAD_VAR( Vars::Visuals::ThirdpersonDist );
			LOAD_VAR( Vars::Visuals::ThirdpersonRight );
			LOAD_VAR( Vars::Visuals::ThirdpersonUp );
			LOAD_VAR( Vars::Visuals::ThirdPersonServerHitbox );
			LOAD_VAR( Vars::Visuals::RemoveScope );
			LOAD_VAR( Vars::Visuals::WorldModulation );

			LOAD_VAR( Vars::Visuals::OutOfFOVArrows );
			LOAD_VAR( Vars::Visuals::ArrowsIgnoreFriends );
			LOAD_VAR( Vars::Visuals::ArrowsEnemyOnly );
			LOAD_VAR( Vars::Visuals::ArrowsDormant );
			LOAD_VAR( Vars::Visuals::ArrowsDistanceAlpha );

			LOAD_VAR( Vars::Visuals::RagdollEffects::Active );
			LOAD_VAR( Vars::Visuals::RagdollEffects::EnemyOnly );
			LOAD_VAR( Vars::Visuals::RagdollEffects::Burning );
			LOAD_VAR( Vars::Visuals::RagdollEffects::Electrocuted );
			LOAD_VAR( Vars::Visuals::RagdollEffects::BecomeAsh );
			LOAD_VAR( Vars::Visuals::RagdollEffects::Dissolve );
			LOAD_VAR( Vars::Visuals::RagdollEffects::RagdollType );
		}

		//menu
		{
			LOAD_VAR( Vars::Menu::DarkMode );
			LOAD_VAR( Vars::Menu::LightMode );
			LOAD_VAR( Vars::Menu::CustomTheme );
		}

		//colors
		{
			LOAD_VAR( Vars::Colors::Scheme );
			LOAD_OTHER( Vars::Colors::LocalPlayer );
			LOAD_OTHER( Vars::Colors::LocalBuilding );
			LOAD_OTHER( Vars::Colors::Enemy );
			LOAD_OTHER( Vars::Colors::Red );
			LOAD_OTHER( Vars::Colors::EnemyBuilding );
			LOAD_OTHER( Vars::Colors::RedBuilding );
			LOAD_OTHER( Vars::Colors::Teammate );
			LOAD_OTHER( Vars::Colors::Blu );
			LOAD_OTHER( Vars::Colors::TeammateBuilding );
			LOAD_OTHER( Vars::Colors::BluBuilding );
			LOAD_OTHER( Vars::Colors::Health );
			LOAD_OTHER( Vars::Colors::Ammo );
			LOAD_OTHER( Vars::Colors::Bomb );
			LOAD_OTHER( Vars::Colors::Other );
			LOAD_OTHER( Vars::Colors::Default );
			LOAD_OTHER( Vars::Colors::WorldModulation);
			LOAD_OTHER( Vars::Colors::StaticPropModulation);
			LOAD_OTHER( Vars::Colors::ParticleModulation);
			LOAD_OTHER( Vars::Colors::ShowHitboxes );
			LOAD_OTHER( Vars::Colors::PredictedPath );
			LOAD_OTHER( Vars::Colors::TargetBBox );
			LOAD_OTHER( Vars::Menu::Colors::CustomTitle );
			LOAD_OTHER( Vars::Menu::Colors::WindowBackground );
			LOAD_OTHER( Vars::Menu::Colors::Outline );
			LOAD_OTHER( Vars::Menu::Colors::OutlineMenu );
			LOAD_OTHER( Vars::Menu::Colors::Widget );
			LOAD_OTHER( Vars::Menu::Colors::WidgetActive );
			LOAD_OTHER( Vars::Menu::Colors::TitleBar );
			LOAD_OTHER( Vars::Menu::Colors::Text );
		}
		
		m_Read.close();
	}
}

void CConfigManager::Remove(const wchar_t *name)
{
	std::filesystem::remove(m_sConfigPath + L"\\" + name + (L".cfg"));
}