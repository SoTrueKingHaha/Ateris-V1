#pragma once
#include "../SDK/SDK.h"

template <class T>
class CVar
{
public:
	T var;
	const wchar_t* szName;
};

namespace Vars
{
	namespace Menu
	{
		inline Rect_t Position = { 100, 100, 620 - 112, 360 + 42 + 35 + 88 + 66 };
		inline const auto Title = (L"Team Fortress 2");
		inline int TitleBarH = Position.h + 21;
		inline CVar<int> CurrentCfg = { 0, L"Current Config"};

		inline CVar<bool> LightMode = { false, L"Light Mode" };
		inline CVar<bool> DarkMode = { true, L"Dark Mode" };
		inline CVar<bool> CustomMode = { true, L"Custom Color"};
		inline CVar<bool> CustomTheme = { true, L"Custom Theme"};

		inline int SpacingX = 8;
		inline int SpacingY = 4;
		inline int SpacingText = 4;

		inline int CheckBoxW = 14;
		inline int CheckBoxH = 14;
		inline int CheckBoxFillSize = 3;

		inline int ButtonW = 90;
		inline int ButtonH = 20;

		inline int ButtonWSmall = 70;
		inline int ButtonHSmall = 16;

		inline int ComboBoxW = 90;
		inline int ComboBoxH = 14;

		inline int InputBoxW = 90;
		inline int InputBoxH = 14;
		inline int InputColorBoxW = 30;

		namespace Colors
		{
			inline Color_t WindowBackground = { 24, 24, 24, 255 };
			inline Color_t Outline = { 0, 0, 0, 255 };
			inline Color_t TitleBar = { 120, 255, 90, 255 };
			inline Color_t CustomTitle = { 147, 112, 219, 255 };
			inline Color_t Text = { 255, 255, 255, 255 };
			inline Color_t Widget = { 20, 20, 20, 255 };
			inline Color_t WidgetActive = { 70, 70, 70, 255 };
			inline Color_t OutlineMenu = { 83, 83, 83, 255 };
		}
	}

	namespace Playerlist
	{
		inline Rect_t Position = { 900, 100, 620 + 112, 352 };
		inline int TitleBarH = Position.h + 21;
	}

	namespace CritHack
	{
		inline CVar<bool> Active{ false, L"Crits Active" };
		inline CVar<bool> Indicators{ false, L"Crits Indicator" };
		inline CVar<bool> AvoidRandom{ false, L"Avoid Random" };
		inline CVar<int> CritKey{ 0x52, L"Crit Key" };
	}

	namespace AntiHack
	{
		namespace AntiAim
		{
			inline CVar<bool> Active{ false, L"Active" };
			inline CVar<int> ToggleKey{ 0, L"ToggleKey" };
			inline CVar<int> Pitch{ 0, L"Pitch" }; //0 - None, 1 - Zero, 2 - Up, 3 - Down, 4 - Fake Up, 5 - Fake Down
			inline CVar<int> YawReal{ 0, L"YawReal" }; //0 - None, 1 - Forward, 2 - Left, 3 - Right, 4 - Backwards
			inline CVar<int> YawFake{ 0, L"YawFake" }; //0 - None, 1 - Forward, 2 - Left, 3 - Right, 4 - Backwards
			inline CVar<float> SpinSpeed{ 15.f, L"SpinSpeed" };
			inline CVar<float> CustomRealPitch{ 0.f, L"CustomRealPitch" };
			inline CVar<int> RandInterval{ 25, L"RandInterval" };
			inline CVar<bool> AntiBackstab{ false, L"AntiBackstab" };
			inline CVar<bool> LegJitter{ false, L"LegJitter" };
			inline CVar<bool> AntiOverlap{ false, L"AntiOverlap" };
			inline CVar<bool> InvalidShootPitch{ false, L"InvalidShootPitch" };
		}

