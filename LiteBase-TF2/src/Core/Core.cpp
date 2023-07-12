#include "Core.h"
#include "Features/Visuals/Render/GUI/Menu/GUI.h"
#include "Features/Visuals/Models/Models.h"
#include "Features/Visuals/Outlines/Outlines.h"
#include "Features/Events/Events.h"
#include <thread>
#include "Features/Visuals/AttributeChanger/AttributeChanger.h"
#include "Features/Visuals/Render/GUI/Menu/DoUser.h"
#include "SDK/Include/Steam/ISteamUserStats011.h"
#include "SDK/Include/Steam/ISteamClient017.h"
#include "SDK/Include/Steam/ISteamFriends015.h"
#include "Features/Misc/HookedNetVars/NetVarsHk.h"
#include "SDK/Include/Steam/ISteamNetworkingUtils.h"

//std::ofstream File;
//const char *szClassName;
//
//void DumpTable(RecvTable *pTable, int nDepth)
//{
//	if (!pTable)
//		return;
//
//	const char *Types[7] = { "int", "float", "Vec3", "Vec2", "const char *", "Array", "void *" };
//
//	if (nDepth == 0)
//		File << "class " << szClassName << "\n{\npublic:\n";
//
//	for (int n = 0; n < pTable->m_nProps; n++)
//	{
//		RecvProp *pProp = pTable->GetPropW(n);
//
//		if (!pProp)
//			continue;
//
//		std::string_view sVarName(pProp->m_pVarName);
//
//		if (!sVarName.find("baseclass") || !sVarName.find("0") || !sVarName.find("1") || !sVarName.find("2"))
//			continue;
//
//		const char *szType = Types[pProp->GetType()];
//
//		if (sVarName.find("m_b") == 0 && pProp->GetType() == 0)
//			szType = "bool";
//
//		if (pProp->GetOffset())
//			File << "\tNETVAR(" << sVarName << ", " << szType << ", \"" << szClassName << "\", \"" << sVarName << "\");\n";
//
//		if (auto DataTable = pProp->GetDataTable())
//			DumpTable(DataTable, nDepth + 1);
//	}
//
//	if (nDepth == 0)
//		File << "};\n";
//}
//
//void DumpTables()
//{
//	File.open("NETVAR_DUMP_NEW.h");
//
//	for (ClientClass *pClass = I::BaseClientDLL->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
//		szClassName = pClass->m_pNetworkName;
//		DumpTable(pClass->m_pRecvTable, 0);
//	}
//
//	File.close();
//}

