#include "pch.h"
#include "Define.h"
#include "CScrollManager.h"

CScrollManager::CScrollManager() : m_fScrollX(0.f), m_fScrollY(0.f)
{
	ZeroMemory(&m_vMinScrollLock, sizeof(Vector2));
	ZeroMemory(&m_vMaxScrollLock, sizeof(Vector2));
}

CScrollManager::~CScrollManager()
{
}

void CScrollManager::ScrollLock()
{
	if (0.f < m_fScrollX)
		m_fScrollX = m_vMinScrollLock.x;

	if (0.f < m_fScrollY)
		m_fScrollY = m_vMinScrollLock.y;

	//6526
	if (WINCX - m_vMaxScrollLock.x > m_fScrollX)
		m_fScrollX = WINCX - m_vMaxScrollLock.x;

	if (WINCY - m_vMaxScrollLock.y > m_fScrollY)
		m_fScrollY = WINCY - m_vMaxScrollLock.y;

}
