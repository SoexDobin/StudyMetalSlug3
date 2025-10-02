#pragma once
#include "CParticle.h"

class CAnimation;
// 0 28 150 150

class CMediumExplode :
    public CParticle
{
public:
	CMediumExplode();
	~CMediumExplode() override;

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