void CCore::Load()
{
	while (!GetModuleHandleW(L"mss32.dll"))
		std::this_thread::sleep_for(std::chrono::seconds(5));

	//Interfaces
	{
		I::Surface = reinterpret_cast<ISurface *>(g_Interface.Get(L"vguimatsurface.dll", "VGUI_Surface030"));
		I::EngineClient = reinterpret_cast<IVEngineClient013 *>(g_Interface.Get(L"engine.dll", "VEngineClient014"));
		I::BaseClientDLL = reinterpret_cast<IBaseClientDLL *>(g_Interface.Get(L"client.dll", "VClient017"));
		I::ClientEntityList = reinterpret_cast<IClientEntityList *>(g_Interface.Get(L"client.dll", "VClientEntityList003"));
		I::ModelInfo = reinterpret_cast<IVModelInfo *>(g_Interface.Get(L"engine.dll", "VModelInfoClient006"));
		I::DebugOverlay = reinterpret_cast<IVDebugOverlay *>(g_Interface.Get(L"engine.dll", "VDebugOverlay003"));
		I::ClientModeShared = **reinterpret_cast<CClientModeShared ***>(g_Pattern.Find(L"client.dll", L"8B 0D ? ? ? ? 8B 02 D9 05") + 0x2);
		I::MaterialSystem = reinterpret_cast<IMaterialSystem *>(g_Interface.Get(L"materialsystem.dll", "VMaterialSystem081"));
		I::ModelRender = reinterpret_cast<IVModelRender *>(g_Interface.Get(L"engine.dll", "VEngineModel016"));
		I::EngineVGui = reinterpret_cast<IEngineVGui *>(g_Interface.Get(L"engine.dll", "VEngineVGui002"));
		I::RenderView = reinterpret_cast<IVRenderView *>(g_Interface.Get(L"engine.dll", "VEngineRenderView014"));
		I::PlayerInfoManager = reinterpret_cast<IPlayerInfoManager *>(g_Interface.Get(L"server.dll", "PlayerInfoManager002"));
		I::GlobalVars = *reinterpret_cast<CGlobalVarsBase **>(g_Pattern.Find(L"engine.dll", L"A1 ? ? ? ? 8B 11 68") + 0x8);
		I::EngineTrace = reinterpret_cast<IEngineTrace *>(g_Interface.Get(L"engine.dll", "EngineTraceClient003"));
		I::CVar = reinterpret_cast<ICvar *>(g_Interface.Get(L"vstdlib.dll", "VEngineCvar004"));
		I::Prediction = reinterpret_cast<CPrediction *>(g_Interface.Get(L"client.dll", "VClientPrediction001"));
		//I::MoveHelper = **reinterpret_cast<IMoveHelper ***>(g_Pattern.Find(L"client.dll", L"8B 0D ? ? ? ? 8B 46 08 68 ? ? ? ? 05 ? ? ? ? 8B 11 50") + 0x2);
		I::Input = **reinterpret_cast<IInput***>(g_Pattern.Find(L"client.dll", (L"8B 0D ? ? ? ? 56 8B 01 FF 50 24 8B 45 FC")) + 0x2);
		I::InputSystem = reinterpret_cast<IInputSystem*>(g_Interface.Get((L"inputsystem.dll"), ("InputSystemVersion001")));
		I::GameMovement = reinterpret_cast<CGameMovement*>(g_Interface.Get(L"client.dll", "GameMovement001"));
		I::Panel = reinterpret_cast< CPanel * >( g_Interface.Get( L"vgui2.dll", "VGUI_Panel009" ) );
		I::RandomSeed = *reinterpret_cast< int32_t ** >( g_Pattern.Find( L"client.dll", ( L"C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34" ) ) + 0x2 );
		I::ClientState = *reinterpret_cast< CClientState ** >( g_Pattern.Find( L"engine.dll", L"68 ? ? ? ? E8 ? ? ? ? 83 C4 08 5F 5E 5B 5D C3" ) + 0x1 );
		I::StudioRender = reinterpret_cast< void * >( g_Interface.Get( L"studiorender.dll", "VStudioRender025" ) );
		I::GameEvent = reinterpret_cast< CGameEventManager * >( g_Interface.Get( L"engine.dll", "GAMEEVENTSMANAGER002" ) );
		static auto fn = reinterpret_cast< ISteamNetworkingUtils * ( __cdecl * )( ) >( GetProcAddress( GetModuleHandleA( "steamnetworkingsockets.dll" ), "SteamNetworkingUtils_LibV4" ) );
		I::NetworkUtils = fn( );
		//I::ServerPlugin = reinterpret_cast< IServerPluginCallbacks * > ( g_Interface.Get( L"engine.dll", INTERFACEVERSION_ISERVERPLUGINCALLBACKS ) );

		auto pdwClient = reinterpret_cast< PDWORD >( I::BaseClientDLL );
		auto pdwTable = *reinterpret_cast< PDWORD * >( pdwClient );
		I::ViewRender = **reinterpret_cast< IViewRender *** >( static_cast< DWORD >( pdwTable[ 27 ] ) + 5 );

		I::SteamClient = g_Interface.Get<ISteamClient017 *>( L"steamclient.dll", "SteamClient017" );
		HSteamPipe hsNewPipe = I::SteamClient->CreateSteamPipe( );
		HSteamPipe hsNewUser = I::SteamClient->ConnectToGlobalUser( hsNewPipe );
		I::UserStats = reinterpret_cast< ISteamUserStats011 * >( I::SteamClient->GetISteamUserStats( hsNewUser, hsNewPipe, "STEAMUSERSTATS_INTERFACE_VERSION011" ) );
		I::Friends015 = reinterpret_cast< ISteamFriends015 * >( I::SteamClient->GetISteamFriends( hsNewUser, hsNewPipe, "SteamFriends015" ) );
		Beep( 500, 100 );
	}
	
	//Other
	{
		g_NetVars.Init( );
		g_NetVarHk.Init( );
		g_Draw.ReloadFonts( );
		Models.Init( );
		Outlines.Init( );
		g_ConVars.Init( );
		//g_AttributeChanger.Init( );
		g_Events.Setup( { "vote_cast", "player_changeclass", "player_connect",
						"player_hurt", "achievement_earned", "player_changename", 
						"vote_started", "teamplay_round_start", "player_spawn", 
						"item_pickup", "player_disconnect" } );
		g_User.Find( );
		Beep( 500, 100 );
	}

	//Hooks
	{
		MH_Initialize();
		{
			Hooks::WndProc::Init( );
			Hooks::CreateMove::Init( );
			Hooks::Paint::Init( );
			Hooks::OnScreenSizeChanged::Init( );
			Hooks::FrameStageNotify::Init( );
			Hooks::LevelInitPreEntity::Init( );
			Hooks::LevelInitPostEntity::Init( );
			Hooks::LevelShutdown::Init( );
			//Hooks::LevelInit::Init( );
			Hooks::OverrideView::Init( );
			Hooks::ShouldDrawViewmodel::Init( );
			Hooks::DoPostScreenSpaceEffects::Init( );
			Hooks::CTFGameMovement::Init( );
			Hooks::RunCommand::Init( );
			Hooks::LockCursor::Init( );
			Hooks::PaintTraverse::Init( );
			Hooks::IsPlayingTimeDemo::Init( );
			Hooks::DrawModelExecute::Init( );
			Hooks::ForcedMaterialOverride::Init( );
			Hooks::CL_FireEvents::Init( );
			Hooks::CL_Move::Init( );
			Hooks::CL_SendMove::Init( );
			Hooks::GetUserCmd::Init( );
			//Hooks::IsPlayerOnSteamFriendsList::Init( );
			Hooks::CreateTFRagdoll::Init( );
			Hooks::ViewOffset::Init( );
			Hooks::StartDynamicSound::Init( );
			Hooks::AddToCritBucket::Init( );
			Hooks::IsAllowedToWithdrawFromCritBucket::Init( );
			//Hooks::CalcIsAttackCritical::Init( );
			Hooks::EngineClientInterpAmount::Init( );
			Hooks::GetClientInterpAmount::Init( );
			Hooks::CheckForSequenceChange::Init( );
			Hooks::FireBullets::Init( );
			Hooks::CheckSimpleMaterial::Init( );
			//Hooks::MaterialUncache::Init( );
			Hooks::SendNetMsg::Init( );
			Hooks::DrawStaticProps::Init( );
			Hooks::SetColorModulation::Init( );
			Hooks::SetAlphaModulation::Init( );
			Hooks::FrameAdvance::Init( );
			Hooks::KeyValues::Init( );
			Hooks::IsPlayerDominated::Init( );
			Hooks::ChatPrintf::Init( );
			Hooks::InCond::Init( );
			//Hooks::RenderView::Init( );
			Hooks::AvoidPlayers::Init( );
			Hooks::DeleteIfUnreferenced::Init( );
			Hooks::GetMaxItemCount::Init( );
			Hooks::CL_LoadWhitelist::Init( );
			Hooks::MatchNotification::Init( );
			Hooks::ProcessDSP::Init( );
			Hooks::PostProcessing::Init( );
			//Hooks::PerformScreenSpaceEffects::Init( );
			Hooks::RenderSpriteCard::Init( );
			Hooks::UpdateAssignedLobby::Init( );
			Hooks::ModifySentChat::Init( );
			Hooks::GetDirectPingToPOP::Init( );
			Hooks::RenderViewSetColorModulation::Init( );
			Hooks::RenderViewSetBlend::Init( );
			Hooks::DrawViewModels::Init( );
			Hooks::DataTableWarning::Init( );
			//Hooks::Connect::Init( );
			//Hooks::Disconnect::Init( );
			//Hooks::IsTaunting::Init( );
			/*Hooks::CanMoveDuringTaunt::Init( );
			Hooks::CanMoveDuringTaunt1::Init( );
			Hooks::CanMoveDuringTaunt2::Init( );*/
			//Hooks::KeyEvent::Init( );
			//Hooks::FX_FireBullets::Init( );
			//Hooks::SetAbsVelocity::Init( );
			//Hooks::SendDatagram::Init( );
			//Hooks::GetOuterAbsVelocity::Init();
		}
		MH_EnableHook(MH_ALL_HOOKS);
	}

	I::CVar->ConsoleColorPrintf( { 255, 255, 255, 255 }, "Attached.\n" );
	I::EngineClient->ClientCmd_Unrestricted( "tf_scoreboard_mouse_mode 1" );
	I::EngineClient->ClientCmd_Unrestricted( "developer 1" );
	I::EngineClient->ClientCmd_Unrestricted( "mat_hdr_level 0" );

	/*if ( ConVar *mat_dxlevel = g_ConVars.FindVar( "mat_dxlevel" ) )
	{
		if ( mat_dxlevel->GetInt( ) != 95 )
		{
			mat_dxlevel->SetValue( 95 );
		}
	}*/

	if ( I::EngineClient->IsConnected( ) || I::EngineClient->IsInGame( ) )
		g_Events.PrintOnAttach( );

	Beep( 500, 100 );
}

