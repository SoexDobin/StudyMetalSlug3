#pragma once
#include "CCollision.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

public:
	const RECT&		GetRect() const			{ return m_tRect; }
	CCollision*		GetCollision()			{ return m_pCollision; }

	bool			IsDestroy() const		{ return m_bDestroy; }
	int				GetHp() const			{ return m_iHp; }
	Vector2			GetPivot() const		{ return m_vPivot; }
	Vector2			GetSize() const			{ return m_vSize; }
	Vector2			GetFace() const			{ return m_vFace; }
	Vector2			GetDirection() const	{ return m_vDirection; }
	
	bool			SetDestroy()								{ m_bDestroy = true; }
	void			SetHp(const int& _iHp)						{ m_iHp = _iHp; }
	void			SetPivot(const Vector2& _vPivot)			{ m_vPivot = _vPivot; }
	void			SetSize(const Vector2& _vSize)				{ m_vPivot = _vSize; }
	void			SetFace(const Vector2& _vFace)				{ m_vFace = _vFace; }
	void			SetDirection(const Vector2& _vDirection)	{ m_vDirection = _vDirection; }

public:
	virtual void	Initialize()										PURE;
	virtual int		Update()											PURE;
	virtual void	LateUpdate()										PURE;
	virtual void	Render(HDC _hDC)									PURE;
	virtual void	Release()											PURE;
	virtual void	OnCollision(CGameObject* _pCol, Vector2 _vColSize)	PURE;

protected:
	void			UpdateGameObject();

protected:
	bool			m_bDestroy;
	RECT			m_tRect;

	int				m_iHp;
	Vector2			m_vPivot;
	Vector2			m_vSize;
	Vector2			m_vFace;
	Vector2			m_vDirection;

	CCollision*		m_pCollision = nullptr;
};