		namespace AntiAimBeta
		{
			inline CVar<bool> Active{ false, L"Active" };
			inline CVar<bool> Rage{ false, L"Rage Antiaim" };
			inline CVar<int> ToggleKey{ 0, L"Toggle Key" };
			//legit
			inline CVar<int> LegitYaw{ 0, L"Desync Real Yaw" };
			inline CVar<int> LegitYawMode{ 0, L"Real Yaw Mode" };// 0 - Static,	1 - Edge
			inline CVar<int> CycleKey{ 0, L"Cycle Key" };
			//rage
			inline CVar<int> FakeYaw{ 0, L"Fake Yaw" };			// -180 / 180
			inline CVar<int> FakeMode{ 0, L"Fake Mode" };		// 0 - Static,	1 - Edge,	2 - Jitter,	3 - Spin
			inline CVar<int> FakeRotate{ 3, L"Fake Rotate Frequency" };
			inline CVar<int> FakeDelta{ 10, L"Fake Jitter Delta" };
			inline CVar<int> FakeSpin{ 3, L"Fake Spin Factor" };
			inline CVar<int> FakeAngle1{ 74, L"Rotate Fake Angle #1" };
			inline CVar<int> FakeAngle2{ 104, L"Rotate Fake Angle #2" };
			inline CVar<int> CycleFakeKey{ 0, L"Cycle Fake Key" };
			inline CVar<bool> CycleFakeJitter{ false, L"Cycle Fake Jitter" };

			inline CVar<int> RealYaw{ 0, L"Real Yaw" };			// -180 / 180
			inline CVar<int> RealMode{ 0, L"Real Mode" };		// 0 - Static,	1 - Edge,	2 - Jitter,	3 - Spin
			inline CVar<int> RealRotate{ 3, L"Real Rotate Frequency" };
			inline CVar<int> RealDelta{ 10, L"Real Jitter Delta" };
			inline CVar<int> RealSpin{ 3, L"Real Spin Factor" };
			inline CVar<int> RealAngle1{ 74, L"Rotate Real Angle #1" };
			inline CVar<int> RealAngle2{ 104, L"Rotate Real Angle #2" };
			inline CVar<int> CycleRealKey{ 0, L"Cycle Real Key" };
			inline CVar<bool> CycleRealJitter{ false, L"Cycle Real Jitter" };

			inline CVar<int> Pitch{ 0, L"Pitch Mode" };			// 0 - None,	1 - Automatic,	2 - Manual
			inline CVar<bool> FakePitch{ false, L"Fake Pitch" };
			inline CVar<int> RealPitch{ 0, L"Real Pitch" };		// 0 - Up,		1 - Down,	2 - Jitter
		}
	}

	namespace Backtrack
	{
		inline CVar<bool> Active{ false, L"Player History" };
		inline CVar<bool> Indicator{ false, L"Visualize History" };
	}

	namespace Aimbot
	{
		namespace Global
		{
			inline CVar<bool>		Active = { false, L"Active" };
			inline CVar<int>		AimKey = { VK_XBUTTON1, L"Aimbot Key" };
			inline CVar<bool>		AutoShoot = { false, L"Auto Shoot" };
			inline CVar<bool>		AimPlayers = { false, L"Aim at Players" };
			inline CVar<bool>		AimBuildings = { false, L"Aim at Buildings" };
			inline CVar<bool>		AimNPCs = { false, L"Aim at NPCs" };
			inline CVar<bool>		AimStickies = { false, L"Aim at Stickies" };
			inline CVar<bool>		AmmoCheck = { false, L"Ammo Check" };
			inline CVar<int>		AmmoCount = { 25, L"Ammo Count" };
			inline CVar<bool>		IgnoreInvulnerable = { false, L"Ignore Invulnerable" };
			inline CVar<bool>		IgnoreCloaked = { false, L"Ignore Cloaked" };
			inline CVar<bool>		IgnoreFriends = { false, L"Ignore Friends" };
			inline CVar<bool>		IgnoreTaunting = { false, L"Ignore Taunting" };
		}

