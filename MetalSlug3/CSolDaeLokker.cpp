#include "pch.h"
#include <random>
#include <iostream>
#include <cmath>
#include "CSolDaeLokker.h"
#include "CAnimation.h"
#include "CProjectile.h"
#include "CYellowCorn.h"
#include "CRedCorn.h"
#include "CShoopDaWhoopArea.h"
#include "CParticle.h"
#include "CSDWEffect.h"
#include "CBigExplode.h"
#include "CMediumExplode.h"

#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CProjectileFactory.h"
#include "CSoundManager.h"
#include "CTimerUI.h"
#include "CGameObjectFactory.h"
#include "CParticleManager.h"
#include "CSceneManager.h"


CSolDaeLokker::CSolDaeLokker()
    : m_pPlayer(nullptr)
    , m_pBodyAnim(nullptr), m_pJewelAnim(nullptr), m_pAttackAnim(nullptr)
    , m_bIsTopFloat(false)
    , m_fDelta(0.f), m_fIntroDelta(0.f), m_fDeadDelta(0.f), m_fMoveDelta(0.f), m_fCornDelta(0.f), m_fCornAccDelta(0.f)
    , m_fFloatingDelta(0.f)
    , m_fSDWDelta(0.f), m_fIdleDelta(0.f), m_fBeforeExplodeDelta(0.f)
    , m_eCurState(SDLK_END), m_eCurPattern(NONE)
    , m_iEffectIdx(0)
    , m_iDeadExplodeIdx(0)
    , m_bShowAllPattern(false), m_iShowIndex(10)
{
    ZeroMemory(&m_vMove, sizeof(Vector2));
}

CSolDaeLokker::~CSolDaeLokker()
{
    Release();
}

void CSolDaeLokker::Initialize()
{
    // for Test
    m_bShowAllPattern = true;

    m_pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();

    m_fIdleDelta = 100.f;
    m_fMoveDelta = n_fSpeedSeed;
    m_eCurState = INTRO;
    m_vPivot = Vector2((WINCX / 2), -100);
    m_vSize = Vector2(960.f, 900.f);
    m_iHp = 100;

    LoadBmp();
    LoadAnim();
    m_fFloatingDelta = 0.8f;
}

int CSolDaeLokker::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    if (m_eCurState == IDLE || m_eCurState == SDLK_END)
        m_fDelta += 80.f * DELTA;
    else
        m_fDelta = 0.f;

    if (Intro()) return OBJ_NOEVENT;
    SwitchPattern();
    Floating();

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
    RedCorn();
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
        std::uniform_int_distribution<int> iPattern(0, 3);

        int iIDX = 0;
        if (m_bShowAllPattern)
            iIDX = m_iShowIndex;
        else
            iIDX = iPattern(gen);

        switch (iIDX)
        {
        case 0: case 10: case 11: case 12: case 13: case 14: case 15:
        {
            std::uniform_int_distribution<int> iDis(0, 5);
            m_eCurState = MOVE;
            m_pAttackAnim->SetFrameSpeed(0.07f);

            int iRand = iDis(gen);
            if (m_vMove == n_vMovePoint[iRand])
            {
                m_vMove = n_vMovePoint[(iRand + 1) % 5];
            }
            else
                m_vMove = n_vMovePoint[iRand];
            break;
        }
        break;
        case 1: case 16: case 17:
        {
            std::uniform_int_distribution<int> dis(0, 1);
            SetAnim(m_pAttackAnim, L"SolDaeLokker_SDW_Ready", false, READY, SDW);
            m_pAttackAnim->SetSizeOffset(n_vSDWOffset);
            m_pAttackAnim->SetFrameSpeed(0.07f);
            m_vDirection = (n_vSDWPoint[dis(gen)] - m_vPivot).GetNomalized();
            break;
        }
        case 2: case 18:
            CSoundManager::GetInstance()
                .PlaySoundOnce(L"SolDaeLokker_Corn_Ready.mp3", ENEMY_READY, 0.2f);
            SetAnim(m_pAttackAnim, L"SolDaeLokker_YelCorn_Ready", false, READY, YEL_CORN);
            m_pAttackAnim->SetSizeOffset(n_vSDWOffset);
            m_pAttackAnim->SetFrameSpeed(0.05f);
            break;
        case 3: case 19:
            CSoundManager::GetInstance()
                .PlaySoundOnce(L"SolDaeLokker_Corn_Ready.mp3", ENEMY_READY, 0.2f);
            SetAnim(m_pAttackAnim, L"SolDaeLokker_RedCorn_Ready", false, READY, RED_CORN);
            m_pAttackAnim->SetSizeOffset(n_vSDWOffset);
            m_pAttackAnim->SetFrameSpeed(0.05f);
            break;
        default:

            break;
        }

        if (m_iShowIndex == 19)
            m_bShowAllPattern = false;
        else
            ++m_iShowIndex;
    }
}

