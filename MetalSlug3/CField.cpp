#include "pch.h"
#include "CField.h"
#include "CHitBox.h"
#include "CAnimation.h"

// Managers
#include "CColliderFactory.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CField::CField()
    : m_pAnim(nullptr), m_szAnimKey(L"")
{

}

CField::~CField()
{

}

void CField::Initialize()
{
    //m_eType = FIELD;
    m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);
    
}

int CField::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    if (m_pAnim)
    {
        m_pAnim->SetCurrentFrameKey(m_szAnimKey);
        m_pAnim->UpdateAnimation();
    }

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CField::LateUpdate()
{
}

void CField::Render(HDC _hDC)
{
    int iX = (int)CScrollManager::GetInstance().GetScrollX();
    int iY = (int)CScrollManager::GetInstance().GetScrollY();

    HDC	hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field1");

    BitBlt(_hDC,
        m_tRect.left + iX,
        m_tRect.top + iY,
        (int)m_vSize.x,
        (int)m_vSize.y,
        hFieldDC,
        0,
        0,
        SRCCOPY);

    if (m_szAnimKey == L"") return;
    if (m_pAnim) m_pAnim->RenderAnimation(_hDC);
}

void CField::Release()
{
}

void CField::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
    if (_pCol->GetObjectType() == PLAYER
        || _pCol->GetObjectType() == ENEMY)
    {
        _pCol->SetPivot(_pCol->GetPivot() - _vColSize);
    }
}
