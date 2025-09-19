#pragma once
#include "CGameObject.h"

class CParticle : public CGameObject
{
public:
	CParticle();
	virtual ~CParticle();
	
public:
	virtual void        Initialize()                                         PURE;
	virtual int         Update()                                             PURE;
	virtual void        LateUpdate()                                         PURE;
	virtual void        Render(HDC _hDC)                                     PURE;
	virtual void        Release()                                            PURE;
	virtual void        OnCollision(CGameObject* _pCol, Vector2 _vColSize)   PURE;
};

