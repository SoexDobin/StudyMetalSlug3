#pragma once
#include "CEnemy.h"

class CAnimation;

class CSolDaeLokker final : public CEnemy
{
public:
    CSolDaeLokker();
    ~CSolDaeLokker() override;

private:
    enum SDLK_STATE { INTRO, IDLE, MOVE, READY, ATTACK, ATK_END, DEAD, SDLK_END };
    enum SDLK_ATTACK { NONE, SDW, YEL_CORN, RED_CORN, ATTACK_END };
public:

public:
    void        Initialize()                                                                    override;
    int         Update()                                                                        override;
    void        LateUpdate()                                                                    override;
    void        Render(HDC _hDC)                                                                override;
    void        Release()                                                                       override;
    void        OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)   override;

private:
    void        SwitchPattern();

    bool        Intro();
    bool        Dead();

    void        Move();
    void        ShoopDaWhoop();
    void        YellowCorn();
    void        RedCorn();

    void        RenderBodyAnimation(HDC _hDC);
    void        RenderJewelAnimation(HDC _hDC);
    void        RenderAttackAnimation(HDC _hDC);

    inline void SetAnim(CAnimation* _pAnim, const TCHAR* _szKey, bool _bIsRepeat, SDLK_STATE _eBodyState, SDLK_ATTACK _eAttackState, int _iLastFacingX = 0);
    void        LoadBmp();
    void        LoadAnim();
private:
    CGameObject*    m_pPlayer;
    CAnimation*     m_pBodyAnim;
    CAnimation*     m_pJewelAnim;
    CAnimation*     m_pAttackAnim;

    SDLK_ATTACK     m_eCurPattern;
    SDLK_STATE      m_eCurState;
    
    float           m_fDelta;
    float           m_fIntroDelta;
    float           m_fDeadDelta;

    float           m_fMoveDelta;
    float           m_fSDWDelta;
    float           m_fCornDelta;
    float           m_fCornAccDelta;
    Vector2         m_vMove;
private:
    float           n_fSpeedSeed        = 20.f;
    Vector2         n_vMovePoint[5]     = { {192.f, 256.f}
                                        , {480.f, 256.f}
                                        , {768.f, 256.f}
                                        , { 320.f, 480.f }
                                        , { 640.f, 480.f } };

    Vector2         n_vSDWPoint[2]      = { {100.f, 356.f}, { 860.f, 356.f } };
    Vector2         n_vSDWOffset        = { 576.f, 0.f };
    
};

