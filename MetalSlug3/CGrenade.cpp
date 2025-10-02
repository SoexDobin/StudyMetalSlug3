#include "pch.h"
#include "CGrenade.h"
#include "CEri.h"

#include "CColliderFactory.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"
#include "CScrollManager.h"

CGrenade::CGrenade()
{
}

CGrenade::~CGrenade()
{
    Release();
}

void CGrenade::Initialize()
{
    m_eType = ITEM;
    m_eItemType = BOMB;
    m_vSize = Vector2(96.f, 96.f);

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize * 0.5f);
}

int CGrenade::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_pColBox->SetSize(m_vSize * 0.5f);

    return OBJ_NOEVENT;
}

void CGrenade::LateUpdate()
{
}

void CGrenade::Render(HDC _hDC)
{
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Grenade");

    GdiTransparentBlt(_hDC
        , m_tRect.left + SCROLLX, m_tRect.top + SCROLLY
        , m_vSize.x, m_vSize.y
        , hMemDC
        , 0, 0
        , m_vSize.x, m_vSize.y
        , RGB(255, 255, 255));

}

void CGrenade::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CGrenade::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == PLAYER)
    {
        m_pColBox->SetEnableCol(false);
        CEri* pPlayer = dynamic_cast<CEri*>(CSceneManager::GetInstance().GetCurPlayer());
        pPlayer->SetBomb(pPlayer->GetBomb() + 5);
        CSoundManager::GetInstance().PlaySound(L"OK.mp3", ANOUNCE, 0.2f);
        m_bDestroy = true;
        // ธิดย ฦฤฦผลฌ
    }
}