void CSolDaeLokker::Floating()
{
    if (m_eCurState == DEAD) return;

    m_fFloatingDelta -= 0.2f * DELTA;

    if (m_bIsTopFloat)
    {
        if (m_fFloatingDelta <= 0.5f)
        {
            m_fFloatingDelta = 0.65f;
            m_bIsTopFloat = !m_bIsTopFloat;
            return;
        } 
        m_vPivot.y -= m_fFloatingDelta;
    }
    else
    {
        if (m_fFloatingDelta <= 0.5f)
        {
            m_fFloatingDelta = 0.65f;
            m_bIsTopFloat = !m_bIsTopFloat;
            return;
        }

        m_vPivot.y += m_fFloatingDelta;
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
        m_fBeforeExplodeDelta += 10 * DELTA;

        m_vPivot.y += m_fDeadDelta;

        if (m_vPivot.y >= 520.f)
        {
            CSceneManager::GetInstance().SetMissionComplete();
            return true;
        }

        if (m_fBeforeExplodeDelta >= 5.f)
        {
            m_fBeforeExplodeDelta = 0.f;
            CParticleManager::GetInstance()
                .CreateParticle<CMediumExplode>(
                    Vector2(m_vPivot + n_vExplodePoint[m_iDeadExplodeIdx]), n_szEffectKey[m_iEffectIdx]);
            m_iDeadExplodeIdx = (m_iDeadExplodeIdx + 1) % 5;
        }

        return true;
    }

    if (m_iHp <= 0 && m_eCurState != DEAD)
    {
        m_vDirection = Vector2::Zero;
        SetAnim(m_pAttackAnim, L"SolDaeLokker_None", false, DEAD, ATTACK_END);
        SetAnim(m_pBodyAnim, L"SolDaeLokker_Dead", false, DEAD, ATTACK_END);
        CSoundManager::GetInstance().StopSound(SOUND_BGM);
        CSoundManager::GetInstance().
            PlaySoundOnce(L"Destroy.mp3", ENEMY_DEAD1, 0.2f);
        CSoundManager::GetInstance().
            PlaySound(L"Destroy_After.mp3", ENEMY_DEAD1, 0.2f);
        CParticleManager::GetInstance()
            .CreateParticle<CBigExplode>(
                Vector2(m_vPivot.x, m_vPivot.y - 192.f), n_szEffectKey[m_iEffectIdx]);
        m_pBodyAnim->SetFrameSpeed(0.2f);
        SafeDelete<CCollider*>(m_pColBox);
        return true;
    }

    return false;
}

void CSolDaeLokker::Move()
{
    if (m_eCurState != MOVE) return;

    Vector2 vDir = m_vMove - m_vPivot;
    float   fSize = vDir.GetSquared();

    Vector2 vNomalized = Vector2::Zero;

    if (fSize != 0.f)
    {
        float fMag = sqrtf(fSize);
        vNomalized = Vector2(vDir.x / fMag, vDir.y / fMag);
    }
        
    m_fMoveDelta += -(70.f * DELTA);

    if (m_fMoveDelta < 0.f) m_fMoveDelta = 8.f;

    m_vPivot.x += vNomalized.x * m_fMoveDelta;
    m_vPivot.y += vNomalized.y * m_fMoveDelta;

    if (vNomalized.x <= 0.f && m_vPivot.x <= m_vMove.x)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.x = m_vMove.x;
    }
    else if (vNomalized.x > 0.f && m_vPivot.x >= m_vMove.x)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.x = m_vMove.x;
    }

    if (vNomalized.y <= 0.f && m_vPivot.y <= m_vMove.y)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.y = m_vMove.y;
    }
    else if (vNomalized.y > 0.f && m_vPivot.y >= m_vMove.y)
    {
        m_fMoveDelta = 0.f;
        m_vPivot.y = m_vMove.y;
    }

    if (m_vPivot == m_vMove)
    {
        m_vDirection = Vector2::UnitY;
        m_fIdleDelta = 10.f;
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

        m_fSDWDelta = 0.f;

        if (m_vPivot.x < WINCX / 2)
            m_vDirection = Vector2::UnitX;
        else if (m_vPivot.x > WINCX / 2)
            m_vDirection = Vector2::UnitX * -1.f;

        SetAnim(m_pAttackAnim, L"SolDaeLokker_SDW_Attack", false, ATTACK, SDW);
        CSoundManager::GetInstance()
            .PlaySound(L"SolDaeLokker_SDW.mp3", ENEMY_ATTACK, 0.2f);
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

        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CShoopDaWhoopArea>
            ::Create(Vector2(m_vPivot.x, m_vPivot.y + 256.f), Vector2::Zero), EXPLODE);
        CParticleManager::GetInstance()
            .CreateParticle<CSDWEffect>(
                Vector2(m_vPivot.x, m_vPivot.y + 224.f), n_szEffectKey[m_iEffectIdx]);

        m_iEffectIdx = (m_iEffectIdx + 1) % 4;

        if (m_vDirection != Vector2::Zero) return;

        if (m_pAttackAnim->GetEndOneLoop())
        {
            SetAnim(m_pAttackAnim, L"SolDaeLokker_SDW_End", false, ATK_END, SDW);
            return;
        }
    }
    else if (m_eCurState == ATK_END && m_pAttackAnim->GetEndOneLoop())
    {
        m_vDirection = Vector2::UnitY;
        m_fIdleDelta = 100.f;
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
        CSoundManager::GetInstance()
            .PlaySoundOnce(L"SolDaeLokker_Corn_Attack.mp3", ENEMY_ATTACK, 0.2f);
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
            CSoundManager::GetInstance()
                .PlaySound(L"Corn_Shoot.mp3", ENEMY_SHOOT, 0.05f);
        }
    }
    else if (m_eCurState == ATK_END && m_pAttackAnim->GetEndOneLoop())
    {
        m_vDirection = Vector2::UnitY;
        m_fIdleDelta = 10.f;
        m_fCornDelta = 0.f;
        m_pAttackAnim->SetSizeOffset(Vector2::Zero);
        SetAnim(m_pAttackAnim, L"SolDaeLokker_None", false, IDLE, NONE);
    }
}

