#pragma once
#include "../../SDK.h"

class CMeshBuilder;

enum PrecipitationType_t
{
	PRECIPITATION_TYPE_RAIN = 0,
	PRECIPITATION_TYPE_SNOW,
	PRECIPITATION_TYPE_ASH,
	PRECIPITATION_TYPE_SNOWFALL,
	NUM_PRECIPITATION_TYPES
};

class CPrecipitationParticle
{
public:
	Vector	m_Pos;
	Vector	m_Velocity;
	float	m_SpawnTime;				// Note: Tweak with this to change lifetime
	float	m_Mass;
	float	m_Ramp;

	float	m_flCurLifetime;
	float	m_flMaxLifetime;
};

class CClient_Precipitation : public CBaseEntity
{
	class CPrecipitationEffect;
	friend class CClient_Precipitation::CPrecipitationEffect;

public:

	NETVAR(GetPrecipType, PrecipitationType_t, "CPrecipitation", "m_nPrecipType")

	virtual ~CClient_Precipitation();

	// Inherited from C_BaseEntity
	virtual void Precache();

	void Render();

	// Creates a single particle
	CPrecipitationParticle* CreateParticle();

	virtual void OnDataChanged(DataUpdateType_t updateType);
	virtual void ClientThink();

	void Simulate(float dt);

	// Renders the particle
	void RenderParticle(CPrecipitationParticle* pParticle, CMeshBuilder& mb);

	void CreateWaterSplashes();

	// Emits the actual particles
	void EmitParticles(float fTimeDelta);

	// Computes where we're gonna emit
	bool ComputeEmissionArea(Vector& origin, Vector2D& size);

	// Gets the tracer width and speed
	float GetWidth() const;
	float GetLength() const;
	float GetSpeed() const;

	// Gets the remaining lifetime of the particle
	float GetRemainingLifetime(CPrecipitationParticle* pParticle) const;

	// Computes the wind vector
	static void ComputeWindVector();

	// simulation methods
	bool SimulateRain(CPrecipitationParticle* pParticle, float dt);
	bool SimulateSnow(CPrecipitationParticle* pParticle, float dt);

	void CreateAshParticle(void);
	void CreateRainOrSnowParticle(Vector vSpawnPosition, Vector vVelocity);

	// Information helpful in creating and rendering particles
	IMaterial* m_MatHandle;	// material used 
	//NETVAR(m_MatHandle, IMaterial*, "CPrecipitation", "m_MatHandle")

	float			m_Color[4];		// precip color
	float			m_Lifetime;		// Precip lifetime
	float			m_InitialRamp;	// Initial ramp value
	float			m_Speed;		// Precip speed
	float			m_Width;		// Tracer width
	float			m_Remainder;	// particles we should render next time
	PrecipitationType_t	m_nPrecipType;			// Precip type
	float			m_flHalfScreenWidth;	// Precalculated each frame.

	float			m_flDensity;

	// Some state used in rendering and simulation
	// Used to modify the rain density and wind from the console
	static ConVar s_raindensity;
	static ConVar s_rainwidth;
	static ConVar s_rainlength;
	static ConVar s_rainspeed;

	static Vector s_WindVector;			// Stores the wind speed vector
private:
	CClient_Precipitation(const CClient_Precipitation&); // not defined, not accessible
};