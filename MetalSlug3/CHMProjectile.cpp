#include "pch.h"
#include "CHMProjectile.h"
#include "CBulletHitParticle.h"
#include "CPlatform.h"

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
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize( m_vSize/ 3.f );
    m_iDamage = 1;
    
    __super::UpdateGameObject();
}

int CHMProjectile::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_vPivot += m_vSpeed * m_vDirection * DELTA;

    return OBJ_NOEVENT;
}

void CHMProjectile::LateUpdate()
{
    __super::CheckOutOfWindow();
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

void CHMProjectile::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_bDestroy) return;

    bool pPassThroughPlat = false;
    if (_pCol->GetObjectType() == PLATFORM)
        pPassThroughPlat = dynamic_cast<CPlatform*>(_pCol)->GetProjectilePass();

    if (_pCol->GetObjectType() == ENEMY || !pPassThroughPlat)
    {
        Vector2 vParticleOffset{ 0.f, 0.f };

        switch (_eFlag)
        {
        case LEFT_COL:  vParticleOffset = Vector2(-m_vSize.x / 2.f, 0.f);
            break;
        case RIGHT_COL: vParticleOffset = Vector2(m_vSize.x / 2.f, 0.f);
            break;
        case UP_COL:    vParticleOffset = Vector2(0.f, -m_vSize.y / 2.f);
            break;
        case DOWN_COL:  vParticleOffset = Vector2(0.f, m_vSize.y / 2.f);
            break;
        default:
            break;
        }

        CParticleManager::GetInstance().CreateParticle<CBulletHitParticle>(m_vPivot + vParticleOffset);
        m_bDestroy = OBJ_DESTROY;
        SafeDelete<CCollider*>(m_pColBox);
    }
}