void CCore::Unload()
{
	if (g_Menu.m_bOpen)
	{
		Hooks::LockCursor::Hook.CallOriginal<Hooks::LockCursor::fn>()(I::Surface);
		I::Surface->SetCursorAlwaysVisible(0);
		g_Menu.m_bOpen = false;
	}
	Vars::Visuals::ViewmodelFOV.var = g_Misc.ViewFOV;
	Vars::Visuals::AspectRatio.var = g_Misc.AspectRatio;

	if ( I::EngineClient->IsConnected( ) || I::EngineClient->IsInGame( ) )
		g_Events.PrintOnDetach( );

	g_Draw.DeleteTexturesFromMemory( );
	g_Events.Destroy( );
	MH_Uninitialize();
	if ( I::EngineClient->IsConnected( ) || I::EngineClient->IsInGame( ) )
	{
		g_Misc.RestoreWorldModulation( );
	}

	SetWindowLongPtr(Hooks::WndProc::hwWindow, GWL_WNDPROC, (LONG_PTR)Hooks::WndProc::Original);
	I::EngineClient->ClientCmd_Unrestricted( "r_drawviewmodel 1" );
	I::CVar->ConsoleColorPrintf( { 255, 90, 90, 255 }, "Detached.\n" );
}

bool CCore::ShouldUnload()
{
	return Utils::IsGameWindowInFocus( ) && ( ( GetAsyncKeyState( VK_DELETE ) & 0x8000 ) || ( GetAsyncKeyState( VK_F8 ) & 0x8000 ) );
}