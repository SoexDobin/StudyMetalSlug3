#include "pch.h"
#include "CBazooca.h"

#include "CBulletHitParticle.h"
#include "CPlatform.h"

#include "CColliderFactory.h"
#include "CTimeManager.h"
#include "CParticleManager.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CScoreManager.h"

CBazooca::CBazooca()
    : m_iLayerX(0), m_fDelta(0.f)
{
}

CBazooca::~CBazooca()
{
}

void CBazooca::Initialize()
{
    m_vSize = Vector2(48.f, 48.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize / 2.f);
    m_iDamage = 1;
    m_fSpeed = 500.f;
    m_eType = PROJECTILE;
    m_eDamageFlag = PLAYER;

    __super::UpdateGameObject();
}

int CBazooca::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_fDelta += 20.f * DELTA;
    m_vPivot += m_vDirection * m_fSpeed * DELTA;

    return OBJ_NOEVENT;
}

void CBazooca::LateUpdate()
{
    if (m_fDelta >= 100.f)
    {
        m_fDelta = 0.f;
        ++m_iLayerX;
    }


    __super::CheckOutOfWindow();
}

void CBazooca::Render(HDC _hDC)
{
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(m_szImgKey);

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

void CBazooca::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CBazooca::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_bDestroy) return;

    
   if (_pCol->GetObjectType() == PLAYER)
    {
        Hit(_eFlag);
    }
}

void CBazooca::Hit(COLLISION_COL_FLAG _eFlag)
{
    m_bDestroy = OBJ_DESTROY;
    SafeDelete<CCollider*>(m_pColBox);
}
