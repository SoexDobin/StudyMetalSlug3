#include "pch.h"
#include "CGame.h"

#include "CGameObject.h"
#include "CBmpObject.h"

// Managers
#include "CSingleTon.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CBmpManager.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CLineManager.h"
#include "CCollisionManager.h"
#include "CScrollManager.h"
#include "CParticleManager.h"

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

	m_hDC = GetDC(g_hWnd);
	GetClientRect(g_hWnd, &m_tRect);
	m_hDCBack = CreateCompatibleDC(m_hDC);
	m_bmpBack = CreateCompatibleBitmap(m_hDC, m_tRect.right, m_tRect.bottom);
	HBITMAP prev = (HBITMAP)::SelectObject(m_hDCBack, m_bmpBack);
	DeleteObject(prev);

	

	CObjectManager::GetInstance().Initialize();
	CSceneManager::GetInstance().Initialize();
	CParticleManager::GetInstance().Initialize();
	CTimeManager::GetInstance().Initialize();
}

void CGame::Update()
{
	CTimeManager::GetInstance().Update();
	CSceneManager::GetInstance().Update();
	CParticleManager::GetInstance().Update();
}

void CGame::LateUpdate()
{
	CSceneManager::GetInstance().LateUpdate();
	CParticleManager::GetInstance().LateUpdate();
	CKeyManager::GetInstance().KeyUpdate();
}

void CGame::Render()
{
	BitBlt(m_hDC, 0, 0, m_tRect.right, m_tRect.bottom, m_hDCBack, 0, 0, SRCCOPY);
	PatBlt(m_hDCBack, 0, 0, m_tRect.right, m_tRect.bottom, WHITENESS);

	CSceneManager::GetInstance().Render(m_hDCBack);
	CParticleManager::GetInstance().Render(m_hDCBack);
	

}

void CGame::Release()
{
#ifdef _DEBUG
FreeConsole();
#endif

	CSceneManager::DeleteInstance();
	CKeyManager::DeleteInstance();
	CBmpManager::DeleteInstance();
	CObjectManager::DeleteInstance();
	CTimeManager::DeleteInstance();
	CLineManager::DeleteInstance();
	CCollisionManager::DeleteInstance();
	CScrollManager::DeleteInstance();
	CParticleManager::DeleteInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
