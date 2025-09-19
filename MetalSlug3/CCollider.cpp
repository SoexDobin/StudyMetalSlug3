#include "pch.h"
#include "CGameObject.h"
#include "CCollider.h"
#include "Define.h"

#include "CScrollManager.h"

CCollider::CCollider() 
	: m_pParent(nullptr)
	, m_eType(COL_TYPE_END)
	, m_bShowCol(false), m_bIsCol(false), m_eRenderType(RECT_COL)
	, m_hColPen(nullptr) , m_dwRGB(0)
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

void CCollider::RenderCollision(HDC _hDC)
{
	if (m_bShowCol == false) return;
	int iX = 0; static_cast<int>(CScrollManager::GetInstance().GetScrollX());
	int iY = 0; static_cast<int>(CScrollManager::GetInstance().GetScrollY());
	PAINTSTRUCT tPT;
	BeginPaint(g_hWnd, &tPT);
	m_hColPen = CreatePen(PS_SOLID, 10, m_dwRGB);
	HPEN hPrevPen = (HPEN)SelectObject(_hDC, m_hColPen);

	switch (m_eRenderType)
	{
	case RECT_COL: Rectangle(_hDC, m_tRect.left + iX, m_tRect.top + iY, m_tRect.right + iX, m_tRect.bottom + iY);
		break;
	case CIRCLE_COL: Ellipse(_hDC, m_tRect.left + iX, m_tRect.top + iY, m_tRect.right + iX, m_tRect.bottom + iY);
		break;
	}

	SelectObject(_hDC, hPrevPen);
	DeleteObject(m_hColPen);
	EndPaint(g_hWnd, &tPT);
}

void CCollider::Release()
{
	
}
