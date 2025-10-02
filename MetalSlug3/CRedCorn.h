#pragma once
#include "CProjectile.h"

class CAnimation;

class CRedCorn : public CProjectile
{
public:
	CRedCorn();
	~CRedCorn() override;

public:
	void			Initialize() override;

	int 			Update() override;

	void			LateUpdate() override;

	void			Render(HDC _hDC) override;

	void			Release() override;

	void			OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void			SetBmpByAngle();

private:
	CAnimation*			m_pAnim;
	bool				m_bIsRotate;
};

