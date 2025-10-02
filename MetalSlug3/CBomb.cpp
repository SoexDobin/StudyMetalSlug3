#include "pch.h"
#include "CBomb.h"
#include "CAnimation.h"
#include "CBmpManager.h"
#include "CPlatformChecker.h"
#include "CBombExplode.h"

#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CScoreManager.h"

CBomb::CBomb()
    : m_pAnim(nullptr), m_pPlatformCol(nullptr)
    , m_fDelta(0.f), m_fDropSpeed(0.f), m_fDropDelta(0.f)
{
}

CBomb::~CBomb()
{
    Release();
}

void CBomb::Initialize()
{
    LoadBmp();
    m_vSize = Vector2(96.f, 96.f);
    m_vDirection = Vector2::UnitX;
    m_vFace = Vector2::UnitX;

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize / 2.f);
    m_iDamage = 5;
    m_fSpeed = 400.f;
    m_eType = PROJECTILE;
    m_eDamageFlag = ENEMY;
    m_eDamageType = DANMAK;

    m_fDropSpeed = -200.f;
    m_fDropDelta = 1500.f;

    m_iBounds = 3;

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pAnim->ChangeAnimation(L"Bomb");
    m_vDirection = Vector2(1.f, 0.f).Rotate(m_fAngle);

    __super::UpdateGameObject();
}

int CBomb::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_fDelta += DELTA;
    m_vPivot += m_vDirection * m_fSpeed * DELTA;
    CheckPlatform();

    return OBJ_NOEVENT;
}

void CBomb::LateUpdate()
{
    __super::CheckOutOfWindow();
    m_pAnim->UpdateAnimation();
}

void CBomb::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CBomb::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
    m_pPlatformCol->SetParent(nullptr);
    m_pPlatformCol->SetDestroy();
}

void CBomb::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == ENEMY)
    {
        m_bDestroy = OBJ_DESTROY;
        m_pColBox->SetEnableCol(false);
        SafeDelete<CCollider*>(m_pColBox);
        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CBombExplode>
            ::Create(Vector2(m_vPivot) + Vector2(0.f, -50.f), Vector2(100.f, 200.f)), EXPLODE);
        CScoreManager::GetInstance().ACCScore(100);
    }
}

void CBomb::CheckPlatform()
{
    
    bool bIsPlt= dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();

    m_fDropSpeed += DELTA * m_fDropDelta;
    m_vPivot.y += m_fDropSpeed * DELTA;

    if (bIsPlt && m_iBounds > 0)
    {
        m_fDropSpeed = -200.f;
        
    }
    else if (m_fDelta >= 2.f)
    {
        m_bDestroy = OBJ_DESTROY;
        m_pColBox->SetEnableCol(false);
        SafeDelete<CCollider*>(m_pColBox);
        CGameObject* pExplode = CGameObjectFactory<CBombExplode>
            ::Create(Vector2(m_vPivot + Vector2(0.f, -50.f)), Vector2(100.f, 200.f));

        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CBombExplode>
            ::Create(Vector2(m_vPivot) + Vector2(0.f, -50.f), Vector2(100.f, 200.f)), EXPLODE);
    }
}

void CBomb::LoadBmp()
{
    m_pAnim = new CAnimation();
    m_pAnim->SetLockFacing(true);
    m_pAnim->SetFrameSpeed(0.5f);
    m_pAnim->AddAnimation(L"Bomb", pair<int, int>{ 0, 16 });
    m_pAnim->SetLoop(false);

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
}
