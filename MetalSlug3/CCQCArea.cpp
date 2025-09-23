#include "pch.h"
#include "CCQCArea.h"
#include "CHitBox.h"

#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CScrollManager.h"

CCQCArea::CCQCArea() 
	: m_bCatchEnemy(true)
{
}

CCQCArea::~CCQCArea()
{
	Release();
}

void CCQCArea::Initialize()
{
	m_vPivot = m_pParent->GetPivot();
	m_vSize = Vector2(192.f, 192.f);
	m_pColBox = CColliderFactory::Create(this, HITBOX);
	m_pColBox->SetOffset(Vector2( 0.f, 48.f ));
}

int CCQCArea::Update()
{
	if (m_pParent == nullptr) return OBJ_NOEVENT;

	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CCQCArea::LateUpdate()
{
}

void CCQCArea::Render(HDC _hDC)
{
	m_bCatchEnemy = false;
}

void CCQCArea::Release()
{
	m_pParent = nullptr;
}

void CCQCArea::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	if (_pCol->GetObjectType() == ENEMY)
	{
		m_pColBox->GetIsCollision();
		m_bCatchEnemy = true;
	}
}
