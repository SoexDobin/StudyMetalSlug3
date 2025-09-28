#include "pch.h"
#include "CMultiHitUI.h"

#include "CBmpManager.h"

CMultiHitUI::CMultiHitUI()
{
}

CMultiHitUI::~CMultiHitUI()
{
	Release();
}

void CMultiHitUI::Initialize()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/UI/MultiHit/MultiHit.bmp", L"MultiHit");
    m_vPivot = Vector2(120.f, 58.f);
    m_vSize = Vector2(240.f, 48.f);
}

int CMultiHitUI::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CMultiHitUI::LateUpdate()
{
}

void CMultiHitUI::Render(HDC _hDC)
{
    if (m_bDisable) return;

	int iSizeX = static_cast<int>(m_vSize.x);
	int iSizeY = static_cast<int>(m_vSize.y);

	HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"MultiHit");
	GdiTransparentBlt(_hDC
		, m_tRect.left, m_tRect.top
		, iSizeX, iSizeY
		, hMemDC
		, 0, 0
		, iSizeX, iSizeY
		, RGB(255, 255, 255));
}

void CMultiHitUI::Release()
{
}

void CMultiHitUI::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
