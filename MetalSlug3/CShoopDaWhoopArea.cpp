#include "pch.h"
#include "CShoopDaWhoopArea.h"

#include "CColliderFactory.h"
#include "CParticleManager.h"

CShoopDaWhoopArea::CShoopDaWhoopArea()
	: m_iLivingFrame(0)
{
}

CShoopDaWhoopArea::~CShoopDaWhoopArea()
{
}

void CShoopDaWhoopArea::Initialize()
{
	m_eType = EXPLODE;
	m_eDamageFlag = PLAYER;
	m_iDamage = 1;

	m_vSize = Vector2(128.f, 196.f);
	m_pColBox = CColliderFactory::Create(this, HITBOX);
	m_pColBox->SetOffset(Vector2(0.f, 0.f));

	m_iLivingFrame = 2;
}

int CShoopDaWhoopArea::Update()
{
	if (m_iLivingFrame == 0)
	{
		m_pColBox->SetEnableCol(false);
		SafeDelete<CCollider*>(m_pColBox);
		m_bDestroy = true;
	}
	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();
	m_pColBox->UpdateCollision();



	--m_iLivingFrame;
	return OBJ_NOEVENT;
}

void CShoopDaWhoopArea::LateUpdate()
{
}

void CShoopDaWhoopArea::Render(HDC _hDC)
{
}

void CShoopDaWhoopArea::Release()
{
	SafeDelete<CCollider*>(m_pColBox);
}

void CShoopDaWhoopArea::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
