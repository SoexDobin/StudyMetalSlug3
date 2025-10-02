#include "pch.h"
#include "CILProjectile.h"
#include "CAnimation.h"
#include "CPlatformChecker.h"
#include "CBombExplode.h"

#include "CBmpManager.h"
#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CScoreManager.h"
#include "CSoundManager.h"

CILProjectile::CILProjectile()
    : m_pAnim(nullptr), m_pPlatformCol(nullptr)
    , m_fDropSpeed(0.f), m_fDropDelta(0.f), m_fDropEnd(false)
{
}

CILProjectile::~CILProjectile()
{
    Release();
}

void CILProjectile::Initialize()
{
    LoadBmp();
    m_fSpeed = 50.f;
    m_vSize = Vector2(192.f, 144.f);
    m_iDamage = 2;
    m_eType = PROJECTILE;
    m_eDamageFlag = ENEMY;

    m_fDropSpeed = -300.f;
    m_fDropDelta = 1500.f;

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize / 3.f);

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    __super::UpdateGameObject();
}

int CILProjectile::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    Accelerate();

    if (m_vDirection.x < 0)
        m_vFace = Vector2::UnitX * -1.f;
    else
        m_vFace = Vector2::UnitX;

    m_vPivot += m_vDirection * m_fSpeed * DELTA;
    CheckPlatform();

    return OBJ_NOEVENT;
}

void CILProjectile::LateUpdate()
{
    __super::CheckOutOfWindow();
    m_pAnim->UpdateAnimation();
}

void CILProjectile::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CILProjectile::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
    m_pPlatformCol->SetParent(nullptr);
    m_pPlatformCol->SetDestroy();
}

void CILProjectile::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == ENEMY)
    {
        m_bDestroy = OBJ_DESTROY;
        m_pColBox->SetEnableCol(false);
        SafeDelete<CCollider*>(m_pColBox);
        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CBombExplode>
            ::Create(Vector2(m_vPivot) + Vector2(m_vFace.x * 100.f, -50.f), Vector2(100.f, 200.f)), EXPLODE);
        CScoreManager::GetInstance().ACCScore(100);
    }
}

void CILProjectile::Accelerate()
{
    if (!m_fDropEnd) return;

    m_fSpeed += 10.f * m_fSpeed * DELTA;
}

void CILProjectile::CheckPlatform()
{
    if (m_fDropEnd) return;
    bool bIsPlt = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();

    m_fDropSpeed += DELTA * m_fDropDelta;
    m_vPivot.y += m_fDropSpeed * DELTA;

    if (bIsPlt)
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
        m_fDropEnd = true;
        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
        m_fDropSpeed = 0.f;
        m_pAnim->ChangeAnimation(L"Iron_Lizzard_Attack");
        m_pAnim->SetLoop(true);
        CSoundManager::GetInstance().PlaySound(L"IronLizzard.mp3", PLAYER_ATTACK, 0.2f);
        return;
    }
}

void CILProjectile::LoadBmp()
{
    m_pAnim = new CAnimation();
    m_pAnim->AddAnimation(L"Iron_Lizzard_Attack", pair<int, int>{0, 6});
    m_pAnim->AddAnimation(L"Iron_Lizzard_Shoot", pair<int, int>{0, 1});
    m_pAnim->ChangeAnimation(L"Iron_Lizzard_Shoot");
    m_pAnim->SetLoop(true);
    
    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
}
