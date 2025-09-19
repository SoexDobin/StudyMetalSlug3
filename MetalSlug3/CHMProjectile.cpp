#include "pch.h"
#include "CHMProjectile.h"
#include "CHitBox.h"
#include "CBulletHitParticle.h"

#include "CColliderFactory.h"
#include "CTimeManager.h"
#include "CParticleManager.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CHMProjectile::CHMProjectile()
    : m_vSpeed({ 1000.f, 1000.f })
{
}

CHMProjectile::~CHMProjectile()
{
    Release();
}

void CHMProjectile::Initialize()
{
    m_vSize = Vector2( 96.f, 96.f);
    m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);

}

int CHMProjectile::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_vPivot += m_vDirection * (m_vSpeed * CTimeManager::GetInstance().GetDeltaTime());

    return OBJ_NOEVENT;
}

void CHMProjectile::LateUpdate()
{

}

void CHMProjectile::Render(HDC _hDC)
{   
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(m_szImgKey);

    int iSizeX = static_cast<int>(m_vSize.x);
    int iSizeY = static_cast<int>(m_vSize.y);

    GdiTransparentBlt(_hDC
        , m_tRect.left + SCROLLX, m_tRect.top + SCROLLY
        , iSizeX, iSizeY
        , hMemDC
        , 0
        , iSizeY * m_iLayer
        , iSizeX, iSizeY
        , RGB(255, 255, 255));

}

void CHMProjectile::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CHMProjectile::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
    if (_pCol->GetObjectType() == ENEMY && m_bDestroy == false)
    {
        CParticleManager::GetInstance().CreateParticle<CBulletHitParticle>(m_vPivot);

        m_bDestroy = OBJ_DESTROY;
        m_eType = OBJECT_END;
    }
}
