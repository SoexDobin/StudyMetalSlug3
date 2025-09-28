#include "pch.h"
#include <random>
#include "CSolDaeLokker.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CYellowCorn.h"

#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CProjectileFactory.h"
#include "CTimerUI.h"

CSolDaeLokker::CSolDaeLokker()
    : m_pPlayer(nullptr)
    , m_pBodyAnim(nullptr), m_pJewelAnim(nullptr), m_pAttackAnim(nullptr)
    , m_fDelta(0.f), m_fIntroDelta(0.f), m_fDeadDelta(0.f), m_fMoveDelta(0.f), m_fCornDelta(0.f), m_fCornAccDelta(0.f)
    , m_fSDWDelta(0.f)
    , m_eCurState(SDLK_END), m_eCurPattern(NONE)
{
    ZeroMemory(&m_vMove, sizeof(Vector2));
}

CSolDaeLokker::~CSolDaeLokker()
{
    Release();
}

void CSolDaeLokker::Initialize()
{
    m_pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();

    m_fMoveDelta = n_fSpeedSeed;
    m_eCurState = INTRO;
    m_vPivot = Vector2((WINCX / 2), -100);
    m_vSize = Vector2(960.f, 900.f);
    m_iHp = 80;

    LoadBmp();
    LoadAnim();
}

int CSolDaeLokker::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    if (m_eCurState == IDLE || m_eCurState == SDLK_END)
        m_fDelta += 100.f * DELTA;
    else
        m_fDelta = 0.f;

    if (Intro()) return OBJ_NOEVENT;
    SwitchPattern();

    return OBJ_NOEVENT;
}

void CSolDaeLokker::LateUpdate()
{
    m_pBodyAnim->UpdateAnimation();
    m_pAttackAnim->UpdateAnimation();

    if (Dead()) return;
    Move();
    ShoopDaWhoop();
    YellowCorn();
}

void CSolDaeLokker::Render(HDC _hDC)
{
    RenderBodyAnimation(_hDC);
    RenderJewelAnimation(_hDC);
    RenderAttackAnimation(_hDC);
}

void CSolDaeLokker::Release()
{
    m_pPlayer = nullptr;
    SafeDelete<CAnimation*>(m_pBodyAnim);
    SafeDelete<CAnimation*>(m_pJewelAnim);
    SafeDelete<CAnimation*>(m_pAttackAnim);
}

void CSolDaeLokker::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == PROJECTILE)
    {
        CProjectile* pPrjt = dynamic_cast<CProjectile*>(_pCol);

        if (pPrjt->GetDamageFlag() == ENEMY)
            m_iHp -= pPrjt->GetDamage();
    }
}

void CSolDaeLokker::SwitchPattern()
{
    if (m_eCurPattern != NONE && m_eCurState != IDLE) return;

    if (m_fDelta >= 200.f)
    {
        m_fDelta = 0.f;

        

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> pattern(0, 3);
        switch (pattern(gen))
        {
        case 0:
        {
            std::uniform_int_distribution<int> dis(0, 2);
            m_eCurState = MOVE;
            m_pAttackAnim->SetFrameSpeed(0.07f);
            m_vDirection = (n_vMovePoint[dis(gen)] - m_vPivot).GetNomalized();
            m_vMove = n_vMovePoint[dis(gen)];
            break;
        }
        break;
        case 1: 
        {
            std::uniform_int_distribution<int> dis(0, 1);
            SetAnim(m_pAttackAnim, L"SolDaeLokker_SDW_Ready", false, READY, SDW);
            m_pAttackAnim->SetSizeOffset(n_vSDWOffset);
            m_pAttackAnim->SetFrameSpeed(0.07f);
            m_vDirection = (n_vSDWPoint[dis(gen)] - m_vPivot).GetNomalized();
            break;
        }
        case 2:
            SetAnim(m_pAttackAnim, L"SolDaeLokker_YelCorn_Ready", false, READY, YEL_CORN);
            m_pAttackAnim->SetSizeOffset(n_vSDWOffset);
            m_pAttackAnim->SetFrameSpeed(0.05f);
            break;
        case 3:
            break;
        default:
            break;
        }
    }
}

bool CSolDaeLokker::Intro()
{
    if (m_eCurState != INTRO) return false;
    if (m_vPivot.y >= 300.f)
    {
        m_pColBox = CColliderFactory::Create(this, HITBOX);
        m_pColBox->SetSize(Vector2(60.f, 84.f));
        m_pColBox->SetOffset(Vector2(0.f, -184.f));
        m_eCurState = IDLE;
        return false;
    }

    m_fIntroDelta += 0.6f * DELTA;
    m_vPivot.y += m_fIntroDelta;

    return true;
}

