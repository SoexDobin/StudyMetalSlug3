#include "pch.h"
#include "CLine.h"
#include "CScrollManager.h"
#include "Define.h"

CLine::CLine()
{
}

CLine::CLine(Vector2& _vLeft, Vector2& _vRight)
{
	m_tLine.vLeft = _vLeft;
	m_tLine.vRight = _vRight;
}

CLine::~CLine()
{
}

void CLine::Render(HDC hDC)
{
	
	MoveToEx(hDC, (int)m_tLine.vLeft.x + SCROLLX, (int)m_tLine.vLeft.y + SCROLLY, nullptr);
	LineTo(hDC, (int)m_tLine.vRight.x + SCROLLX, (int)m_tLine.vRight.y + SCROLLY);

}