#include "pch.h"
#include "CFlameShot.h"

#include "CEri.h"

#include "CColliderFactory.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"
#include "CScrollManager.h"

CFlameShot::CFlameShot()
{
}

CFlameShot::~CFlameShot()
{
    Release();
}

void CFlameShot::Initialize()
{
    m_eType = ITEM;
    m_eItemType = WEAPON;
    m_vSize = Vector2(96.f, 96.f);

    m_pColBox = CColliderFactory::Create(this, HITBOX);
    m_pColBox->SetSize(m_vSize * 0.5f);
}

int CFlameShot::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_pColBox->SetSize(m_vSize * 0.5f);

    return OBJ_NOEVENT;
}

void CFlameShot::LateUpdate()
{
}

void CFlameShot::Render(HDC _hDC)
{
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Flame_Shot");

    GdiTransparentBlt(_hDC
        , m_tRect.left + SCROLLX, m_tRect.top + SCROLLY
        , m_vSize.x, m_vSize.y
        , hMemDC
        , 0, 0
        , m_vSize.x, m_vSize.y
        , RGB(255, 255, 255));

}

void CFlameShot::Release()
{
    SafeDelete<CCollider*>(m_pColBox);
}

void CFlameShot::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
    if (_pCol->GetObjectType() == PLAYER)
    {
        m_pColBox->SetEnableCol(false);
        CEri* pPlayer = dynamic_cast<CEri*>(CSceneManager::GetInstance().GetCurPlayer());
        pPlayer->SetCurWeapon(FLAME_SHOT);
        pPlayer->SetArmo(20);
        CSoundManager::GetInstance().PlaySound(L"FlameShot.mp3", ANOUNCE, 0.2f);
        m_bDestroy = true;
        // ธิดย ฦฤฦผลฌ
    }
}
