#include "pch.h"
#include "CTimerUI.h"

#include "CTimeManager.h"
#include "CBmpManager.h"

CTimerUI::CTimerUI()
	: m_bStopTimer(false), m_fGameTime(0.f), m_iTensDigit(0), m_iUnitsDigit(0)
{
}

CTimerUI::~CTimerUI()
{
	Release();
}

void CTimerUI::Initialize()
{
	LoadBmp();
	m_vSize = Vector2(48.f, 48.f);
	m_vPivot = Vector2(static_cast<float>(WINCX / 2), m_vSize.y);
	

	m_fGameTime = 6.f;
}

int CTimerUI::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;

	__super::UpdateGameObject();

	if (m_bDisable || m_bStopTimer) return OBJ_NOEVENT;

	m_fGameTime -= DELTA / 4.f;

	return OBJ_NOEVENT;
}

void CTimerUI::LateUpdate()
{
	if (m_bDisable) return;

	if (m_fGameTime <= 0.f) return;
	ChangeNumberIdx();
}

void CTimerUI::Render(HDC _hDC)
{
	if (m_bDisable) return;
	
	HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Timer");

	int iSizeX = static_cast<int>(m_vSize.x);
	int iSizeY = static_cast<int>(m_vSize.y);

	GdiTransparentBlt(_hDC
		, m_tRect.left - 24, m_tRect.top
		, iSizeX, iSizeY
		, hMemDC
		, m_iTensDigit * 48, 0
		, iSizeX, iSizeY
		, RGB(255, 255, 255));

	GdiTransparentBlt(_hDC
		, m_tRect.left + 24, m_tRect.top
		, iSizeX, iSizeX
		, hMemDC
		, m_iUnitsDigit * 48, 0
		, iSizeY, iSizeY
		, RGB(255, 255, 255));
}

void CTimerUI::Release()
{
}

void CTimerUI::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}

void CTimerUI::LoadBmp()
{
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/UI/Timer/Timer.bmp", L"Timer");
}

void CTimerUI::ChangeNumberIdx()
{
	m_iTensDigit = static_cast<int>(m_fGameTime) / 10;
	m_iUnitsDigit = static_cast<int>(m_fGameTime) % 10;
}
