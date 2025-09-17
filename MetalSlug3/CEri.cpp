#include "pch.h"
#include "CEri.h"

// Managers
#include "CBmpManager.h"

CEri::CEri()
{
}

CEri::~CEri()
{
}

void CEri::Initialize()
{
	m_vPivot = Vector2(120, 600);
	m_vSize = Vector2(PLAYER_BMPX, PLAYER_BMPY);
	m_vFace = Vector2::UnitX;
	m_vDirection = Vector2(0.f, 0.f);
	
	CBmpManager::GetInstance().InsertBmp();

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
