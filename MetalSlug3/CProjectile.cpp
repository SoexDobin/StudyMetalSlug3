#include "pch.h"
#include "CProjectile.h"

CProjectile::CProjectile() : m_szImgKey(L""), m_iLayer(0), m_iDamage(0)
{
	m_eType = PROJECTILE;
}

CProjectile::~CProjectile()
{
}

void CProjectile::CheckOutOfWindow()
{
	if (m_tRect.right == WINCX
		|| m_tRect.top == 0
		|| m_tRect.left == 0
		|| m_tRect.bottom == WINCY)
	{
		m_bDestroy = OBJ_DESTROY;
	}
}
