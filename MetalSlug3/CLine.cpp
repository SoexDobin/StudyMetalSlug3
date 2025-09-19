#include "pch.h"
#include "CLine.h"
#include "CScrollManager.h"

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
	int iX = static_cast<int>(CScrollManager::GetInstance().GetScrollX());

	MoveToEx(hDC, (int)m_tLine.vLeft.x + iX, (int)m_tLine.vLeft.y, nullptr);
	LineTo(hDC, (int)m_tLine.vRight.x + iX, (int)m_tLine.vRight.y);

}