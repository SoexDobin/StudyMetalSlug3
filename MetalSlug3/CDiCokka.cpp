#include "pch.h"
#include "CDiCokka.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CPlatformChecker.h"
#include "CExplodeArea.h"
#include "CSceneManager.h"
#include "CDummy.h"
#include "CManEaterArea.h"
#include "CParticleManager.h"
#include "CMediumExplode.h"
#include "CDiCokkaProjectile.h"

#include "CColliderFactory.h"
#include "CScrollManager.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CSoundManager.h"
#include "CBmpManager.h"
#include "CProjectileFactory.h"

CDiCokka::CDiCokka()
    :m_fInvisibleDelta(0.f), m_fShootDelta(0.f), m_fDelta(0.f)
{
}

CDiCokka::~CDiCokka()
{
    Release();
}

void CDiCokka::Initialize()
{
    m_eType = ENEMY;
    LoadAnimation();
    m_vSize = Vector2(288.f, 192.f);
    m_vDirection = Vector2::UnitX * -1.f;
    m_iHp = 60;
    m_vFace = Vector2::UnitX;

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(196.f, 192.f));

    m_eState = IDLE;

    m_pAttackCol = CGameObjectFactory<CDummy>
        ::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pAttackCol, NEUTRAL);
    m_pAttackCol->GetCollider()->SetSize(Vector2(500.f, 256.f));
    m_pAttackCol->GetCollider()
        ->SetOffset({ -256.f,  0.f});
}

int CDiCokka::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    if (m_eState == DESTROY) return OBJ_NOEVENT;

    CheckPlayer();
    m_fShootDelta -= 50 * DELTA;
    m_fInvisibleDelta -= DELTA;

	return OBJ_NOEVENT;
}

void CDiCokka::LateUpdate()
{
    Destroy();
    Idle();
    Ready();
    Attack();

    m_pAnim->UpdateAnimation();
}

void CDiCokka::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CDiCokka::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);

    m_pAttackCol->SetDestroy();
    m_pAttackCol->SetParent(nullptr);
}

void CDiCokka::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_fInvisibleDelta > 0.f) return;

    switch (_pCol->GetObjectType())
    {
    case PROJECTILE:
    {
        CProjectile* pProj = dynamic_cast<CProjectile*>(_pCol);
        if (m_eType == pProj->GetDamageFlag())
        {
            m_iHp -= pProj->GetDamage();
            m_fInvisibleDelta = DELTA;
        }
        break;
    }
        break;
    case EXPLODE:
    {
        CExplodeArea* pExplode = dynamic_cast<CExplodeArea*>(_pCol);
        m_fInvisibleDelta = DELTA;
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

void CDiCokka::Destroy()
{
    if (m_iHp > 0)
    {
        return;
    }

    if (m_eState != DESTROY)
    {
        m_eType = NEUTRAL;
        m_eState = DESTROY;
       m_pAnim->ChangeAnimation(L"Di_Cokka_Destroy");
       CParticleManager::GetInstance().CreateParticle<CMediumExplode>(m_vPivot);
    }
}

void CDiCokka::LoadAnimation()
{
    CBmpManager::GetInstance()
        .InsertBmp(L"../Resource/Bmp/Enemy/Di_Cokka/Di_Cokka_Idle.bmp"
            , L"Di_Cokka_Idle");
    CBmpManager::GetInstance()
        .InsertBmp(L"../Resource/Bmp/Enemy/Di_Cokka/Di_Cokka_Destroy.bmp"
            , L"Di_Cokka_Destroy");
    CBmpManager::GetInstance()
        .InsertBmp(L"../Resource/Bmp/Enemy/Di_Cokka/Di_Cokka_Ready.bmp"
            , L"Di_Cokka_Ready");
    CBmpManager::GetInstance()
        .InsertBmp(L"../Resource/Bmp/Enemy/Di_Cokka/Di_Cokka_Shoot.bmp"
            , L"Di_Cokka_Shoot");

    m_pAnim = new CAnimation();
    m_pAnim->AddAnimation(L"Di_Cokka_Idle", pair<int, int>{0, 2});
    m_pAnim->AddAnimation(L"Di_Cokka_Destroy", pair<int, int>{0, 1});
    m_pAnim->AddAnimation(L"Di_Cokka_Ready", pair<int, int>{0, 7});
    m_pAnim->AddAnimation(L"Di_Cokka_Shoot", pair<int, int>{0, 4});

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->SetFrameSpeed(0.2f);
}

void CDiCokka::CheckPlayer()
{
    if (m_eState == ATTACK
        || m_eState == READY) return;

    CDummy* pDst = dynamic_cast<CDummy*>(m_pAttackCol);

    if (pDst->GetColWithPlayer())
    {
        m_eState = READY;
    }
}

void CDiCokka::Idle()
{
    if (m_eState != IDLE) return;

    m_pAnim->ChangeAnimation(L"Di_Cokka_Idle");
    m_pAnim->SetLoop(true);
}

void CDiCokka::Ready() 
{
    if (m_eState != READY) return;

    if (m_pAnim->GetEndOneLoop())
    {
        m_eState = ATTACK;
        m_pAnim->ChangeAnimation(L"Di_Cokka_Shoot");
        m_pAnim->SetLoop(false);
    }

    m_pAnim->ChangeAnimation(L"Di_Cokka_Ready");
    m_pAnim->SetLoop(false);
}
void CDiCokka::Attack()
{
    if (m_eState != ATTACK) return;

    if (m_pAnim->GetEndOneLoop())
    {
        CProjectileFactory<CDiCokkaProjectile>
            ::CreateProjectile(m_vPivot + Vector2(-32.f, -16.f), Vector2(-1.f, 0.f), 0);

        m_eState = IDLE;
        m_pAnim->ChangeAnimation(L"Di_Cokka_Idle");
        m_pAnim->SetLoop(true);
        m_fShootDelta = 200.f;
        CSoundManager::GetInstance().PlaySound(L"DiCokkr.mp3", ENEMY_ATTACK, 0.5f);
    }


    m_pAnim->ChangeAnimation(L"Di_Cokka_Shoot");
    m_pAnim->SetLoop(false);
}