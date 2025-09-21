#pragma once
#include "CGameObject.h"

class CProjectile abstract : public CGameObject
{
public:
	CProjectile();
	virtual ~CProjectile() override;

public:
    virtual void        Initialize()                                         PURE;
    virtual int         Update()                                             PURE;
    virtual void        LateUpdate()                                         PURE;
    virtual void        Render(HDC _hDC)                                     PURE;
    virtual void        Release()                                            PURE;
    virtual void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   PURE;

protected:
    void                CheckOutOfWindow();

public:
    const TCHAR*        GetImgKey()                       { return m_szImgKey; }
    int                 GetImgLayer()                     { return m_iLayer; }
    void                SetImgKey(const TCHAR* _szImgKey) { m_szImgKey = _szImgKey; }
    void                SetImgLayer(int _iLayer)          { m_iLayer = _iLayer; }

protected:
    const TCHAR*    m_szImgKey;
    int             m_iLayer;
};

