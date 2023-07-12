#pragma once
#include "iclientmode.h"
#include "../../../Utils/VFunc/VFunc.h"

class CBaseViewport;
class CReplayReminderPanel;
class CBaseHudWeaponSelection;

class CClockDriftMgr
{
private:
	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float m_ClockOffsets[NUM_CLOCKDRIFT_SAMPLES];
	int m_iCurClockOffset;

	int m_nServerTick;		// Last-received tick from the server.
	int	m_nClientTick;		// The client's own tick counter (specifically, for interpolation during rendering).
							// The server may be on a slightly different tick and the client will drift towards it.
};

class CClientState
{
public:
	byte pad0[0x10];
	INetChannel* m_NetChannel;			// 0x10
	byte pad1[0x140];
	CClockDriftMgr	m_ClockDriftMgr;		// 0x154
	int				m_nDeltaTick;			// 0x1A0
	byte pad2[0x110];
	int				m_nMaxClients;			// 0x2B4	
	byte pad3[0x486C];
	int				lastoutgoingcommand;	// 0x4B24
	int				chokedcommands;			// 0x4B28
	int				last_command_ack;		// 0x4B2C

	int GetSignonState( )
	{
		return *reinterpret_cast< int * >( reinterpret_cast< uintptr_t >( this ) + 304 );
	}

	int GetServerCount( )
	{
		return *reinterpret_cast< int * >( reinterpret_cast< uintptr_t >( this ) + 320 );
	}

	void ForceFullUpdate( )
	{
		using FN = void( __thiscall * )( CClientState * );
		return reinterpret_cast< FN >( g_Pattern.Find( L"engine.dll",  L"56 8B F1 83 BE ? ? ? ? ? 74 1D" ) )( this );
	}
}; namespace I { inline CClientState* ClientState; };

class CBaseHudChat
{
public:

	enum
	{
		CHAT_INTERFACE_LINES = 6,
		MAX_CHARS_PER_LINE = 128
	};

	virtual void	CreateChatInputLine(void);
	virtual void	CreateChatLines(void);

	virtual void	Init(void);

	void			LevelInit(const char* newmap);
	void			LevelShutdown(void);

	void			MsgFunc_TextMsg(const char* pszName, int iSize, void* pbuf);

	virtual void	Printf(int iFilter, const char* fmt, ...);
	//virtual void	ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...);
	void ChatPrintf( int pIndex, const char *fmt, ... )
	{
		typedef void( _cdecl *FN )( void *, int, int, const char *, ... );
		GetVFunc<FN>( this, 19 )( this, pIndex, 0, fmt );
	}

	virtual void	StartMessageMode(int iMessageModeType);
	virtual void	StopMessageMode(void);
	void			Send(void);
};

class CHudChat : public CBaseHudChat
{
	bool IsChatPanelOutOfFocus( )
	{
		void *CHudChat = GetVFunc<void *( __thiscall * )( void * )>( this, 19 )( this );
		return ( CHudChat && !*( float * )( ( DWORD )CHudChat + 0xFC ) );
	}
};

class CClientModeShared : public IClientMode, public CGameEventListener
{
public:
	virtual ~CClientModeShared();
	virtual void Init();
	virtual void InitViewport();
	virtual void VGui_Shutdown();
	virtual void Shutdown();
	virtual void LevelInit(const char *newmap);
	virtual void LevelShutdown(void);
	virtual void Enable();
	virtual void Disable();
	virtual void Layout();
	virtual void ReloadScheme(bool flushLowLevel);
	virtual void OverrideView(CViewSetup *pSetup);
	virtual bool ShouldDrawDetailObjects();
	virtual bool ShouldDrawEntity(CBaseEntity *pEnt);
	virtual bool ShouldDrawLocalPlayer(CBasePlayer *pPlayer);
	virtual bool ShouldDrawViewModel();
	virtual bool ShouldDrawParticles();
	virtual bool ShouldDrawCrosshair(void);
	virtual bool ShouldBlackoutAroundHUD() = 0;
	virtual HeadtrackMovementMode_t ShouldOverrideHeadtrackControl() = 0;
	virtual void AdjustEngineViewport(int &x, int &y, int &width, int &height);
	virtual void PreRender(CViewSetup *pSetup);
	virtual void PostRender();
	virtual void PostRenderVGui();
	virtual void ProcessInput(bool bActive);
	virtual bool CreateMove(float flInputSampleTime, CUserCmd *cmd);
	virtual void Update();
	virtual int KeyInput(int down, ButtonCode_t keynum, const char *pszCurrentBinding);
	virtual int HudElementKeyInput(int down, ButtonCode_t keynum, const char *pszCurrentBinding);
	virtual void OverrideMouseInput(float *x, float *y);
	virtual void StartMessageMode(int iMessageModeType);
	virtual unsigned int *GetMessagePanel();
	virtual void ActivateInGameVGuiContext(unsigned int *pPanel);
	virtual void DeactivateInGameVGuiContext();
	virtual bool ShouldDrawFog(void);
	virtual float GetViewModelFOV(void);
	virtual unsigned int *GetViewport() = 0;
	virtual void *GetViewportAnimationController() = 0;
	virtual void FireGameEvent(IGameEvent *event);
	virtual bool CanRecordDemo(char *errorMsg, int length) const { return true; }
	virtual int HandleSpectatorKeyInput(int down, ButtonCode_t keynum, const char *pszCurrentBinding);
	virtual void ComputeVguiResConditions(KeyValues *pkvConditions) = 0;
	virtual wchar_t *GetServerName() { return NULL; }
	virtual void SetServerName(wchar_t *name) {}
	virtual wchar_t *GetMapName() { return NULL; }
	virtual void SetMapName(wchar_t *name) {}
	virtual bool DoPostScreenSpaceEffects(const CViewSetup *pSetup);
	virtual void DisplayReplayMessage(const char *pLocalizeName, float flDuration, bool bUrgent, const char *pSound, bool bDlg);
	virtual bool IsInfoPanelAllowed() = 0;
	virtual void InfoPanelDisplayed() = 0;
	virtual bool IsHTMLInfoPanelAllowed() = 0;
	virtual void OnDemoRecordStart(char const *pDemoBaseName) = 0;
	virtual void OnDemoRecordStop() = 0;

	CBaseViewport *m_pViewport;
	virtual void UpdateReplayMessages() = 0;
	float m_flReplayStartRecordTime;
	float m_flReplayStopRecordTime;
	CReplayReminderPanel *m_pReplayReminderPanel;
	CBaseHudChat *m_pChatElement;
	unsigned long m_CursorNone;
	CBaseHudWeaponSelection *m_pWeaponSelection;
	int m_nRootSize[2];
};

namespace I { inline CClientModeShared *ClientModeShared; }