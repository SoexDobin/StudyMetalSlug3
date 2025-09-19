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
    : m_vSpeed({ 40.f, 40.f })
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
    int iX = static_cast<int>(CScrollManager::GetInstance().GetScrollX());
    int iY = static_cast<int>(CScrollManager::GetInstance().GetScrollY());
    
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(m_szImgKey);
    GdiTransparentBlt(_hDC
        , m_tRect.left + iX, m_tRect.top + iY
        , m_vSize.x, m_vSize.y
        , hMemDC
        , 0
        , m_vSize.y * m_iLayer
        , m_vSize.x, m_vSize.y
        , RGB(255, 255, 255));

}

void CHMProjectile::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CHMProjectile::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
    if (_pCol->GetObjectType() == ENEMY)
    {
        CParticleManager::GetInstance().CreateParticle<CBulletHitParticle>();

        m_bDestroy = OBJ_DESTROY;
        SafeDelete<CCollider*>(m_pColBox);
    }
}
