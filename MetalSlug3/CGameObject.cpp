#include "pch.h"
#include "CGameObject.h"

CGameObject::CGameObject()
	: m_iHp(1), m_bDestroy(false)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_vPivot, sizeof(Vector2));
	ZeroMemory(&m_vSize, sizeof(Vector2));
	ZeroMemory(&m_vFace, sizeof(Vector2));
	ZeroMemory(&m_vDirection, sizeof(Vector2));
}

CGameObject::~CGameObject()
{
	
}

void CGameObject::UpdateGameObject()
{
	m_tRect.left	= LONG(m_vPivot.x - (m_vSize.x / 2.f));
	m_tRect.top		= LONG(m_vPivot.y - (m_vSize.y / 2.f));
	m_tRect.right	= LONG(m_vPivot.x + (m_vSize.x / 2.f));
	m_tRect.bottom	= LONG(m_vPivot.y + (m_vSize.y / 2.f));
}
