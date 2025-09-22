#pragma once
#include "CSingleTon.h"

class CScrollManager final : public CSingleTon<CScrollManager>
{
	friend class CSingleTon;
private:
	CScrollManager();
	~CScrollManager() override;

public:
	void			SetScrollX(float _X)	{ m_fScrollX += _X; }
	void			SetScrollY(float _Y)	{ m_fScrollY += _Y; }
	float			GetScrollX() const		{ return m_fScrollX; }
	float			GetScrollY() const		{ return m_fScrollY; }
					
	void			SetMinScrollLockX(const float& _fLockX) { m_vMinScrollLock.x = _fLockX; }
	void			SetMaxScrollLockX(const float& _fLockX) { m_vMaxScrollLock.x = _fLockX; }
	void			SetMinScrollLockY(const float& _fLockY) { m_vMinScrollLock.y = _fLockY; }
	void			SetMaxScrollLockY(const float& _fLockY) { m_vMaxScrollLock.y = _fLockY; }
	Vector2			GetMinScrollLock()						{ return m_vMinScrollLock; }
	Vector2			GetMaxScrollLock()						{ return m_vMaxScrollLock; }

	void			ScrollLock();

private:
	float			m_fScrollX;
	float			m_fScrollY;

	Vector2			m_vMinScrollLock;
	Vector2			m_vMaxScrollLock;
};

