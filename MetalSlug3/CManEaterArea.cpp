#include "pch.h"
#include "CManEaterArea.h"

#include "CColliderFactory.h"

CManEaterArea::CManEaterArea()
	: m_iLivingFrame(0)
{
}

CManEaterArea::~CManEaterArea()
{
	Release();
}

void CManEaterArea::Initialize()
{
	m_eType = EXPLODE;
	m_eDamageFlag = PLAYER;
	m_iDamage = 1;

	m_vSize = Vector2(96.f, 96.f);
	m_pColBox = CColliderFactory::Create(this, HITBOX);

	m_iLivingFrame = 1;
}

int CManEaterArea::Update()
{
	if(m_iLivingFrame == 0)
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

void CManEaterArea::LateUpdate()
{

}

void CManEaterArea::Render(HDC _hDC)
{

}

void CManEaterArea::Release()
{
	SafeDelete<CCollider*>(m_pColBox);
}

void CManEaterArea::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
