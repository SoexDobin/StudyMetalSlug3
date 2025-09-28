#include "pch.h"
#include "CYellowCorn.h"
#include "CBulletHitParticle.h"
#include "CPlatform.h"
#include "CAnimation.h"

#include "CColliderFactory.h"
#include "CTimeManager.h"
#include "CParticleManager.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CYellowCorn::CYellowCorn()
    : m_pAnim(nullptr), m_bIsRotate(false)
{

}

CYellowCorn::~CYellowCorn()
{
    Release();
}

void CYellowCorn::Initialize()
{
    m_vSize = Vector2(96.f, 96.f);

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize / 4.f);
    m_iDamage = 1;
    m_fSpeed = 500.f;
    m_eDamageFlag = PLAYER;

    m_pAnim = new CAnimation();
    m_pAnim->SetParent(this);
    m_pAnim->AddAnimation(L"SolDaeRokker_YelCorn_Left", pair<int, int>({0, 2}));
    m_pAnim->AddAnimation(L"SolDaeRokker_YelCorn_Right", pair<int, int>({ 0, 2 }));
    m_pAnim->SetLoop(true);
    m_pAnim->SetFrameSpeed(0.05);
    m_pAnim->SetEaseColor(RGB(255, 255, 255));

    m_eDamageFlag = PLAYER;
    m_pAnim->SetLockFacing(true);

    __super::UpdateGameObject();
}

int CYellowCorn::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;
    if (m_fAngle > 0.f && m_bIsRotate == false)
    {
        m_bIsRotate = true;
        m_vDirection.Rotate(m_fAngle);
        SetBmpByAngle();
    }

    __super::UpdateGameObject();

    m_pAnim->UpdateAnimation();

    m_vPivot += m_vDirection * m_fSpeed * DELTA;

    return OBJ_NOEVENT;
}

void CYellowCorn::LateUpdate()
{
    __super::CheckOutOfWindow();
}

void CYellowCorn::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CYellowCorn::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
    SafeDelete<CAnimation*>(m_pAnim);
}

void CYellowCorn::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_bDestroy) return;

    bool pPassThroughPlat = true;
    if (_pCol->GetObjectType() == PLATFORM)
        pPassThroughPlat = dynamic_cast<CPlatform*>(_pCol)->GetProjectilePass();

    if (_pCol->GetObjectType() == PLAYER || !pPassThroughPlat)
    {
        Vector2 vParticleOffset{ 0.f, 0.f };

        switch (_eFlag)
        {
        case LEFT_COL:  vParticleOffset = Vector2(-m_vSize.x / 2.f, 0.f);
            break;
        case RIGHT_COL: vParticleOffset = Vector2(m_vSize.x / 2.f, 0.f);
            break;
        case UP_COL:    vParticleOffset = Vector2(0.f, -m_vSize.y / 2.f);
            break;
        case DOWN_COL:  vParticleOffset = Vector2(0.f, m_vSize.y / 2.f);
            break;
        default:
            break;
        }

        CParticleManager::GetInstance().CreateParticle<CBulletHitParticle>(m_vPivot + vParticleOffset);
        m_bDestroy = OBJ_DESTROY;
        SafeDelete<CCollider*>(m_pColBox);
    }
}

void CYellowCorn::SetBmpByAngle()
{
    if (m_fAngle < 180.f || m_fAngle > 360.f) return;


    if (m_fAngle <= 270.f)
    {
        m_pAnim->ChangeAnimation(L"SolDaeRokker_YelCorn_Left");
        if ( m_fAngle < 185.f)              m_pAnim->SetFrameLayer(8);
        else if (m_fAngle < 200.f)          m_pAnim->SetFrameLayer(7);
        else if (m_fAngle < 210.f)          m_pAnim->SetFrameLayer(6);
        else if (m_fAngle < 220.f)          m_pAnim->SetFrameLayer(5);
        else if (m_fAngle < 230.f)          m_pAnim->SetFrameLayer(4);
        else if (m_fAngle < 240.f)          m_pAnim->SetFrameLayer(3);
        else if (m_fAngle < 250.f)          m_pAnim->SetFrameLayer(2);
        else if (m_fAngle < 260.f)          m_pAnim->SetFrameLayer(1);
        else if (m_fAngle <= 270.f)         m_pAnim->SetFrameLayer(0);

    }
    else if (m_fAngle > 270.f)
    {
        m_pAnim->ChangeAnimation(L"SolDaeRokker_YelCorn_Right");
        if (m_fAngle < 280.f)          m_pAnim->SetFrameLayer(0);
        else if (m_fAngle < 290.f)          m_pAnim->SetFrameLayer(1);
        else if (m_fAngle < 300.f)          m_pAnim->SetFrameLayer(2);
        else if (m_fAngle < 310.f)          m_pAnim->SetFrameLayer(3);
        else if (m_fAngle < 320.f)          m_pAnim->SetFrameLayer(4);
        else if (m_fAngle < 330.f)          m_pAnim->SetFrameLayer(5);
        else if (m_fAngle < 340.f)          m_pAnim->SetFrameLayer(6);
        else if (m_fAngle < 350.f)          m_pAnim->SetFrameLayer(7);
        else if (m_fAngle <= 360.f)         m_pAnim->SetFrameLayer(8);
    }
}
