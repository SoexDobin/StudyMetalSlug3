#include "pch.h"
#include "CPrisoner.h"
#include "CAnimation.h"
#include "CPlatformChecker.h"
#include "CProjectile.h"
#include "CExplodeArea.h"
#include "CScene.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CTimeManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"

CPrisoner::CPrisoner()
    : m_bEndSupply(false), m_pAnim(nullptr), m_pPlatformCol(nullptr), m_pSupplyItem(nullptr)
    , m_bPatrolSwitch(false), m_fPatrolDelta(0.f), m_bRunAway(false)
    
{
}

CPrisoner::~CPrisoner()
{
    Release();
}

void CPrisoner::Initialize()
{
    LoadBmp();
    m_vSize = Vector2(144.f, 144.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(96.f, 128.f));
    m_pColBox->SetOffset(Vector2(0.f, 16.f));

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pPlatformCol->GetCollider()->SetOffset({ 0.f, m_vSize.y * 0.5f });
    m_bPatrolSwitch = true;
    m_vDirection = Vector2::UnitX * -1.f;
    m_vFace = m_vDirection;


    m_pSupplyItem = CSceneManager::GetInstance().GetCurScene()->GetItems().back();
    CSceneManager::GetInstance().GetCurScene()->GetItems().back() = nullptr;
    CSceneManager::GetInstance().GetCurScene()->GetItems().pop_back();
}

int CPrisoner::Update()
{

    // TODO 바운더리 나가면 삭제
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();
    m_vPivot.y += 200.f * DELTA;
    CheckPlatform();

    if (!m_bPatrolSwitch && m_pAnim->GetEndOneLoop() && m_bRunAway == false)
    {
        m_vPivot = m_vPivot + Vector2(48.f, 0.f);
        m_bEndSupply = true;
        m_pAnim->ChangeAnimation(L"PrisonerMove");
        m_pAnim->SetFrameSpeed(0.1f);
        m_pAnim->SetLoop(true);
        m_bRunAway = true;

        if (m_pSupplyItem == nullptr) return OBJ_NOEVENT;
        CObjectManager::GetInstance().AddGameObject(m_pSupplyItem,ITEM);
        m_pSupplyItem->Initialize();
        m_pSupplyItem->SetPivot(m_vPivot + Vector2(-48.f, 24.f));
        // TODO 아이템 생성

        return OBJ_NOEVENT;
    }

    GoLeft();

    Patrol();

    return OBJ_NOEVENT;
}

void CPrisoner::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CPrisoner::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CPrisoner::Release()
{
    m_pSupplyItem = nullptr;
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
    m_pPlatformCol->SetParent(nullptr);
    m_pPlatformCol->SetDestroy();
}

void CPrisoner::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (!m_bPatrolSwitch || m_bEndSupply) return;

    if (_pCol->GetObjectType() == PLAYER)
    {
        CSoundManager::GetInstance().PlaySound(L"Prisoner_Touch.mp3", NEUTRAL_SOUND, 0.5f);
        m_vDirection = Vector2::Zero;
        m_vFace = Vector2::UnitX;
        m_pAnim->SetFrameLayer(0);
        m_pAnim->SetFrameSpeed(0.1f);
        m_bPatrolSwitch = false;
        m_pAnim->ChangeAnimation(L"PrisonerSupply");
        m_pAnim->SetLoop(false);
    }
}

void CPrisoner::GoLeft()
{
    if (!m_bRunAway) return;

    m_vDirection = Vector2::UnitX * -1.f;
    m_vFace = m_vDirection;
    m_vPivot.x += m_vDirection.x * 50.f * DELTA;
}

void CPrisoner::Patrol()
{
    if (m_bRunAway) return;
    if (m_bPatrolSwitch == false) return;

    m_fPatrolDelta += 10.f * DELTA;
    m_vPivot.x += m_vDirection.x * 20.f * DELTA;

    if (m_fPatrolDelta >= 50.f)
    {
        m_vDirection *= -1.f;
        m_vFace = m_vDirection;
        m_fPatrolDelta = 0.f;
    }
}

void CPrisoner::LoadBmp()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerMove.bmp", L"PrisonerMove");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerSupply.bmp", L"PrisonerSupply");

    m_pAnim = new CAnimation();
    m_pAnim->AddAnimation(L"PrisonerMove", pair<int, int>{0, 7});
    m_pAnim->AddAnimation(L"PrisonerSupply", pair<int, int>{0, 11});

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->ChangeAnimation(L"PrisonerMove");
    m_pAnim->SetLoop(true);
    m_pAnim->SetFrameSpeed(0.1f);
}

void CPrisoner::CheckPlatform()
{

    bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();


    if (bColPlatform) // && m_bIsDrop
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;

        return;
    }
}
