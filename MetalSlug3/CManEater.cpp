#include "pch.h"
#include "CManEater.h"
#include "CHitBox.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CPlatformChecker.h"
#include "CExplodeArea.h"
#include "CSceneManager.h"
#include "CDummy.h"
#include "CManEaterArea.h"
#include <random>
#

#include "CColliderFactory.h"
#include "CScrollManager.h"
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CSoundManager.h"
#include "CDiCokka.h"


CManEater::CManEater()
    : m_bDead(false), m_pAnim(nullptr), m_pPlayer(nullptr)
    , m_pPlatformCol(nullptr), m_pForwardCol(nullptr), m_pUpperCol(nullptr), m_pLowerCol(nullptr)
    , m_pAttackCol(nullptr) 
    , m_pAttackAnim(nullptr), m_pAttackAnimObj(nullptr)
    , m_fInvisibleDelta(0.f), m_fStateDelta(0.f), m_fMoveDelta(0.f), m_fJumpDelta(0.f), m_fDropDelta(0.f)
{
}

CManEater::~CManEater()
{
    Release();
}

void CManEater::Initialize()
{
    LoadAnimation();
    m_pPlayer = CSceneManager::GetInstance().GetCurPlayer();

    m_iHp = 7;
    m_vPivot = Vector2(800.f, 500.f);
    m_vSize = Vector2(192.f, 192.f);
    m_vFace = Vector2::UnitX;

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(96.f, 144.f));
    m_pColBox->SetOffset(Vector2(0.f, -10.f));

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>
        ::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pPlatformCol->GetCollider()->SetOffset({ 0.f, m_pColBox->GetSize().y * 0.5f });

    m_pForwardCol = CGameObjectFactory<CPlatformChecker>
        ::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pForwardCol, NEUTRAL);
    m_pForwardCol->GetCollider()
        ->SetOffset({ m_pColBox->GetSize().x * -0.5f, 0.f });

    m_pUpperCol = CGameObjectFactory<CPlatformChecker>
        ::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pUpperCol, NEUTRAL);
    m_pUpperCol->GetCollider()
        ->SetOffset({ 0.f, m_pColBox->GetSize().y * 0.5f });
    m_pUpperCol->GetCollider()->SetSize(Vector2(64.f,128.f));

    m_pLowerCol = CGameObjectFactory<CPlatformChecker>
        ::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pLowerCol, NEUTRAL);
    m_pUpperCol->GetCollider()
        ->SetOffset({ 0.f, m_pColBox->GetSize().y * 0.5f });
    m_pLowerCol->GetCollider()->SetSize(Vector2(256.f, 128.f));

    m_pAttackCol = CGameObjectFactory<CDummy>
        ::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pAttackCol, NEUTRAL);
    m_pAttackCol->GetCollider()->SetSize(Vector2(128.f, 128.f));
    m_pAttackCol->GetCollider()
        ->SetOffset({ 0.f,  m_vSize.y * 3.f });
    
    m_fJumpDelta = n_fSeedJumpSpeed;
    m_fStateDelta = 100.f;
}

int CManEater::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    // TODO : ¹Ù²ã
    if (m_eCurState != DROP)
        m_vPivot.y += 300.f * DELTA;

    if (m_bDead)
    {
        CheckPlatform();
        return OBJ_NOEVENT;
    }


    m_pAttackAnimObj->Update();
    m_pAttackAnimObj->SetPivot(m_vPivot);
    m_pForwardCol->GetCollider()
        ->SetOffset({ m_pColBox->GetSize().x * 0.5f * m_vFace.x * -1.f, m_pColBox->GetSize().y * 0.5f });
    m_pAttackCol->GetCollider()
        ->SetOffset({ m_vSize.x * m_vFace.x * -1.f, 0.f });
    m_pUpperCol->GetCollider()
        ->SetOffset({ 0.f, m_vSize.y * -1.f });
    m_pLowerCol->GetCollider()
        ->SetOffset({ 0.f, m_vSize.y * 3.f });

    if (m_fDropDelta > 0.f)
        m_fDropDelta -= 150.f * DELTA;
    else if (m_fDropDelta <= 0.f)
        m_fDropDelta = 0.f;

    if (m_fStateDelta > 0.f)
        m_fStateDelta -= 100.f * DELTA;
    else if (m_fStateDelta <= 0.f)
        m_fStateDelta = 0.f;
    
    if (m_fInvisibleDelta > 0.f)
        m_fInvisibleDelta -= DELTA;
    else if (m_fInvisibleDelta <= 0.f)
        m_fInvisibleDelta = 0.f;



    if (CheckDropCol()) return OBJ_NOEVENT;
    CheckPlatform();

    if (m_fStateDelta > 0.f) return OBJ_NOEVENT;

    CheckAttack();
    CheckBehaviour();

    return OBJ_NOEVENT;
}

