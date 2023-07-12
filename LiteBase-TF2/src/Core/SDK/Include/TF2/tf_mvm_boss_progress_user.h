#pragma once

class ITFMvMBossProgressUserAutoList {};

class CTFMvMBossProgressUser : public ITFMvMBossProgressUserAutoList
{
public:
	virtual const char *GetBossProgressImageName() const = 0;
	virtual float GetBossStatusProgress() const = 0;
};