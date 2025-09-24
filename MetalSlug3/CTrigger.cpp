#include "pch.h"
#include "CTrigger.h"

CTrigger::CTrigger()
	: m_pCallBack(nullptr), m_pColPredicate(nullptr)
{
	m_dwRGB = RGB( 0, 0, 255 );
}

CTrigger::~CTrigger()
{
}

bool CTrigger::UpdateCollision()
{
	__super::UpdateCollision();

	bool bHasTrigger = (*m_pColPredicate)();

	if (bHasTrigger)
	{
		(*m_pCallBack)();

		return bHasTrigger && m_bIsCol;
	}
	
	return bHasTrigger;
}
