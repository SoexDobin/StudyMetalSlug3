#include "pch.h"
#include "CEri.h"

// Managers
#include "CBmpManager.h"

CEri::CEri()
{
}

CEri::~CEri()
{
	Release();
}

void CEri::Initialize()
{
	m_vPivot = Vector2(120, 600);
	m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);
	m_vFace = Vector2::UnitX;
	m_vDirection = Vector2(0.f, 0.f);
	
	

	LoadEriBmp();
}

int CEri::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CEri::LateUpdate()
{
	KeyInput();

}

void CEri::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CEri::Release()
{
}

void CEri::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
}

void CEri::KeyInput()
{
}

void CEri::LoadEriBmp()
{
	// sit
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_CQC.bmp", L"Eri_Sit_CQC");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Idle.bmp", L"Eri_Sit_Idle");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Move.bmp", L"Eri_Sit_Move");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Sit_Shoot.bmp", L"Eri_Sit_Shoot");
	// idle
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Idle_Body.bmp", L"Eri_Standing_Idle_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Idle_Leg.bmp", L"Eri_Standing_Idle_Leg");
	// move
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Move_Body.bmp", L"Eri_Standing_Move_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Move_Leg.bmp", L"Eri_Standing_Move_Leg");
	// CQC
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_AxeCQC_Body.bmp", L"Eri_Standing_AxeCQC_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_TonfaCQC_Body.bmp", L"Eri_Standing_TonfaCQC_Body");
	// jump
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Jump_Body.bmp", L"Eri_Standing_Jump_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Jump_Leg.bmp", L"Eri_Standing_Jump_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_JumpFront_Leg.bmp", L"Eri_Standing_JumpFront_Leg");
	// drop
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Drop_Body.bmp", L"Eri_Standing_Drop_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_Drop_Leg.bmp", L"Eri_Standing_Drop_Leg");
	// shoot
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootFront_Body.bmp", L"Eri_Standing_ShootFront_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootUp_Body.bmp", L"Eri_Standing_ShootUp_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootDown_Body.bmp", L"Eri_Standing_ShootDown_Body");

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootFrontToDown_Body.bmp", L"Eri_Standing_ShootFrontToDown_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootDownToFront_Body.bmp", L"Eri_Standing_ShootDownToFront_Body");

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootFrontToUp_Body.bmp", L"Eri_Standing_ShootFrontToUp_Body");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Standing_ShootUpToFront_Body.bmp", L"Eri_Standing_ShootUpToFront_Body");
}
