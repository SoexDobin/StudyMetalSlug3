#include "pch.h"
#include "CSDWEffect.h"
#include "CBmpManager.h"

CSDWEffect::CSDWEffect()
	: m_iLivingFrame(0)
{
}

CSDWEffect::~CSDWEffect()
{
}

void CSDWEffect::Initialize()
{
    m_vSize = Vector2(384.f, 192.f);

	m_iLivingFrame = 2;

    __super::UpdateGameObject();
}

int CSDWEffect::Update()
{
	if (m_iLivingFrame == 0)
	{
		SafeDelete<CCollider*>(m_pColBox);
		m_bDestroy = true;
	}
	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();

	--m_iLivingFrame;
	return OBJ_NOEVENT;
}

void CSDWEffect::LateUpdate()
{
}

void CSDWEffect::Render(HDC _hDC)
{
	HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(m_szFrameKey);

	GdiTransparentBlt(_hDC
		, m_tRect.left, m_tRect.top
		, m_vSize.x, m_vSize.y
		, hMemDC
		, 0, 0
		, m_vSize.x, m_vSize.y
		, RGB(255,255,255));
}

void CSDWEffect::Release()
{
}

void CSDWEffect::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
