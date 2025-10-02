#pragma once
#include "CGameObject.h"

class CProjectile : public CGameObject
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

    Damage_TYPE         GetDamageType() const { return m_eDamageType; }
    void                SetDamageType(Damage_TYPE _eType) { m_eDamageType = _eType; }

protected:
    void                CheckOutOfWindow();

public:
    int                 GetDamage()                         { return m_iDamage; }
    const TCHAR*        GetImgKey()                         { return m_szImgKey; }
    int                 GetImgLayer()                       { return m_iLayer; }
    float               GetSpeed()                          { return m_fSpeed; }
    OBJECT_TYPE         GetDamageFlag()                     { return m_eDamageFlag; }

    void                SetDamage(const int _iDamage)           { m_iDamage = _iDamage; }
    void                SetImgKey(const TCHAR* _szImgKey)       { m_szImgKey = _szImgKey; }
    void                SetImgLayer(int _iLayer)                { m_iLayer = _iLayer; }
    void                SetSpeed(const float& _fSpeed)          { m_fSpeed = _fSpeed; }
    void                SetDamageFlag(OBJECT_TYPE _eDamageFlag) { m_eDamageFlag = _eDamageFlag; }
protected:
    const TCHAR*    m_szImgKey;
    int             m_iLayer;
    int             m_iDamage;
    float			m_fSpeed;
    OBJECT_TYPE     m_eDamageFlag;
    Damage_TYPE     m_eDamageType;
};