void CManEater::LateUpdate()
{
    
    Idle();
    Dead();
    Jump();
    Drop();
    Attack();
    Chase();

    m_pAttackAnimObj->LateUpdate();
    m_pAnim->UpdateAnimation();
    m_pAttackAnim->UpdateAnimation();
    m_ePrevState = m_eCurState;
}

void CManEater::Render(HDC _hDC)
{
    m_pAttackAnimObj->Render(_hDC);
    m_pAnim->RenderAnimation(_hDC);
    m_pAttackAnim->RenderAnimation(_hDC);
}

void CManEater::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CAnimation*>(m_pAttackAnim);
    SafeDelete<CCollider*>(m_pColBox);
    SafeDelete<CGameObject*>(m_pAttackAnimObj);

    m_pPlatformCol->SetParent(nullptr);
    m_pForwardCol->SetParent(nullptr);
    m_pAttackCol->SetParent(nullptr);
    m_pUpperCol->SetParent(nullptr);
    m_pLowerCol->SetParent(nullptr);


    m_pPlatformCol->SetDestroy();
    m_pForwardCol->SetDestroy();
    m_pAttackCol->SetDestroy();
    m_pUpperCol->SetDestroy();
    m_pLowerCol->SetDestroy();
}

void CManEater::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_fInvisibleDelta > 0.f) return;

    switch (_pCol->GetObjectType())
    {
    case PROJECTILE:
        Damage(_pCol);
        m_fInvisibleDelta = DELTA;
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

void CManEater::CheckAttack()
{
    if (m_eCurState == ATTACK
        || m_eCurState == DROP
        || m_eCurState == JUMP)
        return;

    CDummy* pDst = dynamic_cast<CDummy*>(m_pAttackCol);
    
    if (pDst->GetColWithPlayer())
    {
        m_eCurState = ATTACK;
        m_vDirection = Vector2::Zero;
    }
}

void CManEater::CheckBehaviour()
{
    if (m_eCurState == ATTACK 
        || m_eCurState == DROP 
        || m_eCurState == JUMP) 
        return;

    CPlatformChecker* pFrontCol = dynamic_cast<CPlatformChecker*>(m_pForwardCol);
    CPlatformChecker* pUpperCol = dynamic_cast<CPlatformChecker*>(m_pUpperCol);
    float fDstX = m_pPlayer->GetPivot().x - m_vPivot.x;
    float fDstY = m_pPlayer->GetPivot().y - m_vPivot.y;


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> iRand(0, 1);

    if (pUpperCol->GetHasColWithPlatform() && fDstY + 90.f < 0.f && iRand(gen) > 0)
    {
        m_eCurState = JUMP;

        if (fDstX < 0.f)
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
    else if (pFrontCol->GetHasColWithPlatform() || (!pFrontCol->GetHasColWithPlatform() && fDstY > 0.f))
    {
        m_eCurState = MOVE;

        if (fDstX < 0.f)
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
    else if (!pFrontCol->GetHasColWithPlatform())
    {
        m_eCurState = IDLE;
        m_fStateDelta = 200.f;
        m_vDirection = Vector2::Zero;

        if (fDstX < 0.f)
            m_vFace = Vector2::UnitX;
        else
            m_vFace = Vector2::UnitX * -1.f;
    }

}

bool CManEater::CheckDropCol()
{
    if (m_eCurState == ATTACK
        || m_eCurState == DROP
        || m_eCurState == JUMP)
        return false;

    CPlatformChecker* pLowerCol = dynamic_cast<CPlatformChecker*>(m_pLowerCol);

    float fDstX = fabs(m_pPlayer->GetPivot().x - m_vPivot.x);
    float fDstY = m_pPlayer->GetPivot().y - m_vPivot.y;

    if (fDstY > 0.f && pLowerCol->GetHasColWithPlatform())
    {
        m_fJumpDelta = -n_fSeedJumpSpeed / 10.f;
        SetAnim(L"ManEater_Jump&Drop", false, DROP);
        SetAttackAnim(L"ManEater_Attack_Blank", true, DROP);
        m_fDropDelta = 100.f;
        return true;
    }

    return false;
}

void CManEater::CheckPlatform()
{
    if (m_fDropDelta > 0.f) return;
    bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();

    //if (!bColPlatform && !m_bIsJump && !m_bIsDrop)
    //{
    //    m_bIsDrop = true;
    //    m_fJumpSpeed = -n_fSeedJumpSpeed / 10.f;
    //}
    if (m_eCurState == JUMP) return;

    if (bColPlatform && m_ePrevState == DROP) 
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();

        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
        if (m_eCurState != ATTACK && m_eCurState != DEAD)
            m_eCurState = IDLE;
        m_fStateDelta = 150.f;
        m_vDirection = Vector2::Zero;
        m_fJumpDelta = n_fSeedJumpSpeed;
        return;
    }
    else if (bColPlatform)
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;
        if (m_eCurState != ATTACK && m_eCurState != DEAD)
            m_eCurState = IDLE;
        m_vDirection = Vector2::Zero;
    }
}


void CManEater::Idle()
{
    if (m_eCurState != IDLE) return;

    SetAnim(L"ManEater_Idle", true, IDLE);
    SetAttackAnim(L"ManEater_Attack_Blank", false, IDLE);
}

void CManEater::Jump()
{
    if (m_eCurState != JUMP) return;

    m_fJumpDelta += DELTA * n_fFallSpeed;
    m_vPivot.y += m_fJumpDelta * DELTA;
    SetAnim(L"ManEater_Jump&Drop", false, JUMP);
    SetAttackAnim(L"ManEater_Attack_Blank", false, JUMP);

    if (m_fJumpDelta > 0.f)
    {
        m_eCurState = DROP;
    }
}

void CManEater::Drop()
{
    if (m_eCurState != DROP) return;

    m_fJumpDelta += DELTA * n_fFallSpeed;

    if (m_fJumpDelta > n_fMaxFall)
        m_fJumpDelta = n_fMaxFall;

    m_vPivot.y += m_fJumpDelta * DELTA;
}

void CManEater::Chase()
{
    if (m_eCurState != MOVE) return;
    if (m_fMoveDelta >= 100.f)
    {
        m_eCurState = IDLE;
        m_fStateDelta = 100.f;
        m_fMoveDelta = 0.f;
        m_vDirection = Vector2::Zero;

        SetAnim(L"ManEater_Idle", true, IDLE);
        SetAttackAnim(L"ManEater_Attack_Blank", false, IDLE);
    }


    SetAnim(L"ManEater_Move", true, MOVE);
    SetAttackAnim(L"ManEater_Attack_Blank", true, MOVE);
    m_pAnim->SetFrameSpeed(0.07f);
    m_vPivot += m_vDirection * DELTA * 180.f;

    float fDstX = fabsf(m_pPlayer->GetPivot().x - m_vPivot.x);
    float fDstY = fabsf(m_pPlayer->GetPivot().y - m_vPivot.y);

    m_fMoveDelta += 50 * DELTA;
}

void CManEater::Attack()
{
    if (m_eCurState != ATTACK) return;

    if ((m_pAttackAnim->GetCurrentFrameKey() == L"ManEater_Attack_Right"
        || m_pAttackAnim->GetCurrentFrameKey() == L"ManEater_Attack_Left")
        && m_pAttackAnim->GetEndOneLoop())
    {
        SetAnim(L"ManEater_Idle", true, IDLE);
        SetAttackAnim(L"ManEater_Attack_Blank", false, IDLE);
        Vector2 vPos = m_pAttackCol->GetPivot() + m_pAttackCol->GetCollider()->GetOffset();
        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CManEaterArea>::Create(vPos, Vector2(96.f, 96.f)), EXPLODE);
        m_fStateDelta = 200.f;
        return;
    }

    SetAnim(L"ManEater_Blank", true, ATTACK);
    if (m_vFace.x < 0.f)
        SetAttackAnim(L"ManEater_Attack_Right", false, ATTACK);
    else
        SetAttackAnim(L"ManEater_Attack_Left", false, ATTACK);

    if (m_ePrevState != ATTACK)
        CSoundManager::GetInstance().PlaySound(L"ManEater_Attack.mp3", ENEMY_ATTACK, 0.2f);
}
void CManEater::Damage(CGameObject* _pDamageArg)
{
    CProjectile* pProj = dynamic_cast<CProjectile*>(_pDamageArg);
    if (pProj->GetDamageFlag() == m_eType)
    {
        m_iHp -= pProj->GetDamage();
        m_eLastHit = pProj->GetDamageType();
    }
}

