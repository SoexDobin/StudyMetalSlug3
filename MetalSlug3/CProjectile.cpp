#include "pch.h"
#include "CProjectile.h"
#include "CScrollManager.h"

CProjectile::CProjectile() : m_szImgKey(L""), m_iLayer(0), m_iDamage(0)
{
	m_eType = PROJECTILE;
}

CProjectile::~CProjectile()
{
}

void CProjectile::CheckOutOfWindow()
{
	if (m_tRect.right == (WINCX + SCROLLX)
		|| m_tRect.top == CScrollManager::GetInstance().GetMinScrollLock().y
		|| m_tRect.left == CScrollManager::GetInstance().GetMinScrollLock().x
		|| m_tRect.bottom == (WINCY + SCROLLY))
	{
		m_bDestroy = OBJ_DESTROY;
	}
}
