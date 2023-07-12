#pragma once
#include "ivmodelrender.h"

class CSequenceTransitioner
{
public:
	void CheckForSequenceChange(
		// Describe the current animation state with these parameters.
		CStudioHdr* hdr,
		int nCurSequence,

		// Even if the sequence hasn't changed, you can force it to interpolate from the previous
		// spot in the same sequence to the current spot in the same sequence by setting this to true.
		bool bForceNewSequence,

		// Follows EF_NOINTERP.
		bool bInterpolate
	);

	void UpdateCurrent(
		// Describe the current animation state with these parameters.
		CStudioHdr* hdr,
		int nCurSequence,
		float flCurCycle,
		float flCurPlaybackRate,
		float flCurTime
	);

	void RemoveAll(void) { m_animationQueue.RemoveAll(); };

public:
	CUtlVector< CAnimationLayer >	m_animationQueue;
};