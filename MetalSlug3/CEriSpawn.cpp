#include "pch.h"
#include "CEriSpawn.h"
#include "CAnimation.h"

#include "CBmpManager.h"

CEriSpawn::CEriSpawn()
	: m_pSpawnAnim(nullptr)
{
}

CEriSpawn::~CEriSpawn()
{
	Release();
}

void CEriSpawn::Initialize()
{
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Spawn.bmp"
		, L"Eri_Spawn");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Eri/Eri_Spawn_Blank.bmp"
		, L"Eri_Spawn_Blank");
	m_pSpawnAnim = new CAnimation();
	m_pSpawnAnim->SetParent(this);
	m_pSpawnAnim->Initialize();

	m_pSpawnAnim->AddAnimation(L"Eri_Spawn", pair<int, int>{0, 7});
	m_pSpawnAnim->AddAnimation(L"Eri_Spawn_Blank", pair<int, int>{0, 1});
	m_pSpawnAnim->ChangeAnimation(L"Eri_Spawn_Blank");
	m_pSpawnAnim->SetLockFacing(true);
	m_pSpawnAnim->SetFrameSpeed(0.04f);

	m_vSize = Vector2(144.f, 768.f);
}

int CEriSpawn::Update()
{

	__super::UpdateGameObject();
	m_pSpawnAnim->UpdateAnimation();
	
	return OBJ_NOEVENT;
}

void CEriSpawn::LateUpdate()
{
}

void CEriSpawn::Render(HDC _hDC)
{
	m_pSpawnAnim->RenderAnimation(_hDC);
}

void CEriSpawn::Release()
{
	SafeDelete<CAnimation*>(m_pSpawnAnim);
}

void CEriSpawn::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}

