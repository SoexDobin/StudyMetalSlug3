#include "pch.h"
#include "CManEater.h"
#include "CHitBox.h"
#include "CAnimation.h"

#include "CColliderFactory.h"
#include "CScrollManager.h"

CManEater::CManEater()
    : m_pAnim(nullptr), m_pRefPlayer(nullptr)
{
}

CManEater::~CManEater()
{
    Release();
}

void CManEater::Initialize()
{
    LoadAnimation();

    m_vPivot = Vector2(800.f, 500.f);
    m_vSize = Vector2(192.f, 192.f);
    m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);
    m_pColBox->SetSize(Vector2(144.f, 144.f));
    m_pColBox->SetOffset(Vector2(0.f, -12.f));
}

int CManEater::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CManEater::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CManEater::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CManEater::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
}

void CManEater::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    
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


void CManEater::LoadAnimation()
{
    m_pAnim = new CAnimation();


    m_pAnim->AddAnimation(L"ManEater_Idle", pair<int, int>{ 0, 12 });

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->ChangeAnimation(L"ManEater_Idle");
    m_pAnim->SetFrameSpeed(0.1f);
}
