#pragma once

class CAttributeManager;
class CAttributeContainer;
class CBaseEntity;
class CAttribute
{
public:
	void *_pad0;
	std::uint16_t m_nAttributeDefIndex;
	float m_flValue;
	unsigned int _pad1;

	__inline CAttribute( std::uint16_t nAttributeDefIndex, float flVal ) {
		m_nAttributeDefIndex = nAttributeDefIndex;
		m_flValue = flVal;
	}
};
class CAttributeList
{
public:
	void *_pad0;
	CUtlVector<CAttribute> m_Attributes;

	__inline void Add( int iIndex, float flValue ) {
		if ( m_Attributes.Count( ) > 14 ) return;

		CAttribute Attr( iIndex, flValue );

		m_Attributes.AddToTail( Attr );
	}
};

class IHasAttributes
{
public:
	virtual CAttributeManager *GetAttributeManager(void) = 0;
	virtual CAttributeContainer *GetAttributeContainer(void) = 0;
	virtual CBaseEntity *GetAttributeOwner(void) = 0;
	virtual CAttributeList *GetAttributeList(void) = 0;
	virtual void ReapplyProvision(void) = 0;
};