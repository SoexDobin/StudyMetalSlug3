#include "pch.h"
#include "CPrisonerTied.h"
#include "CAnimation.h"
#include "CPlatformChecker.h"
#include "CProjectile.h"
#include "CExplodeArea.h"
#include "CPrisoner.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CGameObjectFactory.h"
#include "CTimeManager.h"

CPrisonerTied::CPrisonerTied()
    : m_pAnim(nullptr), m_bIsUnravel(false), m_pPlatformCol(nullptr), m_bLockUnravel(false)
{
}

CPrisonerTied::~CPrisonerTied()
{
    Release();
}

void CPrisonerTied::Initialize()
{
    LoadBmp();
    m_iHp = 1;
    m_vSize = Vector2(144.f, 144.f);
    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(Vector2(96.f, 128.f));
    m_pColBox->SetOffset(Vector2(0.f, 16.f));

    m_pPlatformCol = CGameObjectFactory<CPlatformChecker>::Create(Vector2::Zero, Vector2::Zero, this);
    CObjectManager::GetInstance().AddGameObject(m_pPlatformCol, NEUTRAL);
    m_pPlatformCol->GetCollider()->SetOffset({ 0.f, m_vSize.y * 0.5f });
}

int CPrisonerTied::Update()
{
    if (m_bIsUnravel && m_pAnim->GetEndOneLoop())
    {
        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CPrisoner>::Create(m_vPivot, m_vSize), NEUTRAL);
        // TODO 포로 생성
        m_bDestroy = true;
    }
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();
    m_vPivot.y += 200.f * DELTA;
    CheckPlatform();


    return OBJ_NOEVENT;
}

void CPrisonerTied::LateUpdate()
{
    m_pAnim->UpdateAnimation();
}

void CPrisonerTied::Render(HDC _hDC)
{
    m_pAnim->RenderAnimation(_hDC);
}

void CPrisonerTied::Release()
{
    SafeDelete<CAnimation*>(m_pAnim);
    SafeDelete<CCollider*>(m_pColBox);
    m_pPlatformCol->SetParent(nullptr);
    m_pPlatformCol->SetDestroy();
}

void CPrisonerTied::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (m_bLockUnravel == true) return;
    switch (_pCol->GetObjectType())
    {
    case PROJECTILE:
    {
        CProjectile* pProj = dynamic_cast<CProjectile*>(_pCol);
        if (pProj->GetDamageFlag() == ENEMY)
        {
            m_iHp -= pProj->GetDamage();
            m_pColBox->SetEnableCol(false);
            m_bIsUnravel = true;
            m_pAnim->ChangeAnimation(L"PrisonerUnravel");
            m_pAnim->SetLoop(false);
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
            m_pColBox->SetEnableCol(false);
            m_bIsUnravel = true;
            m_pAnim->ChangeAnimation(L"PrisonerUnravel");
            m_pAnim->SetLoop(false);
        }
        break;
    }
    break;
    default:
        break;
    }
}


void CPrisonerTied::LoadBmp()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerTied.bmp", L"PrisonerTied");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerUnravel.bmp", L"PrisonerUnravel");

    m_pAnim = new CAnimation();
    m_pAnim->AddAnimation(L"PrisonerTied", pair<int, int>{0, 7});
    m_pAnim->AddAnimation(L"PrisonerUnravel", pair<int, int>{0, 4});

    m_pAnim->Initialize();
    m_pAnim->SetParent(this);
    m_pAnim->ChangeAnimation(L"PrisonerTied");
    m_pAnim->SetLoop(true);
    m_pAnim->SetFrameSpeed(0.1f);
}

void CPrisonerTied::CheckPlatform()
{

    bool bColPlatform = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetHasColWithPlatform();


    if (bColPlatform) // && m_bIsDrop
    {
        float fPosY = dynamic_cast<CPlatformChecker*>(m_pPlatformCol)->GetColTopPosition();
        m_vPivot.y = fPosY - (m_pColBox->GetSize().y / 2.f) - m_pColBox->GetOffset().y;

        return;
    }
}
