#include "pch.h"
#include "CGameObject.h"
#include "CCollider.h"
#include "Define.h"

#include "CScrollManager.h"

CCollider::CCollider() 
	: m_pParent(nullptr)
	, m_eType(COL_TYPE_END)
	, m_bIsCol(false), m_eRenderType(RECT_COL)
	, m_dwRGB(0)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_vPivot, sizeof(Vector2));
	ZeroMemory(&m_vSize, sizeof(Vector2));
	ZeroMemory(&m_vOffset, sizeof(Vector2));
}

CCollider::~CCollider()
{
	Release();
}

void CCollider::UpdateCollision()
{
	m_vPivot = m_pParent->GetPivot() + m_vOffset;

	m_tRect.left	= LONG(m_vPivot.x - (m_vSize.x / 2.f));
	m_tRect.top		= LONG(m_vPivot.y - (m_vSize.y / 2.f));
	m_tRect.right	= LONG(m_vPivot.x + (m_vSize.x / 2.f));
	m_tRect.bottom	= LONG(m_vPivot.y + (m_vSize.y / 2.f));
}

void CCollider::RenderCollider(HDC _hDC)
{
	HPEN hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN hPrevPen = (HPEN)SelectObject(_hDC, hGreenPen);
	HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(_hDC, hNullBrush);

	switch (m_eRenderType)
	{
	case RECT_COL: Rectangle(_hDC, m_tRect.left + SCROLLX, m_tRect.top + SCROLLY, m_tRect.right + SCROLLX, m_tRect.bottom + SCROLLY);
		break;
	case CIRCLE_COL: Ellipse(_hDC, m_tRect.left + SCROLLX, m_tRect.top + SCROLLY, m_tRect.right + SCROLLX, m_tRect.bottom + SCROLLY);
		break;
	}

	SelectObject(_hDC, hPrevPen);
	SelectObject(_hDC, hPrevBrush);
	DeleteObject((HPEN)hGreenPen);
	DeleteObject((HBRUSH)hNullBrush);
}

void CCollider::Release()
{
	m_pParent = nullptr;
}