		namespace Hitscan
		{
			inline CVar<bool>		Active = { false, L"Active" };
			inline CVar<int>		SortMethod = { 0, L"Sort Method" };	//0 - FOV,		1 - Distance
			inline CVar<int> BacktrackMethod = { 0, L"Backtrack Method" };//0 - All,	1 - First,	2 - Last,	3 - Adaptive,	4 - Force OnShot
			inline CVar<int>		AimMethod = { 2, L"Aim Method" };	//0 - Normal,	1 - Smooth, 2 - Silent
			inline CVar<int>		AimHitbox = { 2, L"Aim Hitbox" };	//0 - Head,		1 - Body,	2 - Auto
			inline CVar<float>		AimFOV = { 15.0f, L"FOV" };
			inline CVar<float>		SmoothingAmount = { 4.0f, L"Smoothing" };
			inline CVar<int>		TapFire = { 1, L"Tap Fire" };		//0 - Off,		1 - Distance, 2 - Always
			inline CVar<bool>		ScanHitboxes = { false, L"Player Multi-Point" };
			inline CVar<bool>		ScanHead = { false, L"Head Multi-Point" };
			inline CVar<bool>		ScanBuildings = { false, L"Buildings Multi-Point" };
			inline CVar<bool>		WaitForHeadshot = { false, L"Wait for Headshot" };
			inline CVar<bool>		WaitForCharge = { false, L"Wait for Charge" };
			inline CVar<bool>		SpectatedSmooth = { false, L"Smooth when Spectated" };
			inline CVar<bool>		ScopedOnly = { false, L"Scoped Only" };
			inline CVar<bool>		AutoScope = { false, L"Auto Scope" };
			inline CVar<bool>		AutoRev = { false, L"Auto Rev" };
			inline CVar<bool>		ShowHitboxes = { false, L"Show Hitboxes" };
			inline CVar<float>		HitboxDuration = { 2.5f, L"Hitbox Duration" };
		}

