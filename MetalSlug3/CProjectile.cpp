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
	int RightOut = WINCX - SCROLLX;
	int LeftOut = CScrollManager::GetInstance().GetMinScrollLock().x;

	if (m_tRect.left >= (WINCX - SCROLLX))
		m_bDestroy = OBJ_DESTROY;

	if (m_tRect.right <= CScrollManager::GetInstance().GetMinScrollLock().x)
		m_bDestroy = OBJ_DESTROY;

	if (m_tRect.top <= -CScrollManager::GetInstance().GetMaxScrollLock().y)
		m_bDestroy = OBJ_DESTROY;
		
	if(m_tRect.bottom <= -CScrollManager::GetInstance().GetMinScrollLock().y)
		m_bDestroy = OBJ_DESTROY;
}
