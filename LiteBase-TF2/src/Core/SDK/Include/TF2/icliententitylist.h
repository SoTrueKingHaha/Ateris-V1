#pragma once
#include <assert.h>

class IClientEntity;
class IClientNetworkable;
class IClientUnknown;
class CBaseHandle;

#define MAX_EDICT_BITS 11
#define MAX_EDICTS 1 << MAX_EDICT_BITS

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle &GetRefEHandle() const = 0;
};

class CBaseHandle
{

public:

	CBaseHandle( );
	CBaseHandle( const CBaseHandle &other );
	CBaseHandle( unsigned long value );
	CBaseHandle( int iEntry, int iSerialNumber );

	void Init( int iEntry, int iSerialNumber );
	void Term( );

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool IsValid( ) const;

	int GetEntryIndex( ) const;
	int GetSerialNumber( ) const;

	int ToInt( ) const;
	bool operator !=( const CBaseHandle &other ) const;
	bool operator ==( const CBaseHandle &other ) const;
	bool operator ==( const IHandleEntity *pEnt ) const;
	bool operator !=( const IHandleEntity *pEnt ) const;
	bool operator <( const CBaseHandle &other ) const;
	bool operator <( const IHandleEntity *pEnt ) const;

	// Assign a value to the handle.
	const CBaseHandle &operator=( const IHandleEntity *pEntity );
	const CBaseHandle &Set( const IHandleEntity *pEntity );

	// Use this to dereference the handle.
	// Note: this is implemented in game code (ehandle.h)
	IHandleEntity *Get( ) const;


protected:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	unsigned long	m_Index;
};

inline CBaseHandle::CBaseHandle( )
{
	m_Index = 0xFFFFFFFF;
}

inline CBaseHandle::CBaseHandle( const CBaseHandle &other )
{
	m_Index = other.m_Index;
}

inline CBaseHandle::CBaseHandle( unsigned long value )
{
	m_Index = value;
}

inline CBaseHandle::CBaseHandle( int iEntry, int iSerialNumber )
{
	Init( iEntry, iSerialNumber );
}

inline void CBaseHandle::Init( int iEntry, int iSerialNumber )
{
	assert( iEntry >= 0 && iEntry < (1 << 12) - 1 );
	assert( iSerialNumber >= 0 && iSerialNumber < ( 1 << 20 ) );

	m_Index = iEntry | ( iSerialNumber << 12 );
}

inline void CBaseHandle::Term( )
{
	m_Index = 0xFFFFFFFF;
}

inline bool CBaseHandle::IsValid( ) const
{
	return m_Index != 0xFFFFFFFF;
}

inline int CBaseHandle::GetEntryIndex( ) const
{
	return m_Index & (1 << 12) - 1;
}

inline int CBaseHandle::GetSerialNumber( ) const
{
	return m_Index >> 12;
}

inline int CBaseHandle::ToInt( ) const
{
	return ( int )m_Index;
}

inline bool CBaseHandle::operator !=( const CBaseHandle &other ) const
{
	return m_Index != other.m_Index;
}

inline bool CBaseHandle::operator ==( const CBaseHandle &other ) const
{
	return m_Index == other.m_Index;
}

inline bool CBaseHandle::operator ==( const IHandleEntity *pEnt ) const
{
	return Get( ) == pEnt;
}

inline bool CBaseHandle::operator !=( const IHandleEntity *pEnt ) const
{
	return Get( ) != pEnt;
}

inline bool CBaseHandle::operator <( const CBaseHandle &other ) const
{
	return m_Index < other.m_Index;
}

inline bool CBaseHandle::operator <( const IHandleEntity *pEntity ) const
{
	unsigned long otherIndex = ( pEntity ) ? pEntity->GetRefEHandle( ).m_Index : 0xFFFFFFFF;
	return m_Index < otherIndex;
}

inline const CBaseHandle &CBaseHandle::operator=( const IHandleEntity *pEntity )
{
	return Set( pEntity );
}

inline const CBaseHandle &CBaseHandle::Set( const IHandleEntity *pEntity )
{
	if ( pEntity )
	{
		*this = pEntity->GetRefEHandle( );
	}
	else
	{
		m_Index = 0xFFFFFFFF;
	}

	return *this;
}

template< class T >
class CHandle : public CBaseHandle
{
public:

	CHandle( );
	CHandle( int iEntry, int iSerialNumber );
	CHandle( const CBaseHandle &handle );
	CHandle( T *pVal );

	// The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
	static CHandle<T> FromIndex( int index );

	T *Get( ) const;
	void	Set( const T *pVal );

	operator T *( );
	operator T *( ) const;

	bool	operator !( ) const;
	bool	operator==( T *val ) const;
	bool	operator!=( T *val ) const;
	const CBaseHandle &operator=( const T *val );

	T *operator->( ) const;
};

class IClientEntityList
{
public:
	virtual IClientNetworkable *GetClientNetworkable(int entnum) = 0;
	virtual IClientNetworkable *GetClientNetworkableFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientUnknown *GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientEntity *GetClientEntity(int entnum) = 0;
	virtual IClientEntity *GetClientEntityFromHandle(CBaseHandle hEnt) = 0;
	virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int GetHighestEntityIndex(void) = 0;
	virtual void SetMaxEntities(int maxents) = 0;
	virtual int GetMaxEntities() = 0;
};

namespace I { inline IClientEntityList *ClientEntityList; }

#define LOCAL_IDX I::EngineClient->GetLocalPlayer()
#define GET_ENT_I(index) I::ClientEntityList->GetClientEntity(index)
#define GET_ENT_H(handle) I::ClientEntityList->GetClientEntityFromHandle(handle)
#define USING_LOCAL const auto& pLocal = GET_ENT_I( LOCAL_IDX )->As<CTFPlayer>( )
#define USING_ENTITY(index) const auto& pEntity = GET_ENT_I(index)
#define LOCAL_PLAYER GET_ENT_I( LOCAL_IDX )->As<CTFPlayer>( )
#define LOCAL_SANITY if ( !pLocal || pLocal->IsDormant( ) ) { return; }
#define LOCAL_SANITYCHECK if ( !pLocal || pLocal->IsDormant( ) )
#define USING_WEAPON const auto& pWeapon = GET_ENT_H( pLocal->m_hActiveWeapon( ) )->As<CTFWeaponBase>( )
#define LOCAL_WEAPON GET_ENT_H( pLocal->m_hActiveWeapon( ) )->As<CTFWeaponBase>( )
#define WEAPON_SANITY if ( !pWeapon || pWeapon->IsDormant( ) ) { return; }
#define WEAPON_SANITYCHECK if ( !pWeapon || pWeapon->IsDormant( ) )
#define ENTITY_SANITY if ( !pEntity || pEntity->IsDormant( ) ) { continue; }