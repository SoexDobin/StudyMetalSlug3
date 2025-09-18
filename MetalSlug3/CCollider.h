#pragma once
#include "Define.h"
#include "CGameObject.h"

class CCollider
{
public:
	CCollider();
	virtual ~CCollider();

public:
	void			EnableShowCollision()				{ m_bShowCol = 1; }
	void			DisableShowCollision()				{ m_bShowCol = 0; }

	CGameObject*	GetParent()		 					{ return m_pParent; }
	bool			GetIsCollision() const				{ return m_bIsCol; }
	Vector2			GetPivot() const					{ return m_vPivot; }
	Vector2			GetSize() const						{ return m_vSize; }
	Vector2			GetOffset() const					{ return m_vOffset; }
	COL_TYPE		GetCollisionType() const			{ return m_eColType; }

	void			SetParent(CGameObject* _pObj)		{ m_pParent = _pObj; }
	void			SetCollision(const bool& _bCol)		{ m_bIsCol = _bCol; }
	void			SetPivot(const Vector2& _vPivot)	{ m_vPivot = _vPivot; }
	void			SetSize(const Vector2& _vSize)		{ m_vPivot = _vSize; }
	void			SetOffset(const Vector2& _vOffset)	{ m_vOffset = _vOffset; }
	void			SetCollisionType(COL_TYPE _eType)	{ m_eColType = _eType; }

public:
	virtual void	UpdateCollision();
	virtual void	RenderCollision(HDC _hDC);

protected:
	CGameObject*	m_pParent;
	bool			m_bShowCol;
	bool			m_bIsCol;
	RECT			m_tRect;
	Vector2			m_vPivot;
	Vector2			m_vSize;
	Vector2			m_vOffset;
	COL_TYPE		m_eColType;

	HPEN			m_hColPen;
	COLORREF		m_dwRGB;
};

