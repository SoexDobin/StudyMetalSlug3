#include "pch.h"
#include "CGame.h"

#include "CGameObject.h"
#include "CBmpObject.h"

// Managers
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CBmpManager.h"
#include "CTimeManager.h"

CGame::CGame()
	: m_hDC(nullptr)
	, m_hDCBack(nullptr), m_bmpBack(nullptr)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CGame::~CGame()
{
}

void CGame::Initialize()
{
#ifdef _DEBUG
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}
#endif
	GetClientRect(g_hWnd, &m_tRect);
	m_hDCBack = CreateCompatibleDC(m_hDC);
	m_bmpBack = CreateCompatibleBitmap(m_hDC, m_tRect.right, m_tRect.bottom);
	HBITMAP prev = (HBITMAP)::SelectObject(m_hDCBack, m_bmpBack);
	DeleteObject(prev);

	m_hDC = GetDC(g_hWnd);

	CTimeManager::GetInstance().Initialize();
}

void CGame::Update()
{
	CTimeManager::GetInstance().Update();
}

void CGame::LateUpdate()
{

	CKeyManager::GetInstance().KeyUpdate();

}

void CGame::Render()
{
	BitBlt(m_hDC, 0, 0, m_tRect.right, m_tRect.bottom, m_hDCBack, 0, 0, SRCCOPY);
	PatBlt(m_hDCBack, 0, 0, m_tRect.right, m_tRect.bottom, WHITENESS);

	//HDC	hBackDC = BmpMgr::Get_Instance()->Find_Img(L"Back");

	//SceneMgr::Get_Instance()->Render(hBackDC);

	//BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CGame::Release()
{
#ifdef _DEBUG
FreeConsole();
#endif
	CKeyManager::DeleteInstance();
	CBmpManager::DeleteInstance();
	CObjectManager::DeleteInstance();
	CTimeManager::DeleteInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