bool CSolDaeLokker::Dead()
{
    if (m_eCurState == DEAD)
    {
        m_fDeadDelta += 0.1f * DELTA;
        m_vPivot.y += m_fDeadDelta;
        return true;
    }

    if (m_iHp <= 0 && m_eCurState != DEAD)
    {
        m_vDirection = Vector2::Zero;
        SetAnim(m_pAttackAnim, L"SolDaeLokker_None", false, DEAD, ATTACK_END);
        SetAnim(m_pBodyAnim, L"SolDaeLokker_Dead", false, DEAD, ATTACK_END);
        m_pBodyAnim->SetFrameSpeed(0.2f);
        SafeDelete<CCollider*>(m_pColBox);
        return true;
    }

    return false;
}

void CSolDaeLokker::Move()
{
    if (m_eCurState != MOVE) return;

    if ((m_vMove - m_vPivot).GetNomalized().x <= 150.f || (m_vMove - m_vPivot).GetNomalized().y <= 150.f )
        m_fMoveDelta += -(45.f * DELTA);
    else
        m_fMoveDelta += -(70.f * DELTA);

    if (m_fMoveDelta < 0.f) m_fMoveDelta = 8.f;

    m_vPivot.x += m_vDirection.x * m_fMoveDelta;
    m_vPivot.y += m_vDirection.y * m_fMoveDelta;

    if (m_vDirection.x < 0.f && m_vPivot.x <= m_vMove.x)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.x = m_vMove.x;
        m_vDirection.x = 0.f;
    }
    else if (m_vDirection.x > 0.f && m_vPivot.x >= m_vMove.x)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.x = m_vMove.x;
        m_vDirection.x = 0.f;
    }

    if (m_vDirection.y < 0.f && m_vPivot.y <= m_vMove.y)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.y = m_vMove.y;
        m_vDirection.y = 0.f;
    }
    else if (m_vDirection.y > 0.f && m_vPivot.y >= m_vMove.y)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.y = m_vMove.y;
        m_vDirection.y = 0.f;
    }

    if (m_vDirection == Vector2::Zero)
    {
        m_fMoveDelta = n_fSpeedSeed;
        m_eCurState = IDLE;
    }
}

void CSolDaeLokker::ShoopDaWhoop()
{
    if (m_eCurPattern != SDW) return;
    
    m_fSDWDelta += 10.f * DELTA;

    if (m_eCurState == READY)
    {
        m_vPivot.x += m_vDirection.x * m_fSDWDelta;
        m_vPivot.y += m_vDirection.y * m_fSDWDelta;

        if (m_vDirection.x < 0 && m_vPivot.x <= n_vSDWPoint[0].x)
        {
            m_fSDWDelta = 0.f;
            m_vPivot = n_vSDWPoint[0];
            m_vDirection = Vector2::Zero;
        }
        else if (m_vDirection.x > 0 && m_vPivot.x >= n_vSDWPoint[1].x)
        {
            m_fSDWDelta = 0.f;
            m_vPivot = n_vSDWPoint[1];
            m_vDirection = Vector2::Zero;
        }

        if (!m_pAttackAnim->GetEndOneLoop()) return;

        SetAnim(m_pAttackAnim, L"SolDaeLokker_SDW_Attack", false, ATTACK, SDW);
        m_fSDWDelta = 0.f;

        if (m_vPivot.x < WINCX / 2)
            m_vDirection = Vector2::UnitX;
        else if (m_vPivot.x > WINCX / 2)
            m_vDirection = Vector2::UnitX * -1.f;
    }
    else if (m_eCurState == ATTACK)
    {
        m_vPivot.x += m_vDirection.x * m_fSDWDelta;

        if (m_vDirection.x > 0.f && n_vSDWPoint[1].x - 128.f <= m_vPivot.x)
        {
            m_vPivot.x = n_vSDWPoint[1].x - 128.f;
            m_vDirection = Vector2::Zero;
        }
        else if (m_vDirection.x < 0.f && n_vSDWPoint[0].x + 128.f >= m_vPivot.x)
        {
            m_vPivot.x = n_vSDWPoint[0].x + 128.f;
            m_vDirection = Vector2::Zero;
        }

        if (m_vDirection != Vector2::Zero) return;

        if (m_pAttackAnim->GetEndOneLoop())
        {
            SetAnim(m_pAttackAnim, L"SolDaeLokker_SDW_End", false, ATK_END, SDW);
            return;
        }
    }
    else if (m_eCurState == ATK_END && m_pAttackAnim->GetEndOneLoop())
    {
        m_fSDWDelta = 0.f;
        m_pAttackAnim->SetSizeOffset(Vector2::Zero);
        SetAnim(m_pAttackAnim, L"SolDaeLokker_None", false, IDLE, NONE);
    }
}

