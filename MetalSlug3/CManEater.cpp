#include "pch.h"
#include "CManEater.h"
#include "CHitBox.h"

#include "CColliderFactory.h"
#include "CScrollManager.h"

CManEater::CManEater()
{
}

CManEater::~CManEater()
{
}

void CManEater::Initialize()
{
    m_vPivot = Vector2(500, 500);
    m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);
    m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);
}

int CManEater::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;


    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CManEater::LateUpdate()
{
}

void CManEater::Render(HDC _hDC)
{
    int iX = CScrollManager::GetInstance().GetScrollX();
    int iY = CScrollManager::GetInstance().GetScrollY();

    Rectangle(_hDC, m_tRect.left + iX, m_tRect.top + iY, m_tRect.right + iX, m_tRect.bottom + iY);
}

void CManEater::Release()
{
}

void CManEater::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
}