void CSolDaeLokker::RedCorn()
{
    if (m_eCurPattern != RED_CORN) return;

    if (m_eCurState == READY && m_pAttackAnim->GetEndOneLoop())
    {
        m_fSDWDelta = 0.f;
        m_pAttackAnim->SetSizeOffset(Vector2::Zero);
        SetAnim(m_pAttackAnim, L"SolDaeLokker_RedCorn_Attack", true, ATTACK, RED_CORN);
        CSoundManager::GetInstance()
            .PlaySoundOnce(L"SolDaeLokker_Corn_Attack.mp3", ENEMY_ATTACK, 0.2f);
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
            SetAnim(m_pAttackAnim, L"SolDaeLokker_RedCorn_End", false, ATK_END, RED_CORN);
            m_pAttackAnim->SetFrameSpeed(0.05f);
            return;
        }

        if (m_fCornDelta >= 100.f)
        {
            
            Vector2 vDst = m_pPlayer->GetPivot() - m_vPivot;
            vDst.y += 128.f;
            float fDiagonal = sqrtf(vDst.GetSquared());
            
            float fRad = acosf(vDst.x / fDiagonal);
            fRad = 2.f * 3.14156f - fRad;
            
            float fAngle = fRad * (180.f / 3.14156f);

            Vector2 vPos = Vector2::Zero;
            if (m_fCornDelta >= 120.f)
            {
                vPos = m_vPivot + Vector2(48.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
                vPos = m_vPivot + Vector2(-48.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
                m_fCornDelta = 0.f;
            }
            else if (m_fCornDelta >= 115.f)
            {
                vPos = m_vPivot + Vector2(32.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
                vPos = m_vPivot + Vector2(-32.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
            }
            else if (m_fCornDelta >= 110.f)
            {
                vPos = m_vPivot + Vector2(18.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
                vPos = m_vPivot + Vector2(-18.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
            }
            else if (m_fCornDelta >= 105.f)
            {
                vPos = m_vPivot + Vector2(9.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
                vPos = m_vPivot + Vector2(-9.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
            }
            else
            {
                vPos = m_vPivot + Vector2(0.f, -184.f);
                CProjectileFactory<CRedCorn>
                    ::CreateProjectile(vPos, Vector2::UnitX, 0, fAngle);
            }
            CSoundManager::GetInstance()
                .PlaySound(L"Corn_Shoot.mp3", ENEMY_SHOOT, 0.1f);
        }
    }
    else if (m_eCurState == ATK_END && m_pAttackAnim->GetEndOneLoop())
    {
        m_vDirection = Vector2::UnitY;
        m_fIdleDelta = 10.f;
        m_fCornDelta = 0.f;
        m_pAttackAnim->SetSizeOffset(Vector2::Zero);
        SetAnim(m_pAttackAnim, L"SolDaeLokker_None", false, IDLE, NONE);
    }
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
    
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_RedCorn_Ready", pair<int, int>{0, 28});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_RedCorn_Attack", pair<int, int>{0, 3});
    m_pAttackAnim->AddAnimation(L"SolDaeLokker_RedCorn_End", pair<int, int>{0, 10});
    

    m_pBodyAnim->Initialize();
    m_pAttackAnim->Initialize();
    m_pBodyAnim->SetParent(this);
    m_pAttackAnim->SetParent(this);
    m_pBodyAnim->ChangeAnimation(L"SolDaeLokker");
    m_pAttackAnim->ChangeAnimation(L"SolDaeLokker_None");
}