void CSolDaeLokker::YellowCorn()
{
    if (m_eCurPattern != YEL_CORN) return;

    if (m_eCurState == READY && m_pAttackAnim->GetEndOneLoop())
    {
        m_fSDWDelta = 0.f;
        m_pAttackAnim->SetSizeOffset(Vector2::Zero);
        SetAnim(m_pAttackAnim, L"SolDaeLokker_YelCorn_Attack", true, ATTACK, YEL_CORN);
        m_pAttackAnim->SetFrameSpeed(0.04f);
    }
    else if (m_eCurState == ATTACK)
    {
        m_fCornDelta += 100.f * DELTA;
        m_fCornAccDelta += 100.f * DELTA;
        
        if (m_fCornAccDelta >= 800.f)
        {
            m_fCornDelta = 0.f;
            m_fCornAccDelta = 0.f;
            m_pAttackAnim->SetSizeOffset(n_vSDWOffset);
            SetAnim(m_pAttackAnim, L"SolDaeLokker_YelCorn_End", false, ATK_END, YEL_CORN);
            m_pAttackAnim->SetFrameSpeed(0.05f);
            return;
        }

        if (m_fCornDelta >= 10.f)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> idegree(180, 360);

            m_fCornDelta = 0.f;
            Vector2 vPos = m_vPivot + Vector2(0.f, -184.f);
            CProjectileFactory<CYellowCorn>
                ::CreateProjectile(vPos, Vector2::UnitX, 0, static_cast<float>(idegree(gen)));
        }
    }
    else if (m_eCurState == ATK_END && m_pAttackAnim->GetEndOneLoop())
    {
        m_fCornDelta = 0.f;
        m_pAttackAnim->SetSizeOffset(Vector2::Zero);
        SetAnim(m_pAttackAnim, L"SolDaeLokker_None", false, IDLE, NONE);
    }
}

void CSolDaeLokker::RedCorn()
{
}

void CSolDaeLokker::RenderBodyAnimation(HDC _hDC)
{
    m_pBodyAnim->RenderAnimation(_hDC);
}

void CSolDaeLokker::RenderJewelAnimation(HDC _hDC)
{
}

void CSolDaeLokker::RenderAttackAnimation(HDC _hDC)
{
    m_pAttackAnim->RenderAnimation(_hDC);
}

inline void CSolDaeLokker::SetAnim(CAnimation* _pAnim, const TCHAR* _szKey, bool _bIsRepeat
    , SDLK_STATE _eBodyState, SDLK_ATTACK _eAttackState, int _iLastFacingX)
{
    if (m_pBodyAnim && _pAnim == m_pBodyAnim)
    {
        m_pBodyAnim->ChangeAnimation(_szKey);
        m_pBodyAnim->SetLoop(_bIsRepeat);
        m_eCurState = _eBodyState;
        m_eCurPattern = _eAttackState;

        if (_iLastFacingX == 0) return;
        m_pBodyAnim->SetLastFacingX(_iLastFacingX);
    }
    else if (m_pJewelAnim && _pAnim == m_pJewelAnim)
    {
        m_pJewelAnim->ChangeAnimation(_szKey);
        m_pJewelAnim->SetLoop(_bIsRepeat);
        m_eCurState = _eBodyState;
        m_eCurPattern = _eAttackState;

        if (_iLastFacingX == 0) return;
        m_pJewelAnim->SetLastFacingX(_iLastFacingX);
    }
    else if (m_pAttackAnim && _pAnim == m_pAttackAnim)
    {
        m_pAttackAnim->ChangeAnimation(_szKey);
        m_pAttackAnim->SetLoop(_bIsRepeat);
        m_eCurState = _eBodyState;
        m_eCurPattern = _eAttackState;

        if (_iLastFacingX == 0) return;
        m_pAttackAnim->SetLastFacingX(_iLastFacingX);
    }
}

void CSolDaeLokker::LoadBmp()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Corn/SolDaeRokker_YelCorn_Right.bmp"
        , L"SolDaeRokker_YelCorn_Right");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Corn/SolDaeRokker_YelCorn_Left.bmp"
        , L"SolDaeRokker_YelCorn_Left");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Corn/SolDaeRokker_RedCorn_Right.bmp"
        , L"SolDaeRokker_RedCorn_Right");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Corn/SolDaeRokker_RedCorn_Left.bmp"
        , L"SolDaeRokker_RedCorn_Left");
}

void CSolDaeLokker::LoadAnim()
{
    m_pBodyAnim = new CAnimation();
    m_pAttackAnim = new CAnimation();

    m_pBodyAnim->AddAnimation(L"SolDaeLokker", pair<int, int>{0, 1});
    m_pBodyAnim->AddAnimation(L"SolDaeLokker_Dead", pair<int, int>{0, 6});

    m_pAttackAnim->AddAnimation(L"SolDaeLokker_None", pair<int, int>{0, 1});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_SDW_Ready", pair<int, int>{0, 24});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_SDW_Attack", pair<int, int>{0, 16});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_SDW_End", pair<int, int>{0, 29});
    
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_YelCorn_Ready", pair<int, int>{0, 28});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_YelCorn_Attack", pair<int, int>{0, 3});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_YelCorn_End", pair<int, int>{0, 10});
    
    

    m_pBodyAnim->Initialize();
    m_pAttackAnim->Initialize();
    m_pBodyAnim->SetParent(this);
    m_pAttackAnim->SetParent(this);
    m_pBodyAnim->ChangeAnimation(L"SolDaeLokker");
    m_pAttackAnim->ChangeAnimation(L"SolDaeLokker_None");
}
