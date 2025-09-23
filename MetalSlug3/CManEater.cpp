#include "pch.h"
#include "CManEater.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CPlatformChecker.h"

#include "CColliderFactory.h"
#include "CScrollManager.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"


CManEater::CManEater()
    : m_bDead(false), m_pAnim(nullptr), m_pRefPlayer(nullptr), m_pPlatformCol(nullptr)
{
}

CManEater::~CManEater()
{
    Release();
}

void CManEater::Initialize()
{
    LoadAnimation();

    m_iHp = 30;
    m_vPivot = Vector2(800.f, 500.f);
    m_vSize = Vector2(192.f, 192.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(144.f, 144.f));
    m_pColBox->SetOffset(Vector2(0.f, -10.f));

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pPlatformCol->GetCollider()->SetOffset({ 0.f, m_pColBox->GetSize().y * 0.5f });
}

int CManEater::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    if (m_bDead) return OBJ_NOEVENT;
    m_vPivot.y += 200.f * DELTA;

    __super::UpdateGameObject();
    CheckPlatform();

    return OBJ_NOEVENT;
}

void CManEater::LateUpdate()
{
    Dead();
    m_pAnim->UpdateAnimation();
}

void CManEater::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CManEater::Release()
{
    m_pRefPlayer = nullptr;
    SafeDelete<CAnimation*>(m_pAnim);
    //SafeDelete<CGameObject*>(m_pPlatformCol);
    m_pPlatformCol->SetDestroy();
}

void CManEater::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    switch (_pCol->GetObjectType())
    {
    case PROJECTILE:
        Damage(_pCol);
        break;
    case EXPLODE:
        break;
    default:
        break;
    }
}

void CManEater::CheckPlatform()
{
    //if (m_pColBox == nullptr) return;
    bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();

    //if (!bColPlatform && !m_bIsJump && !m_bIsDrop)
    //{
    //    m_bIsDrop = true;
    //    m_fJumpSpeed = -n_fSeedJumpSpeed / 10.f;
    //}
    //if (m_bIsJump || !m_bIsDrop) return;

    if (bColPlatform) // && m_bIsDrop
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
        // m_bIsDrop = false;
        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
        //m_eCurLegState = STAND;
        //m_fJumpSpeed = n_fSeedJumpSpeed;
        return;
    }
    else if (bColPlatform == false)
    {
        //m_bIsDrop = true;
        //m_eCurBodyState == DROP;
        //m_eCurLegState == DROP;
    }
}

void CManEater::Jump()
{

}

void CManEater::ChasePlayer()
{

}

void CManEater::Attack()
{

}

void CManEater::Damage(CGameObject* _pDamageArg)
{
    m_iHp -= dynamic_cast<CProjectile*>(_pDamageArg)->GetDamage();
}

void CManEater::Dead()
{
    if (m_iHp > 0) 
        return;
    else if (m_pAnim->GetCurrentFrameIndex() == m_pAnim->GetEndFrameIndex() - 1)
    {
        m_bDestroy = OBJ_DESTROY;
        return;
    }
    
    m_bDead = true;
    m_pAnim->ChangeAnimation(L"ManEater_Dead");
    m_pAnim->SetLoop(false);
    SafeDelete<CCollider*>(m_pColBox);
}

void CManEater::LoadAnimation()
{
    m_pAnim = new CAnimation();

    m_pAnim->AddAnimation(L"ManEater_Idle", pair<int, int>{ 0, 12 });
    m_pAnim->AddAnimation(L"ManEater_Dead", pair<int, int>{ 0, 22 });

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->ChangeAnimation(L"ManEater_Idle");
    m_pAnim->SetFrameSpeed(0.1f);
}
