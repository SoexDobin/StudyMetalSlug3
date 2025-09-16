#pragma once

class CCollision
{
public:
	CCollision();
	virtual ~CCollision();

public:
	void			EnableShowCollision()				{ m_bShowCol = 1; }
	void			DisableShowCollision()				{ m_bShowCol = 0; }

	bool			GetIsCollision() const				{ return m_bIsCol; }
	Vector2			GetPivot() const					{ return m_vPivot; }
	Vector2			GetSize() const						{ return m_vSize; }
	COL_TYPE		GetCollisionType() const			{ return m_eColType; }

	void			SetIsCollision(const bool& _bCol)	{ m_bIsCol = _bCol; }
	void			SetPivot(const Vector2& _vPivot)	{ m_vPivot = _vPivot; }
	void			SetSize(const Vector2& _vSize)		{ m_vPivot = _vSize; }
	void			SetCollisionType(COL_TYPE _eType)	{ m_eColType = _eType; }

public:
	void			UpdateCollision();
	void			RenderCollision(HDC hDC);

protected:
	bool			m_bShowCol;
	bool			m_bIsCol;

	RECT			m_tRect;
	Vector2			m_vPivot;
	Vector2			m_vSize;
	COL_TYPE		m_eColType;
};

