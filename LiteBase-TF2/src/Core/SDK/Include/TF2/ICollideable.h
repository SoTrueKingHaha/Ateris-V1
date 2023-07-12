#pragma once
#include "icliententity.h"

enum SolidType_t
{
	SOLID_NONE = 0,	// no solid model
	SOLID_BSP = 1,	// a BSP tree
	SOLID_BBOX = 2,	// an AABB
	SOLID_OBB = 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW = 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM = 5,	// Always call into the entity for tests
	SOLID_VPHYSICS = 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};

enum SolidFlags_t
{
	FSOLID_CUSTOMRAYTEST = 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST = 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID = 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER = 0x0008,	// This is something may be collideable but fires touch functions
	// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
	FSOLID_NOT_STANDABLE = 0x0010,	// You can't stand on this
	FSOLID_VOLUME_CONTENTS = 0x0020,	// Contains volumetric contents (like water)
	FSOLID_FORCE_WORLD_ALIGNED = 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
	FSOLID_USE_TRIGGER_BOUNDS = 0x0080,	// Uses a special trigger bounds separate from the normal OBB
	FSOLID_ROOT_PARENT_ALIGNED = 0x0100,	// Collisions are defined in root parent's local coordinate space
	FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,	// This trigger will touch debris objects

	FSOLID_MAX_BITS = 10
};

struct Ray_t;
struct model_t;
class CGameTrace;
typedef CGameTrace trace_t;

class ICollideable
{
public:
	virtual IHandleEntity *GetEntityHandle() = 0;
	virtual const Vector &OBBMinsPreScaled() const = 0;
	virtual const Vector &OBBMaxsPreScaled() const = 0;
	virtual const Vector &OBBMins() const = 0;
	virtual const Vector &OBBMaxs() const = 0;
	virtual void WorldSpaceTriggerBounds(Vector *pVecWorldMins, Vector *pVecWorldMaxs) const = 0;
	virtual bool TestCollision(const Ray_t &ray, unsigned int fContentsMask, trace_t &tr) = 0;
	virtual bool TestHitboxes(const Ray_t &ray, unsigned int fContentsMask, trace_t &tr) = 0;
	virtual int GetCollisionModelIndex() = 0;
	virtual const model_t *GetCollisionModel() = 0;
	virtual const Vector &GetCollisionOrigin() const = 0;
	virtual const QAngle &GetCollisionAngles() const = 0;
	virtual const matrix3x4_t &CollisionToWorldTransform() const = 0;
	virtual SolidType_t	GetSolid() const = 0;
	virtual int GetSolidFlags() const = 0;
	virtual IClientUnknown *GetIClientUnknown() = 0;
	virtual int GetCollisionGroup() const = 0;
	virtual void WorldSpaceSurroundingBounds(Vector *pVecMins, Vector *pVecMaxs) = 0;
	virtual bool ShouldTouchTrigger(int triggerSolidFlags) const = 0;
	virtual const matrix3x4_t *GetRootParentToWorldTransform() const = 0;
};