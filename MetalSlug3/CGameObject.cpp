#include "pch.h"
#include "CGameObject.h"
#include "CCollision.h"

CGameObject::CGameObject()
	: m_iHp(1), m_bDestroy(false), m_pCollision(nullptr)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_vPivot, sizeof(Vector2));
	ZeroMemory(&m_vSize, sizeof(Vector2));
	ZeroMemory(&m_vFace, sizeof(Vector2));
	ZeroMemory(&m_vDirection, sizeof(Vector2));
}

CGameObject::~CGameObject()
{
	if (m_pCollision)
	{
		delete m_pCollision;
		m_pCollision = nullptr;
	}
}

void CGameObject::UpdateGameObject()
{
	m_tRect.left	= LONG(m_vPivot.x - (m_vSize.x / 2.f));
	m_tRect.top		= LONG(m_vPivot.y - (m_vSize.y / 2.f));
	m_tRect.right	= LONG(m_vPivot.x + (m_vSize.x / 2.f));
	m_tRect.bottom	= LONG(m_vPivot.y + (m_vSize.y / 2.f));

	if (m_pCollision != nullptr)
		m_pCollision->UpdateCollision();
}