void CManEater::Dead()
{
    if (m_iHp > 0) 
        return;
    else if (m_bDead && m_pAnim->GetEndOneLoop())
    {
        m_bDestroy = OBJ_DESTROY;
        return;
    }
    
    if (m_eCurState == DEAD) return;

    CSoundManager::GetInstance().PlaySound(L"ManEater_Dead.mp3", ENEMY_DEAD1, 0.2f);

    m_bDead = true;
    m_eCurState = DEAD;
    if (m_eLastHit == FIRE)
        m_pAnim->ChangeAnimation(L"ManEater_Burn");
    else
        m_pAnim->ChangeAnimation(L"ManEater_Dead");
    m_pAttackAnim->ChangeAnimation(L"ManEater_Attack_Blank");
    m_pAnim->SetLoop(false);
    m_pColBox->SetEnableCol(false);
}

void CManEater::LoadAnimation()
{
    m_pAttackAnimObj = new CDummy();
    m_pAttackAnimObj->SetSize(Vector2(576.f, 192.f));

    m_pAnim = new CAnimation();
    m_pAttackAnim = new CAnimation();

    m_pAnim->AddAnimation(L"ManEater_Idle", pair<int, int>{ 0, 12 });
    m_pAnim->AddAnimation(L"ManEater_Dead", pair<int, int>{ 0, 22 });
    m_pAnim->AddAnimation(L"ManEater_Burn", pair<int, int>{ 0, 24 });
    m_pAnim->AddAnimation(L"ManEater_Move", pair<int, int>{ 0, 15 });
    m_pAnim->AddAnimation(L"ManEater_Jump&Drop", pair<int, int>{ 0, 9 });
    m_pAnim->AddAnimation(L"ManEater_Blank", pair<int, int>{ 0, 1 });
    
    m_pAttackAnim->AddAnimation(L"ManEater_Attack_Blank", pair<int, int>{ 0, 1 });
    m_pAttackAnim->AddAnimation(L"ManEater_Attack_Left", pair<int, int>{ 0, 11 });
    m_pAttackAnim->AddAnimation(L"ManEater_Attack_Right", pair<int, int>{ 0, 11 });

    m_pAnim->Initialize();
    m_pAttackAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAttackAnim->SetParent(m_pAttackAnimObj);

    m_pAnim->ChangeAnimation(L"ManEater_Idle");
    m_pAttackAnim->ChangeAnimation(L"ManEater_Attack_Blank");
    m_eCurState = IDLE;
    m_pAnim->SetFrameSpeed(0.1f);
    m_pAttackAnim->SetFrameSpeed(0.05f);
}

inline void CManEater::SetAnim(const TCHAR* _szKey, bool _bIsRepeat, MANEATER_STATE _eState, int _iLastFacingX)
{
    m_pAnim->ChangeAnimation(_szKey);
    m_pAnim->SetLoop(_bIsRepeat);
    m_eCurState = _eState;

    if (_iLastFacingX == 0) return;
    m_pAnim->SetLastFacingX(_iLastFacingX);
}

inline void CManEater::SetAttackAnim(const TCHAR* _szKey, bool _bIsRepeat, MANEATER_STATE _eState, int _iLastFacingX)
{
    m_pAttackAnim->ChangeAnimation(_szKey);
    m_pAttackAnim->SetLoop(_bIsRepeat);
    m_eCurState = _eState;

    if (_iLastFacingX == 0) return;
    m_pAttackAnim->SetLastFacingX(_iLastFacingX);
}
