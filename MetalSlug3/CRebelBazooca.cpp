#include "pch.h"
#include <random>
#include "CRebelBazooca.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CPlatformChecker.h"
#include "CExplodeArea.h"
#include "CSceneManager.h"
#include "CBazooca.h"

#include "CColliderFactory.h"
#include "CScrollManager.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CSoundManager.h"
#include "CProjectileFactory.h"


CRebelBazooca::CRebelBazooca()
    : m_bDead(false), m_pAnim(nullptr), m_pPlayer(nullptr), m_pPlatformCol(nullptr)
    , m_eState(REBEL_END), m_fSpeed(0.f), m_fInvisibleDelta(0.f), m_fShootDelta(0.f)
    , m_fBeside(0.f)
{
}

CRebelBazooca::~CRebelBazooca()
{
    Release();
}

void CRebelBazooca::Initialize()
{
    LoadAnimation();

    m_pPlayer = CSceneManager::GetInstance().GetCurPlayer();

    m_iHp = 3;
    m_vSize = Vector2(192.f, 192.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(96.f, 128.f));
    m_pColBox->SetOffset(Vector2(0.f, 32.f));

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pPlatformCol->GetCollider()->SetOffset({ 0.f, m_vSize.y * 0.5f });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> iRand(256, 426);

    m_fBeside = (float)iRand(gen);
}

int CRebelBazooca::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    if (m_bDead) return OBJ_NOEVENT;
  
    __super::UpdateGameObject();

    m_vPivot.y += 400.f * DELTA;
    CheckPlatform();


    if (m_eState == DEAD) return OBJ_NOEVENT;
    if (m_fInvisibleDelta > 0.f)
        m_fInvisibleDelta -= DELTA;
    else if (m_fInvisibleDelta <= 0.f)
        m_fInvisibleDelta = 0.f;
    
    if (m_fShootDelta >= 70.f)
    {
        m_eState = SHOOT;
        return OBJ_NOEVENT;
    }

    float fDst = m_pPlayer->GetPivot().x - m_vPivot.x;

    if (fabs(fDst) <= m_fBeside && fDst > 0)
    {
        m_eState = IDLE;
        m_vFace = Vector2::UnitX * -1.f;
        m_vDirection = Vector2::Zero;
        m_fShootDelta += 40.f * DELTA;
    }
    else if (fabs(fDst) <= m_fBeside && fDst <= 0)
    {
        m_eState = IDLE;
        m_vFace = Vector2::UnitX;
        m_vDirection = Vector2::Zero;
        m_fShootDelta += 40.f * DELTA;
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

void CRebelBazooca::LateUpdate()
{
    Idle();
    Shoot();
    Chase();
    Dead();
    m_pAnim->UpdateAnimation();
}

void CRebelBazooca::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CRebelBazooca::Release()
{
    m_pPlayer = nullptr;
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
    m_pPlatformCol->SetParent(nullptr);
    m_pPlatformCol->SetDestroy();
}

void CRebelBazooca::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_fInvisibleDelta > 0.f) return;
    switch (_pCol->GetObjectType())
    {
    case PROJECTILE:
    {
        if (m_fInvisibleDelta == 0.f)
            m_fInvisibleDelta = 0.1f;
        CProjectile* pProj = dynamic_cast<CProjectile*>(_pCol);
        if (pProj->GetDamageFlag() == ENEMY)
            m_iHp -= pProj->GetDamage();
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

void CRebelBazooca::CheckPlatform()
{

    bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();

    if (bColPlatform)
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
        // m_bIsDrop = false;
        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
        //m_eCurLegState = STAND;
        //m_fJumpSpeed = n_fSeedJumpSpeed;

        return;
    }

}

void CRebelBazooca::Idle()
{
    if (m_eState != IDLE) return;

    m_pAnim->ChangeAnimation(L"Rebel_Bazooca_Idle");
    m_pAnim->SetFrameSpeed(0.1f);
}

void CRebelBazooca::Shoot()
{
    if (m_eState != SHOOT) return;

    m_pAnim->ChangeAnimation(L"Rebel_Bazooca_Shoot");
    m_pAnim->SetFrameSpeed(0.05f);
    if (m_pAnim->GetEndOneLoop())
    {
        CSoundManager::GetInstance().PlaySound(L"Enemy_Shoot.mp3", ENEMY_ATTACK, 0.2f);
        CProjectileFactory<CBazooca>
            ::CreateProjectile(m_vPivot, Vector2(m_vFace.x * -1.f, 0.f), L"Bazooca", 0);
        m_fShootDelta = 0.f;
    }
}

void CRebelBazooca::Chase()
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


void CRebelBazooca::Dead()
{
    if (m_iHp > 0)
        return;
    else if (m_pAnim->GetEndOneLoop() && m_eState == DEAD)
    {
        m_bDestroy = OBJ_DESTROY;
        return;
    }
    if (m_eState == DEAD) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> iRand(0, 3);

    switch (iRand(gen))
    {
    case 1:
        CSoundManager::GetInstance().PlaySoundOnce(L"Rebel_Dead1.mp3", ENEMY_DEAD1, 0.1f);
        break;
    case 2:
        CSoundManager::GetInstance().PlaySoundOnce(L"Rebel_Dead2.mp3", ENEMY_DEAD2, 0.1f);
        break;
    case 3:
        CSoundManager::GetInstance().PlaySoundOnce(L"Rebel_Dead3.mp3", ENEMY_DEAD3, 0.1f);
        break;
    case 4:
        CSoundManager::GetInstance().PlaySoundOnce(L"Rebel_Dead4.mp3", ENEMY_DEAD4, 0.1f);
        break;
    default:
        break;
    }

    m_eState = DEAD;
    m_bDead = true;
    m_vDirection = Vector2::Zero;
    m_pAnim->ChangeAnimation(L"Rebel_Dead");
    m_pAnim->SetLoop(false);
    SafeDelete<CCollider*>(m_pColBox);
}

void CRebelBazooca::LoadAnimation()
{
    m_pAnim = new CAnimation();

    m_pAnim->AddAnimation(L"Rebel_Dead", pair<int, int>{ 0, 12 });
    m_pAnim->AddAnimation(L"Rebel_Move", pair<int, int>{ 0, 12 });
    m_pAnim->AddAnimation(L"Rebel_Bazooca_Shoot", pair<int, int>{ 0, 5 });
    m_pAnim->AddAnimation(L"Rebel_Bazooca_Idle", pair<int, int>{ 0, 6 });

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->ChangeAnimation(L"Rebel_Bazooca_Idle");
    m_pAnim->SetFrameSpeed(0.1f);
}
