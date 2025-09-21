#pragma once
#include "CParticle.h"

class CAnimation;

class CBulletHitParticle final : public CParticle
{
public:
	CBulletHitParticle();
	~CBulletHitParticle() override;

public:
	void        Initialize()                                         override;
	int         Update()                                             override;
	void        LateUpdate()                                         override;
	void        Render(HDC _hDC)                                     override;
	void        Release()                                            override;
	void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   override;

private:
	CAnimation*		m_pAnim;
	pair<int, int>	m_pairFrame;
	const TCHAR*	m_szFrameKey;
};

