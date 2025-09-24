#include "pch.h"
#include "Define.h"
#include "CScrollManager.h"
#include "CTimeManager.h"

CScrollManager::CScrollManager() : m_fScrollX(0.f), m_fScrollY(0.f), m_bIsYScrolling(false), m_bIsXMaxScrolling(false)
{
	ZeroMemory(&m_vMinScrollLock, sizeof(Vector2));
	ZeroMemory(&m_vMaxScrollLock, sizeof(Vector2));
}

CScrollManager::~CScrollManager()
{
}

void CScrollManager::ScrollLock()
{
	if (m_vMinScrollLock.x < m_fScrollX)
		m_fScrollX = m_vMinScrollLock.x;

	if (m_bIsXMaxScrolling)
	{
		m_fScrollX += n_fScrollDeltaSpeed * DELTA;
		m_bIsXMaxScrolling = WINCX - m_vMaxScrollLock.x >= m_fScrollX;
	}
	else if (WINCX - m_vMaxScrollLock.x > m_fScrollX)
		m_fScrollX = (WINCX - m_vMaxScrollLock.x);

	if (m_bIsYScrolling && -m_vMinScrollLock.y < m_fScrollY)
	{
		m_fScrollY += n_fScrollDeltaSpeed * DELTA;
	}
	if (m_fScrollY >= m_vMinScrollLock.y || m_vMinScrollLock.y == 0.f)
	{
		m_bIsYScrolling = false;
	}
	else
	{
		m_bIsYScrolling = true;
	}
		
}
