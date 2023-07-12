#pragma once

#define MAX_EVENT_NAME_LENGTH 32
#define MAX_EVENT_BITS 9
#define MAX_EVENT_NUMBER (1 << MAX_EVENT_BITS)
#define MAX_EVENT_BYTES 1024

class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char *GetName() const = 0;
	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char *keyName = 0) = 0;
	virtual bool GetBool(const char *keyName = 0, bool defaultValue = false) = 0;
	virtual int GetInt(const char *keyName = 0, int defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = 0, float defaultValue = 0.0f) = 0;
	virtual const char *GetString(const char *keyName = 0, const char *defaultValue = "") = 0;
	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
};