#pragma once
#include "CProjectile.h"

class CAnimation;

class CBomb : public CProjectile
{
public:
	CBomb();
	virtual ~CBomb() override;

public:
	void		Initialize() override;
	int			Update() override;
	void		LateUpdate() override;
	void		Render(HDC _hDC) override;
	void		Release() override;
	void		OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void		CheckPlatform();
	void		LoadBmp();

private:
	int				m_iBounds;
	CAnimation*		m_pAnim;
	CGameObject*	m_pPlatformCol;

	float			m_fDropSpeed;
	float			m_fDropDelta;
	float			m_fDelta;
};

