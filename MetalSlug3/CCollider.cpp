#include "pch.h"
#include "CCollider.h"
#include "Define.h"

CCollider::CCollider() 
	: m_bShowCol(false), m_bIsCol(false), m_eColType(RECT_COL)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_vPivot, sizeof(Vector2));
	ZeroMemory(&m_vSize, sizeof(Vector2));
}

CCollider::~CCollider()
{

}

void CCollider::UpdateCollision()
{
	m_tRect.left	= LONG(m_vPivot.x - (m_vSize.x / 2.f));
	m_tRect.top		= LONG(m_vPivot.y - (m_vSize.y / 2.f));
	m_tRect.right	= LONG(m_vPivot.x + (m_vSize.x / 2.f));
	m_tRect.bottom	= LONG(m_vPivot.y + (m_vSize.y / 2.f));
}

void CCollider::RenderCollision(HDC hDC)
{
	switch (m_eColType)
	{
	case RECT_COL: Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		break;
	case CIRCLE_COL: Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		break;
	}
}
