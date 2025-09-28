#include "pch.h"
#include "CDummy.h"

#include "CColliderFactory.h"


CDummy::CDummy()
	: m_bColWithPlayer(false)
{
    m_pColBox = CColliderFactory::Create(this, HITBOX);
}

CDummy::~CDummy()
{
	Release();
}

void CDummy::Initialize()
{
}

int CDummy::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    return OBJ_NOEVENT;
}

void CDummy::LateUpdate()
{
    
}

void CDummy::Render(HDC _hDC)
{
	m_bColWithPlayer = false;
}

void CDummy::Release()
{
	SafeDelete<CCollider*>(m_pColBox);
}

void CDummy::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	if (m_bColWithPlayer) return;

	if (_pCol->GetObjectType() == PLAYER)
	{
		m_bColWithPlayer = true;
	}
	else
		m_bColWithPlayer = false;
}
