#pragma once
#include "CProjectile.h"

class CAnimation;

class CILProjectile : public CProjectile
{

public:
    CILProjectile();
    ~CILProjectile() override;

public:
    void Initialize() override;

    int Update() override;

    void LateUpdate() override;

    void Render(HDC _hDC) override;

    void Release() override;

    void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
    void        Accelerate();
    void		CheckPlatform();
    void		LoadBmp();

private:
    CAnimation*     m_pAnim;
    CGameObject*    m_pPlatformCol;

    bool            m_fDropEnd;
    float			m_fDropSpeed;
    float			m_fDropDelta;
};

