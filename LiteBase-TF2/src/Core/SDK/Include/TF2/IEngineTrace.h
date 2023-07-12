#pragma once
#include "ICollideable.h"

#pragma warning (push)
#pragma warning (disable : 26495)

struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class __declspec(align(16))VectorAligned : public Vec3
{
public:
	inline VectorAligned(void) { };

	inline VectorAligned(float x, float y, float z) {
		Set(x, y, z);
	}

	explicit VectorAligned(const Vec3 &othr) {
		Set(othr.x, othr.y, othr.z);
	}

	VectorAligned &operator=(const Vec3 &othr) {
		Set(othr.x, othr.y, othr.z);
		return *this;
	}

	float w;
};

struct Ray_t
{
	VectorAligned  m_Start;	// starting point, centered within the extents
	VectorAligned  m_Delta;	// direction + length of the ray
	VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
	bool m_IsRay;	// are the extents zero?
	bool m_IsSwept;	// is delta != 0?

	void Init(Vector const &start, Vector const &end)
	{
		m_Delta = end - start;
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Set();
		m_IsRay = true;
		m_StartOffset.Set();
		m_Start = start;
	}

	void Init(Vector const &start, Vector const &end, Vector const &mins, Vector const &maxs)
	{
		m_Delta = end - start;
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);
		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}

	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis)
		{
			if (m_Delta[iAxis] != 0.0f)
			{
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else
			{
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}

private:
};

#pragma warning (pop)

class CTraceListData;

#define DISPSURF_FLAG_SURFACE		(1 << 0)
#define DISPSURF_FLAG_WALKABLE		(1 << 1)
#define DISPSURF_FLAG_BUILDABLE		(1 << 2)
#define DISPSURF_FLAG_SURFPROP1		(1 << 3)
#define DISPSURF_FLAG_SURFPROP2		(1 << 4)

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos = Vec3( );				// start position
	Vector			endpos = Vec3( );					// final position
	cplane_t		plane = {};					// surface normal at impact
	float			fraction = 0.0f;				// time completed, 1.0 = didn't hit anything
	int				contents = 0;				// contents on other side of surface hit
	unsigned short	dispFlags = 0;				// displacement flags for marking surfaces with data
	bool			allsolid = false;				// if true, plane is not valid
	bool			startsolid = false;				// if true, the initial point was in a solid area
};

struct csurface_t
{
	const char *name;
	short		surfaceProps;
	unsigned short	flags;		// BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int  GetEntityIndex() const;

	bool DidHit() const {
		return (fraction < 1 || allsolid || startsolid);
	}

public:

	float		fractionleftsolid;		// time we left a solid, only valid if we started in solid
	csurface_t	surface;				// surface hit (impact surface)
	int			hitgroup;				// 0 == generic, non-zero is specific body part
	short		physicsbone;			// physics bone hit by trace in studio
	CBaseEntity *m_pEnt;
	// NOTE: this member is overloaded.
	// If hEnt points at the world entity, then this is the static prop index.
	// Otherwise, this is the hitbox index.
	int			hitbox;					// box hit by trace in studio
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class CTraceFilter
{
public:
	virtual bool ShouldHitEntity(void *pServerEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilterEntitiesOnly : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(void*pServerEntity, int contentsMask) override {
		return true;
	}

	virtual TraceType_t	GetTraceType() const override {
		return TRACE_ENTITIES_ONLY;
	}
};

class CTraceFilterWorldOnly : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(void*pServerEntity, int contentsMask) override {
		return false;
	}

	virtual TraceType_t	GetTraceType() const override {
		return TRACE_WORLD_ONLY;
	}
};

class CTraceFilterHitscan : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int nContentsMask);
	virtual TraceType_t GetTraceType() const;
	CBaseEntity* pSkip = nullptr;
};

class CTraceFilterWorldAndPropsOnly : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int nContentsMask);
	virtual TraceType_t	GetTraceType() const;
	CBaseEntity* pSkip = nullptr;
};

class CTraceFilterHitAll : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(void *pServerEntity, int contentsMask) override {
		return true;
	}

	virtual TraceType_t	GetTraceType() const override {
		return TRACE_EVERYTHING;
	}

	CBaseEntity* pSkip = nullptr;
};

class IEntityEnumerator
{
public:
	virtual bool EnumEntity(IHandleEntity *pHandleEntity) = 0;
};

class IEngineTrace
{
public:
	virtual int GetPointContents(const Vector &vecAbsPosition, IHandleEntity **ppEntity = NULL) = 0;
	virtual int GetPointContents_Collideable(ICollideable *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, trace_t *pTrace) = 0;
	virtual void ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, trace_t *pTrace) = 0;
	virtual void TraceRay(const Ray_t &ray, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace) = 0;
	virtual void SetupLeafAndEntityListRay(const Ray_t &ray, CTraceListData &traceData) = 0;
	virtual void SetupLeafAndEntityListBox(const Vector &vecBoxMin, const Vector &vecBoxMax, CTraceListData &traceData) = 0;
	virtual void TraceRayAgainstLeafAndEntityList(const Ray_t &ray, CTraceListData &traceData, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace) = 0;
	virtual void SweepCollideable(ICollideable *pCollide, const Vector &vecAbsStart, const Vector &vecAbsEnd,
		const QAngle &vecAngles, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace) = 0;
	virtual void EnumerateEntities(const Ray_t &ray, bool triggers, IEntityEnumerator *pEnumerator) = 0;
	virtual void EnumerateEntities(const Vector &vecAbsMins, const Vector &vecAbsMaxs, IEntityEnumerator *pEnumerator) = 0;
	virtual ICollideable *GetCollideable(IHandleEntity *pEntity) = 0;
	virtual int GetStatByIndex(int index, bool bClear) = 0;
	virtual void GetBrushesInAABB(const Vector &vMins, const Vector &vMaxs, CUtlVector<int> *pOutput, int iContentsMask = 0xFFFFFFFF) = 0;
	virtual CPhysCollide *GetCollidableFromDisplacementsInAABB(const Vector &vMins, const Vector &vMaxs) = 0;
	virtual bool GetBrushInfo(int iBrush, CUtlVector<Vector4D> *pPlanesOut, int *pContentsOut) = 0;
	virtual bool PointOutsideWorld(const Vector &ptTest) = 0;
	virtual int GetLeafContainingPoint(const Vector &ptTest) = 0;
};

namespace I { inline IEngineTrace *EngineTrace; }