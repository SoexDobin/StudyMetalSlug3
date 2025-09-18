#pragma once
#include "CSingleTon.h"

class CScrollManager final : public CSingleTon<CScrollManager>
{
	friend class CSingleTon;
private:
	CScrollManager();
	~CScrollManager() override;

public:
	void		SetScrollX(float _X)	{ m_fScrollX += _X; }
	void		SetScrollY(float _Y)	{ m_fScrollY += _Y; }
	float		GetScrollX()			{ return m_fScrollX; }
	float		GetScrollY()			{ return m_fScrollY; }

	void		SetScrollLock(const Vector2 _vLock) { m_vScrollLock = _vLock; }
	Vector2		GetScrollLock()						{ return m_vScrollLock; }

	void		ScrollLock();

private:
	float			m_fScrollX;
	float			m_fScrollY;

	Vector2			m_vScrollLock;
};