		namespace Projectile
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<int> SortMethod{ 0, L"Sort Method" };	//0 - FOV,		1 - Distance
			inline CVar<int> AimMethod{ 1, L"Aim Method" };	//0 - Normal,	1 - Silent
			inline CVar<int> AimPosition{ 3, L"Aim Position" };	//0 - Head,		1 - Body,		2 - Feet,	3 - Auto
			inline CVar<float> AimFOV{ 25.0f, L"Aim FOV" };
			inline CVar<bool> RespectFOV{ true, L"Respect FOV" };
			//inline CVar<int> VisTestPoints{ 15, L"VisTestPoints" }; //how many points are we allowed to vis test before we stop scanning.
			//inline CVar<int> ScanPoints{ 15, L"ScanPoints" }; //how many "visible points" need to be reached before we stop searching.
			//inline CVar<float> ScanScale{ 0.9f, L"Point Scale" }; // how to scale the points.
			inline CVar<bool> FeetAimIfOnGround{ true, L"Feet On Ground" };
			inline CVar<bool> HeadAllowed{ false, L"Head Hitbox" };
			inline CVar<bool> BodyAllowed{ true, L"Body Hitbox" };
			inline CVar<bool> FeetAllowed{ true, L"Feet Hitbox" };
			//inline CVar<float> PredictionTime{ 2.0f, L"PredictionTime" };
			inline CVar<bool> ChargeLooseCannon{ false, L"Charge Loose Cannon" };
			inline CVar<bool> PredictedPath = { false, L"Show Predicted Path" };
			inline CVar<float> PathDuration = { 5.f, L"Path Duration" };
			inline CVar<bool> TargetBBox = { false, L"Show BBox On Hit" };
			inline CVar<float> BBoxDuration = { 2.f, L"BBox Duration" };
			inline CVar<bool> StrafePredictionGround = { false, L"Strafe Pred Ground" };
			inline CVar<bool> StrafePredictionAir = { true, L"Strafe Pred In Air" };
		}
	}

	namespace Triggerbot
	{
		inline CVar<bool> Active = { false, L"Triggerbot Active" };
		inline CVar<int> Key = { VK_XBUTTON1, L"Triggerbot Key" };
		inline CVar<bool> TriggerPlayers = { false, L"Shoot Players" };
		inline CVar<bool> TriggerBuildings = { false, L"Shoot Buildings" };
		inline CVar<bool> HeadOnly = { false, L"Head Only" };
		inline CVar<bool> WaitForHeadshot = { false, L"Wait for Headshot" };
		inline CVar<bool> ScopeOnly = { false, L"Scoped Only" };
		inline CVar<float> HeadScale = { 0.7f, L"Head Scale" };
	}

	namespace ESP
	{
		namespace Main
		{
			inline CVar<bool> esp_global = { true, L"ESP Global"};
			inline CVar<bool> esp_dormant = { true, L"Dormant ESP"};
			inline CVar<bool> esp_enemies = { false, L"Enemy Only" };
			inline CVar<bool> esp_cheaptext = { false, L"Cheap Text Rendering" };
		}

		namespace Players
		{
			inline CVar<bool> esp_players = { true, L"ESP Players"};
			inline CVar<bool> name = { true, L"Name" };
			inline CVar<bool> healthbar = { true, L"Health Bar" };
			inline CVar<bool> gradient = { true, L"Gradient Color" };
			inline CVar<int> lines = { 0, L"Health Bar Lines" };
			inline CVar<bool> conds = { false, L"Conditions" };
			inline CVar<bool> skeleton = { false, L"Skeleton" };
			inline CVar<bool> weapons = { true, L"Weapons" };
			inline CVar<bool> box = { true, L"Box" };
			inline CVar<bool> distance = { true, L"Distance" };
		}

		namespace Buildings
		{
			inline CVar<bool> esp_buildings = { true, L"ESP Buildings" };
			inline CVar<bool> name = { true, L"Name" };
			inline CVar<bool> healthbar = { true, L"Health Bar" };
			inline CVar<bool> gradient = { true, L"Gradient Color" };
			inline CVar<int> lines = { 0, L"Health Bar Lines" };
			inline CVar<bool> conds = { false, L"Conditions" };
			inline CVar<bool> box = { true, L"Box" };
			inline CVar<bool> distance = { true, L"Distance" };
		}

		namespace World
		{
			inline CVar<bool> esp_world = { true, L"ESP World" };
			inline CVar<bool> name = { true, L"Name" };
			inline CVar<bool> box = { true, L"Box" };
			inline CVar<bool> distance = { true, L"Distance" };
		}
	}

	namespace Outline
	{
		namespace Main
		{
			inline CVar<bool> Enabled = { true, L"Custom Outlines" };
			inline CVar<bool> Enemies = { true, L"Enemy Only" };
			inline CVar<bool> BlurOutline = { true, L"Blur Outline" };
			inline CVar<bool> WireframeOutline = { true, L"Wireframe Outline" };
			inline CVar<bool> CheapOutline = { true, L"Cheap Outline" };
		}

		namespace Players
		{
			inline CVar<bool> Enabled = { true, L"Players Outline" };
			inline CVar<bool> ViewmodelOutline = { true, L"Viewmodel Outline" };
			inline CVar<bool> AttachmentsOutline = { true, L"Attachments Outline" };
			inline CVar<int> OutlineScale = { 2, L"Outline Scale" };
			inline CVar<float> OutlineAlpha = { 0.45f, L"Outline Alpha" };
		}

		namespace Buildings
		{
			inline CVar<bool> Enabled = { true, L"Buildings Outline" };
			inline CVar<int> OutlineScale = { 2, L"Outline Scale" };
			inline CVar<float> OutlineAlpha = { 0.45f, L"Outline Alpha" };
		}

		namespace World
		{
			inline CVar<bool> Enabled = { true, L"World Outline" };
			inline CVar<int> OutlineScale = { 2, L"Outline Scale" };
			inline CVar<float> OutlineAlpha = { 0.45f, L"Outline Alpha" };
		}
	}

	namespace Models
	{
		enum Mats
		{
			NONE = 0,
			FLAT,
			SHADED,
			FRESNEL
		};

		namespace Main
		{
			inline CVar<bool> Enabled = { true, L"Custom Models" };
			inline CVar<bool> Enemies = { true, L"Enemy Only" };
			inline CVar<int> ModelOrder = { 1, L"Drawing Order" };
		}

		namespace Players
		{
			inline CVar<bool> Enabled = { true, L"Player Models" };
			inline CVar<int> Model = { 0, L"Model Type" };
			inline CVar<bool> IgnoreZ = { true, L"Through Walls" };
			inline CVar<bool> Overlay = { true, L"Overlay Model" };
			inline CVar<float> OverlayAlpha = { 1.0f, L"Overlay Alpha" };
		}

		namespace Buildings
		{
			inline CVar<bool> Enabled = { true, L"Building Models" };
			inline CVar<int> Model = { 0, L"Model Type" };
			inline CVar<bool> IgnoreZ = { true, L"Through Walls" };
			inline CVar<bool> Overlay = { true, L"Overlay" };
			inline CVar<float> OverlayAlpha = { 1.0f, L"Overlay Alpha" };
		}

		namespace World
		{
			inline CVar<bool> Enabled = { true, L"World Models" };
			inline CVar<int> Model = { 0, L"Model Type" };
			inline CVar<bool> IgnoreZ = { true, L"Through Walls" };
			inline CVar<bool> Overlay = { true, L"Overlay" };
			inline CVar<float> OverlayAlpha = { 1.0f, L"Overlay Alpha" };
		}
	}

	namespace Misc
	{
		inline CVar<bool> AutoJump = { true, L"Auto Jump" };
		inline CVar<bool> BreakJump = { true, L"Break Jump" };
		inline CVar<bool> ScoutJump = { true, L"Auto FaN Jump" };
		inline CVar<int> ScoutJumpKey = { VK_XBUTTON2, L"FaN Jump Key" };
		inline CVar<bool> AccurateMovement = { true, L"Fast Stop" };
		inline CVar<bool> TomislavRev = { true, L"Tomislav Rev" };
		inline CVar<int> AutoStrafe = { 2, L"Auto Strafe" };
		inline CVar<bool> CheatsBypass = { true, L"sv_cheats bypass" };
		inline CVar<bool> PureBypass = { true, L"sv_pure bypass" };
		inline CVar<bool> DisableInterpolation = { false, L"Disable Interp" };
		inline CVar<bool> DisableTaunts = { false, L"Disable Taunts" };
		inline CVar<bool> AnimFix = { false, L"Anim Fix" };
		inline CVar<bool> NoPush = { true, L"No Push" };

		inline CVar<bool> StacBypass = { false, L"StAC Bypass" };

		inline CVar<bool> TauntSlide{ false, L"Taunt Slide" };
		inline CVar<bool> TauntControl{ false, L"Taunt Control" };
		inline CVar<bool> TauntSpin{ false, L"Taunt Spin" };
		inline CVar<int> TauntSpinKey{ 0, L"Taunt SpinKey" };
		inline CVar<float> TauntSpinSpeed{ 5.f, L"Spin Speed" };
		inline CVar<bool> TauntFollowsCamera{ false, L"Follow Camera" };

		namespace Steam
		{
			inline CVar<bool> EnableRPC{ false, L"Enable" };
			inline CVar<int> MatchGroup{ 0, L"Match Group" }; // 0 - Special Event; 1 - MvM Mann Up; 2 - Competitive; 3 - Casual; 4 - MvM Boot Camp;
			inline CVar<bool> OverrideMenu{ false, L"Main Menu" }; // Override matchgroup when in main menu
			inline CVar<int> MapText{ 1 , L"Map Text" }; // 0 - Fedoraware; 1 - CAM.club; 2 - Meowhook.club; 3 - rathook.cc; 4 - NNitro.tf; 5 - custom;
			inline CVar<int> GroupSize{ 1337 , L"Group Size" };
			inline CVar<std::string> CustomText = { "i hate niggers", L"Custom Text" };
		}

		namespace Regions
		{
			inline CVar<bool> RegionSelector{ false, L"Region Selector" };

			inline CVar<bool> EUEast{ false, L"EU East" };				//EU East		- Frankfurt, Stockholm, Warsaw, Vienna
			inline CVar<bool> EUWest{ false, L"EU West" };				//EU West		- Amsterdam, London, Luxembourg, Madrid, Paris
			inline CVar<bool> Asia{ false, L"Asia" };					//Asia			- Manila, Singapore
			inline CVar<bool> MiddleEast{ false, L"Middle East" };		//Middle East	- Dubai
			inline CVar<bool> HongKong{ false, L"Hong Kong" };			//Hong Kong		- Hong Kong
			inline CVar<bool> Japan{ false, L"Japan" };					//Japan			- Tokyo (GNRT)
			inline CVar<bool> India{ false, L"India" };					//India			- Mumbai, Chennai
			inline CVar<bool> Australia{ false, L"Australia" };			//Australia		- Sydney
			inline CVar<bool> Africa{ false, L"Africa" };				//Africa		- Johannesburg
			inline CVar<bool> Chile{ false, L"Chile" };					//Chile			- Santiago
			inline CVar<bool> Peru{ false, L"Peru" };					//Peru			- Lima
			inline CVar<bool> Brazil{ false, L"Brazil" };				//Brazil		- Sao Paulo
			inline CVar<bool> USEast{ false, L"US East" };				//US East		- Atlanta, Virginia, Chicago
			inline CVar<bool> USNorthWest{ false, L"US North West" };	//US North West - Moses Lake, Washington, Seattle
			inline CVar<bool> USSouthWest{ false, L"US South West" };	//US South West - Los Angeles, Oklahoma City
		}

		namespace CL_Move
		{
			inline CVar<bool> Enabled{ true, L"Enabled" };
			inline CVar<bool> Doubletap{ true, L"Double Tap" };
			inline CVar<bool> AntiWarp{ true, L"Anti Warp" };
			inline CVar<int> Indicator{ 0, L"DT Indicator" }; //0 - None,	1 - CAM.club,	2 - deadflag,	3 - Harpoon
			inline CVar<int> DTTicksStanding{ 22, L"Ticks Standing" };
			inline CVar<int> DTTicksMoving{ 20, L"Ticks Moving" };
			//inline CVar<int> TeleportMode{ 0, L"Teleport Mode" }; //0 - Plain,	1 - Smooth
			inline CVar<int> RechargeKey{ 0x51, L"Recharge Key" }; //Q
			inline CVar<int> TeleportKey{ 0x5A, L"Boost Key" }; //Z
			inline CVar<bool> AutoPeek{ false, L"Auto-Peek" };
			inline CVar<int> AutoPeekKey{ VK_XBUTTON2, L"Auto-Peek Key" }; //Z
			inline CVar<bool> AutoPeekFree{ false, L"Return Only" };
			inline CVar<bool> PassiveRecharge{ false, L"Passive Regen" };
			inline CVar<int> TickFrequency{ 22, L"Tick Frequency" };

			inline CVar<bool> Fakelag{ false, L"Fakelag" };
			inline CVar<bool> FakelagSafe{ false, L"Safe Amount" };
			inline CVar<int> FakelagMode{ 0, L"Lag Mode" }; // 0 - plain, 1 - random, 2 - vel based
			inline CVar<int> FakelagMin{ 1, L"Min Amount" }; //	only show when FakelagMode=2
			inline CVar<int> FakelagMax{ 22, L"Max Amount" };
			inline CVar<bool> FakelagOnKey{ false, L"On-Key" }; // dont show when fakelagmode=2|3
			inline CVar<int> FakelagKey{ 0x54, L"Fakelag Key" }; //T
			inline CVar<int> FakelagValue{ 1, L"Lag Amount" }; // dont show when fakelagmode=2
		}
	}

	namespace Visuals
	{
		inline CVar<int> ParticleTracer = { 2, L"Particle Tracer" };
		inline CVar<bool> ViewmodelSway = { false, L"Viewmodel Sway" };
		inline CVar<bool> ViewmodelPos = { false, L"Viewmodel Position" };
		inline CVar<bool> ViewmodelFlip = { false, L"Viewmodel Flip" };
		inline CVar<int> ViewmodelFOV = { 0, L"Viewmodel FOV" };
		inline CVar<int> ViewX = { 0, L"X Pos" };
		inline CVar<int> ViewY = { 0, L"Y Pos" };
		inline CVar<int> ViewZ = { 0, L"Z Pos" };
		inline CVar<int> ViewRoll = { 0, L"Roll" };
		inline CVar<bool> ThirdPersonServerHitbox = { false, L"Server Hitbox" };
		inline CVar<int> ThirdPersonKey = { VK_MBUTTON, L"ThirdPerson Key" };
		inline CVar<bool> ThirdpersonOffset{ false, L"Camera Offset" };
		inline CVar<float> ThirdpersonDist{ 150.f, L"Forward Offset" };
		inline CVar<float> ThirdpersonRight{ 0.f, L"Sideways Offset" };
		inline CVar<float> ThirdpersonUp{ 0.f, L"Upward Offset" };
		inline CVar<int>	fov = { 90, L"FOV Override" };
		inline CVar<float>	AspectRatio = { 0.f, L"Aspect Ratio" };
		inline CVar<bool> RemoveScope = { false, L"Remove Scope" };
		inline CVar<bool> WorldModulation{ false, L"World Modulation" };
		inline CVar<bool> ParticlesTeamColor{ false, L"Team Colored" };
		inline CVar<bool> BulletTracer{ false, L"Bullet Tracer" };
		inline CVar<bool> Debug{ false, L"Debug Indicators" };
		inline CVar<bool> OutOfFOVArrows{ false, L"OOF Arrows" };
		inline CVar<bool> ArrowsIgnoreFriends{ false, L"Ignore Friends" };
		inline CVar<bool> ArrowsEnemyOnly{ false, L"Enemy Only" };
		inline CVar<bool> ArrowsDormant{ false, L"Dormant Players" };
		inline CVar<bool> ArrowsDistanceAlpha{ false, L"Distance Alpha" };
		/*inline CVar<float> ArrowLength{ 21.f, L"Length" };
		inline CVar<float> ArrowAngle{ 100.f, L"Angle" };
		inline CVar<float> FovArrowsDist{ 0.15f, L"Screen Distance" };
		inline CVar<float> MaxDist{ 1000.f, L"Max Distance" };
		inline CVar<float> MinDist{ 200.f, L"Min Distance" };*/

		namespace RagdollEffects
		{
			inline CVar<bool> Active{ false, L"Active" };
			inline CVar<bool> EnemyOnly{ false, L"Enemy Only" };
			inline CVar<bool> Burning{ false, L"Burning" };
			inline CVar<bool> Electrocuted{ false, L"Electrocuted" };
			inline CVar<bool> BecomeAsh{ false, L"Become Ash" };
			inline CVar<bool> Dissolve{ false, L"Dissolve" };
			inline CVar<int> RagdollType{ 0, L"Ragdoll Type" };
		}

		namespace Skins
		{
			inline CVar<bool> Enabled{ false, L"Enabled" };
			inline CVar<int> Sheen{ 0 , L"Sheen" };
			inline CVar<int> Effect{ 0 , L"Effect" };
			inline CVar<int> Particle{ 0 , L"Particle" };
			inline CVar<bool> Acient{ false , L"Ancient" };
			inline CVar<bool> Override{ false , L"Australium" };
			inline CVar<bool> Australium{ false, L"Australium" };
		}
	}

	namespace Colors
	{
		//0 - Enemy/Team,	1 - Red/Blu
		inline CVar<int> Scheme = { 0, L"Color Scheme" };
		inline Color_t LocalPlayer = { 130, 200, 40, 255 };
		inline Color_t LocalBuilding = { 130, 200, 40, 255 };
		inline Color_t Enemy = { 240, 180, 60, 255 };//{ 255, 107, 107, 255 };
		inline Color_t EnemyBuilding = { 240, 180, 60, 255 };
		inline Color_t Red = { 255, 107, 107, 255 };
		inline Color_t RedBuilding = { 255, 107, 107, 255 };
		inline Color_t Teammate = { 120, 240, 120, 255 };//{ 72, 219, 251, 255 };
		inline Color_t TeammateBuilding = { 120, 240, 120, 255 };
		inline Color_t Blu = { 72, 219, 251, 255 };
		inline Color_t BluBuilding = { 72, 219, 251, 255 };
		inline Color_t Cloaked = { 150, 150, 150, 255 };
		inline Color_t Health = { 55, 200, 55, 255 };
		inline Color_t Ammo = { 255, 255, 255, 255 };
		inline Color_t Bomb = { 255, 75, 75, 255 };
		inline Color_t Merasmus = { 25, 255, 105, 255 };
		inline Color_t HeadlessHatman = { 100, 40, 255, 255 };
		inline Color_t Zombie = { 25, 255, 105, 255 };
		inline Color_t Monoculus = { 100, 40, 255, 255 };
		inline Color_t Tank = { 88, 133, 162, 255 };
		inline Color_t Other = { 140, 120, 160, 255 };
		inline Color_t Fresnel = { 0, 0, 0, 255 };
		inline Color_t WorldModulation = { 255, 255, 255, 255 };
		inline Color_t StaticPropModulation = { 255, 255, 255, 255 };
		inline Color_t ParticleModulation = { 255, 255, 255, 255 };
		inline Color_t ShowHitboxes = { 0, 255, 0, 255 };
		inline Color_t PredictedPath = { 0, 255, 0, 255 };
		inline Color_t TargetBBox = { 0, 255, 0, 50 };
		inline Color_t Default = { 255, 255, 255, 255 };
	}

	namespace Test
	{
		inline CVar<bool> testbool		= { true, L"testbool" };
		inline CVar<int> testint			= { 0, L"testint" };
		inline CVar<int> testkey			= { 0x0, L"testkey" };
		inline CVar<float> testfloat		= { 0.0f, L"testfloat" };
		inline Color_t testcolor = { 30, 30, 30, 255 };
	}
}