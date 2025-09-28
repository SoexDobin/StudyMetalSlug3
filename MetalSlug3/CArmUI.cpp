#include "pch.h"
#include "CArmUI.h"
#include "CEri.h"

#include "CBmpManager.h"
#include "CSceneManager.h"


CArmUI::CArmUI()
    : m_szArmoBuffer(L"0"), m_szBombBuffer(L"0")
{
}

CArmUI::~CArmUI()
{
    Release();
}

void CArmUI::Initialize()
{
    LoadBmp();

	m_vPivot = Vector2(336.f, 48.f);
	m_vSize = Vector2(192.f, 64.f);
}

int CArmUI::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CArmUI::LateUpdate()
{
}

void CArmUI::Render(HDC _hDC)
{
	CEri* pPlayer = dynamic_cast<CEri*>(CSceneManager::GetInstance().GetCurPlayer());
	if (m_bDisable || pPlayer == nullptr) return;

	int iSizeX = static_cast<int>(m_vSize.x);
	int iSizeY = static_cast<int>(m_vSize.y);

	HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"ArmBox");
	GdiTransparentBlt(_hDC
		, m_tRect.left, m_tRect.top
		, iSizeX, iSizeY
		, hMemDC
		, 0, 0
		, iSizeX, iSizeY
		, RGB(255, 255, 255));

	HFONT hFont = CreateFontW(
		24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, VARIABLE_PITCH,
		L"Metal Slug (Colour)"
	);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);
	SetBkMode(_hDC, TRANSPARENT);
	
	
	swprintf_s(m_szBombBuffer, L"%d", pPlayer->GetBomb());
	
	RECT tBombRect = m_tRect;
	tBombRect.top += static_cast<int>(m_vSize.y / 2.f);
	tBombRect.right -= static_cast<int>(m_vSize.x / 6.f);

	DrawText(_hDC, m_szBombBuffer, lstrlen(m_szBombBuffer), &tBombRect
		, DT_RIGHT | DT_NOCLIP);
	if (pPlayer->GetArmo() == INFINITE)
	{
		HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Infinity");

		GdiTransparentBlt(_hDC
			, 288, 52
			, 48, 16
			, hMemDC
			, 0, 0
			, 48, 16
			, RGB(255, 255, 255));
	}
	else
	{
		swprintf_s(m_szArmoBuffer, L"%d", pPlayer->GetArmo());

		RECT tArmoRect = m_tRect;
		tArmoRect.top += static_cast<int>(m_vSize.y / 2.f);
		tArmoRect.right -= static_cast<int>(m_vSize.x / 2.f);

		DrawText(_hDC, m_szArmoBuffer, lstrlen(m_szArmoBuffer), &tArmoRect
			, DT_RIGHT | DT_NOCLIP);
	}

	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}

void CArmUI::Release()
{
}

void CArmUI::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}

void CArmUI::LoadBmp()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/UI/ArmBox/ArmBox.bmp", L"ArmBox");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/UI/ArmBox/Infinity.bmp", L"Infinity");

}
