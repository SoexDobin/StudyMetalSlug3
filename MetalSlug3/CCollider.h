#pragma once
#include "Define.h"

class CGameObject;

class CCollider
{
public:
	CCollider();
	virtual ~CCollider();

public:
	CGameObject*	GetParent()		 						{ return m_pParent; }
	bool			GetIsCollision() const					{ return m_bIsCol; }
	Vector2			GetPivot() const						{ return m_vPivot; }
	Vector2			GetSize() const							{ return m_vSize; }
	Vector2			GetOffset() const						{ return m_vOffset; }
	COLLIDER_TYPE	GetColType() const						{ return m_eType; }

	void			SetParent(CGameObject* _pObj)			{ m_pParent = _pObj; }
	void			SetCollision(const bool& _bCol)			{ m_bIsCol = _bCol; }
	void			SetPivot(const Vector2& _vPivot)		{ m_vPivot = _vPivot; }
	void			SetSize(const Vector2& _vSize)			{ m_vSize = _vSize; }
	void			SetOffset(const Vector2& _vOffset)		{ m_vOffset = _vOffset; }
	void			SetColType(const COLLIDER_TYPE _eType)	{ m_eType = _eType; }

	void				SetPenColor(COLORREF _dwRGB)			{ m_dwRGB = _dwRGB; }
	COLORREF			GetPenColor()							{ return m_dwRGB; }
	void				SetRenderType(COL_RENDER_TYPE _eType)	{ m_eRenderType = _eType; }
	COL_RENDER_TYPE		GetRenderType() const					{ return m_eRenderType; }
public:
	virtual bool	UpdateCollision();
	virtual void	RenderCollider(HDC _hDC);
	virtual void	Release();

protected:
	CGameObject*		m_pParent;
	bool				m_bIsCol;
	RECT				m_tRect;
	Vector2				m_vPivot;
	Vector2				m_vSize;
	Vector2				m_vOffset;
	COLLIDER_TYPE		m_eType;

	COL_RENDER_TYPE		m_eRenderType;
	COLORREF			m_dwRGB;
};

