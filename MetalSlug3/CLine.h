#pragma once
#include "tagLine.h"

class CLine
{
public:
	CLine();
	CLine(Vector2& _vLeft, Vector2& _vRight);
	~CLine();

public:
	void	Render(HDC hDC);

public:
	const LINE& GetLine()									{ return m_tLine; }
	Vector2		GetLeftPoint()								{ return m_tLine.vLeft; }
	Vector2		GetRightPoint()								{ return m_tLine.vRight; }
	void		SetLeftPoint(const Vector2& _vLeft)			{ m_tLine.vLeft = _vLeft; }
	void		SetRightPoint(const Vector2& _vRight)		{ m_tLine.vRight = _vRight; }

private:
	LINE		m_tLine;
};

