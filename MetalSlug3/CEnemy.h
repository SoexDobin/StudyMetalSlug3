#pragma once
#include "CGameObject.h"

class CEnemy abstract : public CGameObject
{
public:
    CEnemy();
    virtual ~CEnemy() override;
public:
    virtual void        Initialize()                                         PURE;
    virtual int         Update()                                             PURE;
    virtual void        LateUpdate()                                         PURE;
    virtual void        Render(HDC _hDC)                                     PURE;
    virtual void        Release()                                            PURE;
    virtual void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   PURE;
};

