#include "pch.h"
#include "CCQCChecker.h"
#include "CHitBox.h"

#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CScrollManager.h"

CCQCChecker::CCQCChecker() 
	: m_bCatchEnemy(true)
{
}

CCQCChecker::~CCQCChecker()
{
	Release();
}

void CCQCChecker::Initialize()
{
	m_vPivot = m_pParent->GetPivot();
	m_vSize = Vector2(192.f, 192.f);
	m_pColBox = CColliderFactory::Create(this, HITBOX);
	m_pColBox->SetOffset(Vector2( 0.f, 48.f ));
}

int CCQCChecker::Update()
{
	if (m_pParent == nullptr) return OBJ_DESTROY;

	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CCQCChecker::LateUpdate()
{
}

void CCQCChecker::Render(HDC _hDC)
{
	m_bCatchEnemy = false;
}

void CCQCChecker::Release()
{
	m_pParent = nullptr;
}

void CCQCChecker::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
	if (_pCol->GetObjectType() == ENEMY)
	{
		m_pColBox->GetIsCollision();
		m_bCatchEnemy = true;
	}
}
