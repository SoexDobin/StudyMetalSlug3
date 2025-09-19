#include "pch.h"
#include "CCQCArea.h"
#include "CHitBox.h"

#include "CObjectManager.h"
#include "CColliderFactory.h"
#include "CScrollManager.h"

CCQCArea::CCQCArea() 
	: m_pPlayer(nullptr), m_bCatchEnemy(true)
{
}

CCQCArea::~CCQCArea()
{
	Release();
}

void CCQCArea::Initialize()
{
	m_pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();
	m_vPivot = m_pPlayer->GetPivot();
	m_vSize = Vector2(192.f, 192.f);
	m_pColBox = CColliderFactory<CHitBox>::CreateHitBox(this);
}

int CCQCArea::Update()
{
	if (m_pPlayer == nullptr) return OBJ_NOEVENT;

	m_vPivot = m_pPlayer->GetPivot();

	__super::UpdateGameObject();

	return OBJ_NOEVENT;
}

void CCQCArea::LateUpdate()
{
}

void CCQCArea::Render(HDC _hDC)
{
	int iX = CScrollManager::GetInstance().GetScrollX();
	int iY = CScrollManager::GetInstance().GetScrollY();

	HPEN hGreenPen		= CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN hPrevPen		= (HPEN)SelectObject(_hDC, hGreenPen);
	HBRUSH hNullBrush	= (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hPrevBrush	= (HBRUSH)SelectObject(_hDC, hNullBrush);

	Rectangle(_hDC, m_tRect.left + iX, m_tRect.top + iY, m_tRect.right + iX, m_tRect.bottom + iY);

	SelectObject(_hDC, hPrevPen);
	SelectObject(_hDC, hPrevBrush);
	DeleteObject((HPEN)hGreenPen);
	DeleteObject((HBRUSH)hNullBrush);

	m_bCatchEnemy = false;
}

void CCQCArea::Release()
{
	m_pPlayer = nullptr;
}

void CCQCArea::OnCollision(CGameObject* _pCol, Vector2 _vColSize)
{
	if (_pCol->GetObjectType() == ENEMY)
	{
		m_pColBox->GetIsCollision();
		m_bCatchEnemy = true;
	}
}
