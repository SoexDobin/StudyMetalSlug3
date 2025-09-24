#pragma once
#include "CGameObject.h"

class CEnemy : public CGameObject
{
public:
    CEnemy();
    virtual ~CEnemy();

public:
    int        GetHp()                      { return m_iHp; }
    int        SetHp(const int _iHp)        { m_iHp = _iHp; }

public:
    virtual void        Initialize()                                         PURE;
    virtual int         Update()                                             PURE;
    virtual void        LateUpdate()                                         PURE;
    virtual void        Render(HDC _hDC)                                     PURE;
    virtual void        Release()                                            PURE;
    virtual void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   PURE;
protected:
    int				m_iHp;
};

