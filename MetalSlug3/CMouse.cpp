#include "pch.h"
#include "CMouse.h"

#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CMouse::CMouse()
	: m_szCurPosBuffer(L"")
	, m_szFromPosBuffer(L"")
	, m_szToPosBuffer(L"")
	, m_szDistanceBuffer(L"")
{
	Release();
}

CMouse::~CMouse()
{
	ZeroMemory(&m_vCurPos, sizeof(Vector2));
	ZeroMemory(&m_vFromPos, sizeof(Vector2));
	ZeroMemory(&m_vToPos, sizeof(Vector2));
	ZeroMemory(&m_vDistance, sizeof(Vector2));
}

void CMouse::Initialize()
{
	m_eType = PLAYER;
	m_pColBox = nullptr;

	AddFontResourceEx(L"../Resource/Font/metal-slug-colour.colr.ttf", FR_PRIVATE, 0);
}

int CMouse::Update()
{
	UpdateMouseFunction();
	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CMouse::LateUpdate()
{
}

void CMouse::Render(HDC _hDC)
{
	HFONT hFont = CreateFontW(
		24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, VARIABLE_PITCH,
		L"Metal Slug (Colour)" 
	);
	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);
	SetBkMode(_hDC, TRANSPARENT);

	if (m_vDistance != Vector2::Zero)
	{
		POINT tPT{};
		MoveToEx(_hDC, (int)m_vFromPos.x, (int)m_vFromPos.y, &tPT);
		LineTo(_hDC, (int)m_vToPos.x, (int)m_vToPos.y);
	}
	
	CGameObject* pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();

	swprintf_s(m_szCurPosBuffer, L"CurPos( x : %d, y : %d )"
		, (int)m_vCurPos.x - SCROLLX, (int)m_vCurPos.y - SCROLLY);
    TextOut(_hDC, (int)(m_vCurPos.x - 150.f), (int)(m_vCurPos.y - 40.f)
		, m_szCurPosBuffer, lstrlen(m_szCurPosBuffer));

	swprintf_s(m_szFromPosBuffer, L"FromPos( x : %d, y : %d )"
		, (int)m_vFromPos.x, (int)m_vFromPos.y);
	TextOut(_hDC, (int)(m_vCurPos.x - 150.f), (int)(m_vCurPos.y - 60.f)
		, m_szFromPosBuffer, lstrlen(m_szFromPosBuffer));

	swprintf_s(m_szToPosBuffer, L"ToPos( x : %d, y : %d )"
		, (int)m_vToPos.x, (int)m_vToPos.y);
	TextOut(_hDC, (int)(m_vCurPos.x - 150.f), (int)(m_vCurPos.y - 80.f)
		, m_szToPosBuffer, lstrlen(m_szToPosBuffer));

	swprintf_s(m_szDistanceBuffer, L"Player( x : %d, y : %d )"
		, (int)pPlayer->GetPivot().x, (int)pPlayer->GetPivot().y);
	TextOut(_hDC, (int)(m_vCurPos.x - 150.f), (int)(m_vCurPos.y - 100.f)
		, m_szDistanceBuffer, lstrlen(m_szDistanceBuffer));

	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}

void CMouse::Release()
{
	RemoveFontResourceEx(L"../Resource/Font/metal-slug-colour.ttf", FR_PRIVATE, 0);
}

void CMouse::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}

void CMouse::UpdateMouseFunction()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_vCurPos = { (float)pt.x, (float)pt.y};
	if (CKeyManager::GetInstance().KeyDown(VK_LBUTTON))
	{
		m_vDistance = Vector2::Zero;
		m_vFromPos = { (float)pt.x, (float)pt.y};
	}

	if (CKeyManager::GetInstance().KeyDown(VK_RBUTTON))
	{
		m_vToPos = { (float)pt.x, (float)pt.y };
	}

	if (m_vFromPos != Vector2::Zero && m_vToPos != Vector2::Zero)
	{
		m_vDistance = m_vToPos - m_vFromPos;
		m_vFromPos = Vector2::Zero;
		m_vToPos = Vector2::Zero;
	}
}