#pragma once
#include "CGameObject.h"

class CMouse final : public CGameObject
{
public:
	CMouse();
	~CMouse() override;

public:
	void	Initialize()										override;
	int		Update()											override;
	void	LateUpdate()										override;
	void	Render(HDC _hDC)									override;
	void	Release()											override;
	void	OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;

private:
	void	UpdateMouseFunction();

private:
	Vector2 m_vCurPos;
	Vector2 m_vFromPos;
	Vector2 m_vToPos;
	Vector2 m_vDistance;

	TCHAR	m_szCurPosBuffer[256];
	TCHAR	m_szFromPosBuffer[256];
	TCHAR	m_szToPosBuffer[256];
	TCHAR	m_szDistanceBuffer[256];
};

