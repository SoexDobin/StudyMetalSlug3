#pragma once
#include "CGameObject.h"
class CItem :
    public CGameObject
{
public:
    CItem();
    ~CItem() override;

public:
    virtual void        Initialize()                                         PURE;
    virtual int         Update()                                             PURE;
    virtual void        LateUpdate()                                         PURE;
    virtual void        Render(HDC _hDC)                                     PURE;
    virtual void        Release()                                            PURE;
    virtual void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   PURE;

protected:
    ITEM_TYPE m_eItemType;
};

