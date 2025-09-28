#include "pch.h"
#include "CCQCArea.h"

#include "CColliderFactory.h"
#include "CScoreManager.h"

CCQCArea::CCQCArea()
	: livingFrame(0)
{
}

CCQCArea::~CCQCArea()
{
}

void CCQCArea::Initialize()
{
	m_eType = EXPLODE;
	m_eDamageFlag = ENEMY;
	m_iDamage = 3;

	m_vSize = Vector2(128.f,128.f);
	m_pColBox = CColliderFactory::Create(this, HITBOX);
	m_pColBox->SetOffset(Vector2(0.f, 64.f));

	livingFrame = 1;
	CScoreManager::GetInstance().ACCScore(500);
}

int CCQCArea::Update()
{
	if (livingFrame == 0)
	{
		m_pColBox->SetEnableCol(false);
		SafeDelete<CCollider*>(m_pColBox);
		m_bDestroy = true;
	}
	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();
	m_pColBox->UpdateCollision();


	
	--livingFrame;
	return OBJ_NOEVENT;
}

void CCQCArea::LateUpdate()
{
}

void CCQCArea::Render(HDC _hDC)
{
}

void CCQCArea::Release()
{
	SafeDelete<CCollider*>(m_pColBox);
}

void CCQCArea::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
