#include "pch.h"
#include <random>
#include "CRebel.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CPlatformChecker.h"
#include "CExplodeArea.h"
#include "CSceneManager.h"

#include "CColliderFactory.h"
#include "CScrollManager.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CSoundManager.h"


CRebel::CRebel()
    : m_bDead(false), m_pAnim(nullptr), m_pPlayer(nullptr), m_pPlatformCol(nullptr), m_eState(REBEL_END), m_fSpeed(0.f), m_fInvisibleDelta(0.f)
{
}

CRebel::~CRebel()
{
    Release();
}

void CRebel::Initialize()
{
    LoadAnimation();

    m_pPlayer = CSceneManager::GetInstance().GetCurPlayer();

    m_iHp = 2;
    m_vSize = Vector2(192.f, 192.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(96.f, 128.f));
    m_pColBox->SetOffset(Vector2(0.f, 32.f));

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pPlatformCol->GetCollider()->SetOffset({ 0.f, m_vSize.y * 0.5f });
}

int CRebel::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    if (m_bDead) return OBJ_NOEVENT;
    m_vPivot.y += 200.f * DELTA;

    __super::UpdateGameObject();
    CheckPlatform();


    if (m_eState == DEAD) return OBJ_NOEVENT;
    if (m_fInvisibleDelta > 0.f)
        m_fInvisibleDelta -= DELTA;
    else if (m_fInvisibleDelta <= 0.f)
        m_fInvisibleDelta = 0.f;

    float fDst = m_pPlayer->GetPivot().x - m_vPivot.x;
    if (fabsf(fDst) <= 64.f)
    {
        m_eState = IDLE;
        m_vDirection = Vector2::Zero;
    }
    else
    {
        m_eState = CHASE;
        if (fDst < 0.f)
        {
            m_vFace = Vector2::UnitX;
            m_vDirection = Vector2::UnitX * -1.f;
        }
        else
        {
            m_vFace = Vector2::UnitX * -1.f;
            m_vDirection = Vector2::UnitX;
        }
    }

    return OBJ_NOEVENT;
}

void CRebel::LateUpdate()
{
    Idle();
    Chase();
    Dead();
    m_pAnim->UpdateAnimation();
}

void CRebel::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CRebel::Release()
{
    m_pPlayer = nullptr;
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
    m_pPlatformCol->SetParent(nullptr);
    m_pPlatformCol->SetDestroy();
}

void CRebel::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_fInvisibleDelta > 0.f) return;
    switch (_pCol->GetObjectType())
    {
    case PROJECTILE:
    {
        if (m_fInvisibleDelta == 0.f)
            m_fInvisibleDelta = DELTA;
        CProjectile* pProj = dynamic_cast<CProjectile*>(_pCol);
        if (pProj->GetDamageFlag() == ENEMY)
        {
            m_iHp -= pProj->GetDamage();

        }
        break;
    }   
        break;
    case EXPLODE:
    {
        CExplodeArea* pExplode = dynamic_cast<CExplodeArea*>(_pCol);
        if (m_eType == pExplode->GetDamageFlag())
        {
            m_iHp -= pExplode->GetDamage();
            break;
        }
    }
    break;
    default:
        break;
    }
}

void CRebel::CheckPlatform()
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

void CRebel::Idle()
{
    if (m_eState != IDLE) return;

    m_pAnim->ChangeAnimation(L"Rebel_Idle");
    m_pAnim->SetFrameSpeed(0.1f);
}

void CRebel::Chase()
{
    if (m_eState != CHASE) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> iMul(200, 400);

    m_fSpeed = static_cast<float>(iMul(gen));

    m_pAnim->ChangeAnimation(L"Rebel_Move");
    m_pAnim->SetFrameSpeed(0.07f);
    m_vPivot += m_vDirection * DELTA * m_fSpeed;
}


void CRebel::Dead()
{
    if (m_iHp > 0)
        return;
    else if (m_pAnim->GetEndOneLoop() && m_eState == DEAD)
    {
        m_bDestroy = OBJ_DESTROY;
        return;
    }
    if (m_eState == DEAD) return;
    CSoundManager::GetInstance().PlaySoundOnce(L"Rebel_Dead.mp3", ENEMY_DEAD, 0.1f);
    m_eState = DEAD;
    m_bDead = true;
    m_vDirection = Vector2::Zero;
    m_pAnim->ChangeAnimation(L"Rebel_Dead");
    m_pAnim->SetLoop(false);
    SafeDelete<CCollider*>(m_pColBox);
}

void CRebel::LoadAnimation()
{
    m_pAnim = new CAnimation();

    m_pAnim->AddAnimation(L"Rebel_Idle", pair<int, int>{ 0, 4 });
    m_pAnim->AddAnimation(L"Rebel_Dead", pair<int, int>{ 0, 12 });
    m_pAnim->AddAnimation(L"Rebel_Move", pair<int, int>{ 0, 12 });

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->ChangeAnimation(L"Rebel_Idle");
    m_pAnim->SetFrameSpeed(0.1f);
}
