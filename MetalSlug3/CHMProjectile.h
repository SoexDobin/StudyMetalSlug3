#pragma once
#include "CProjectile.h"

class CHMProjectile : public CProjectile
{
public: 
	CHMProjectile();
	~CHMProjectile() override;

public:
	void        Initialize()                                         override;
	int         Update()                                             override;
	void        LateUpdate()                                         override;
	void        Render(HDC _hDC)                                     override;
	void        Release()                                            override;
	void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   override;

private:
	Vector2			m_vSpeed;
};

