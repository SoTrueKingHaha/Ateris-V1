#pragma once
#include "Classes.h"
#include "Const.h"
#include "Enums.h"
#include "Structs.h"
#include "bitbuf.h"
#include "MD5.h"
#include "worldsize.h"

#include <deque>
#include "../../../Utils/Vector/Vector.h"
using Vector = Vec3;
using QAngle = Vec3;
class IRefCounted
{
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};