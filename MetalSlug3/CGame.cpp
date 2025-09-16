#include "pch.h"
#include "CGame.h"

// Managers
#include "CKeyManager.h"
#include "CGameObject.h"

CGame::CGame()
{
}

CGame::~CGame()
{
}

void CGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

}

void CGame::Update()
{
}

void CGame::LateUpdate()
{

	CKeyManager::GetInstance().KeyUpdate();

}

void CGame::Render()
{
	//HDC	hBackDC = BmpMgr::Get_Instance()->Find_Img(L"Back");

	//SceneMgr::Get_Instance()->Render(hBackDC);

	//BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CGame::Release()
{
	CKeyManager::DeleteInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
