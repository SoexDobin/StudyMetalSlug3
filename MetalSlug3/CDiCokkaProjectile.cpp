#include "pch.h"
#include "CDiCokkaProjectile.h"
#include "CAnimation.h"

#include "CColliderFactory.h"
#include "CTimeManager.h"
#include "CParticleManager.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CScoreManager.h"

CDiCokkaProjectile::CDiCokkaProjectile()
    : m_iLayerX(0), m_fDelta(0.f)
{
}

CDiCokkaProjectile::~CDiCokkaProjectile()
{
}

void CDiCokkaProjectile::Initialize()
{

    m_vSize = Vector2(192.f, 96.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize / 2.f);
    m_iDamage = 1;
    m_fSpeed = 400.f;
    m_eType = PROJECTILE;
    m_eDamageFlag = PLAYER;
    m_eDamageType = DANMAK;
    m_vFace = Vector2::UnitX;
    __super::UpdateGameObject();
}

int CDiCokkaProjectile::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_fDelta += 100.f * DELTA;
    m_vPivot += m_vDirection * m_fSpeed * DELTA;

    return OBJ_NOEVENT;
}

void CDiCokkaProjectile::LateUpdate()
{
    __super::CheckOutOfWindow();
    
    if (m_fDelta >= 10.f)
    {
        m_fDelta = 0.f;
        m_iLayerX = (m_iLayerX + 1) % 4;
    }

}

void CDiCokkaProjectile::Render(HDC _hDC)
{
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Di_Cokka_Shoot_Projectile");

    int iSizeX = static_cast<int>(m_vSize.x);
    int iSizeY = static_cast<int>(m_vSize.y);

    GdiTransparentBlt(_hDC
        , m_tRect.left + SCROLLX, m_tRect.top + SCROLLY
        , iSizeX, iSizeY
        , hMemDC
        , m_iLayerX * m_vSize.x
        , 0
        , iSizeX, iSizeY
        , RGB(255, 255, 255));
}

void CDiCokkaProjectile::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CDiCokkaProjectile::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_bDestroy) return;

    if (_pCol->GetObjectType() == PLAYER)
    {
        m_bDestroy = OBJ_DESTROY;
        SafeDelete<CCollider*>(m_pColBox);
    }
}
