#include "pch.h"
#include "CEri.h"

CEri::CEri()
{
}

CEri::~CEri()
{
}

void CEri::Initialize()
{
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
