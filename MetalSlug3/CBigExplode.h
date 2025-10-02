#pragma once
#include "CParticle.h"

// 0 26 510 510

class CAnimation;

class CBigExplode :
    public CParticle
{
public:
	CBigExplode();
	~CBigExplode() override;

public:
	void        Initialize()                                         override;
	int         Update()                                             override;
	void        LateUpdate()                                         override;
	void        Render(HDC _hDC)                                     override;
	void        Release()                                            override;
	void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   override;

private:
	CAnimation* m_pAnim;
};

