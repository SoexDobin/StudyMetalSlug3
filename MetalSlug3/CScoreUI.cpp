#include "pch.h"
#include "CScoreUI.h"

#include "CScoreManager.h"

CScoreUI::CScoreUI()
	: m_szScoreBuffer(L"0")
{
}

CScoreUI::~CScoreUI()
{
    Release();
}

void CScoreUI::Initialize()
{
    m_vPivot = Vector2(112.f, 24.f);
	m_vSize = Vector2(240.f, 24.f);
}

int CScoreUI::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CScoreUI::LateUpdate()
{
}

void CScoreUI::Render(HDC _hDC)
{
	HFONT hFont = CreateFontW(
		36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, VARIABLE_PITCH,
		L"Metal Slug (Colour)"
	);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);
	SetBkMode(_hDC, TRANSPARENT);

	swprintf_s(m_szScoreBuffer, L"%d", CScoreManager::GetInstance().GetScore());
		
	DrawText(_hDC, m_szScoreBuffer, lstrlen(m_szScoreBuffer), &m_tRect
		, DT_RIGHT | DT_NOCLIP);

	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}

void CScoreUI::Release()
{
	
}

void CScoreUI::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
