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
					
	void			SetMinScrollLockX(const float& _fLockX, bool _bUseDelta = false)				{ m_vMinScrollLock.x = _fLockX; }
	void			SetMaxScrollLockX(const float& _fLockX, bool _bUseDelta = false)				{ m_vMaxScrollLock.x = _fLockX; m_bIsXMaxScrolling = _bUseDelta; }
	void			SetMinScrollLockY(const float& _fLockY, bool _bUseDelta = false)				{ m_vMinScrollLock.y = _fLockY; }
	void			SetMaxScrollLockY(const float& _fLockY, bool _bUseDelta = false)				{ m_vMaxScrollLock.y = _fLockY; }
	Vector2			GetMinScrollLock()						{ return m_vMinScrollLock; }
	Vector2			GetMaxScrollLock()						{ return m_vMaxScrollLock; }

	void			EnableMaxXScroll()						{ m_bIsXMaxScrolling = true; }
	void			DisableMaxXScroll()						{ m_bIsXMaxScrolling = false; }

	void			ScrollLock();

private:
	float			n_fScrollDeltaSpeed = 300.f;

	bool			m_bIsXMaxScrolling;
	bool			m_bIsYScrolling;
	float			m_fScrollX;
	float			m_fScrollY;

	Vector2			m_vMinScrollLock;
	Vector2			m_vMaxScrollLock;
};

