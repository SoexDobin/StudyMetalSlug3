#include "pch.h"
#include "CAnimation.h"
#include "CGameObject.h"

// Managers
#include "CBmpManager.h"
#include "CTimeManager.h"

CAnimation::CAnimation()
	: m_pParentObject(nullptr), m_iLastFacingX(1)
	,m_iStartFrame(0), m_iEndFrame(0), m_iCurFrame(0), m_iLayer(0)
	, m_fDeltaFrame(0.f), m_fFrameSpeed(0.f)
	, m_szCurFrameKey(L"")
{
	ZeroMemory(&m_dwEaseColor, sizeof(COLORREF));
}

CAnimation::~CAnimation()
{
	Release();
}

int CAnimation::GetStartFrameIndex()			{ return m_iStartFrame; }
int CAnimation::GetEndFrameIndex()				{ return m_iEndFrame; }
int CAnimation::GetCurrentFrameIndex()			{ return m_iCurFrame; }
int CAnimation::GetFrameLayer()					{ return m_iLayer; }
float CAnimation::GetDeltaFrame()				{ return m_fDeltaFrame; }
float CAnimation::GetFrameSpeed()				{ return m_fFrameSpeed; }
const TCHAR* CAnimation::GetCurrentFrameKey()	{ return m_szCurFrameKey; }

COLORREF CAnimation::GetEaseColor()				{ return m_dwEaseColor; }
void CAnimation::SetParent(CGameObject* _pObj)	{ m_pParentObject = _pObj; }
void CAnimation::SetStartFrameIndex(int _idx)	{ m_iStartFrame = _idx; }
void CAnimation::SetEndFrameIndex(int _idx)		{ m_iEndFrame = _idx; }
void CAnimation::SetCurrentFrameIndex(int _idx)	{ m_iCurFrame = _idx; }
void CAnimation::SetFrameLayer(int _idx)		{ m_iLayer = _idx; }
void CAnimation::SetDeltaFrame(float _fDelta)	{ m_fDeltaFrame = _fDelta; }
void CAnimation::SetFrameSpeed(float _fSpeed)	{ m_fFrameSpeed = _fSpeed; }
void CAnimation::SetCurrentFrameKey(const TCHAR* _szkey)
{
	m_szCurFrameKey = _szkey;
}
void CAnimation::SetEaseColor(COLORREF _dwRGB)
{
	m_dwEaseColor = _dwRGB;
}

void CAnimation::AddAnimation(const TCHAR* _szKey, pair<int, int> _motionPair)
{
	m_umapAnimationPair.insert
	(
		pair<const TCHAR*, pair<int, int>>{ _szKey, _motionPair }
	);
}

void CAnimation::DeleteAnimation(const TCHAR* _szKey)
{
	m_umapAnimationPair.erase(m_umapAnimationPair.find(_szKey));
}

pair<int, int> CAnimation::GetAnimationPairByTString(const TCHAR* _szKey)
{
	return m_umapAnimationPair.find(_szKey)->second;
}

void CAnimation::Initialize()
{
	m_dwEaseColor = RGB( 255,255,255 );
	m_fFrameSpeed = 0.1f;
}

void CAnimation::UpdateAnimation()
{
	if (m_pParentObject->GetFace().x > 0)
	{
		m_iLastFacingX = static_cast<int>(m_pParentObject->GetFace().x);
		m_iLayer = 0;
	}
	else if (m_pParentObject->GetFace().x < 0)
	{
		m_iLastFacingX = static_cast<int>(m_pParentObject->GetFace().x);
		m_iLayer = 1; 
	}
	else
	{
		if (m_iLastFacingX > 0) m_iLayer = 0;
		else m_iLayer = 1;
	}
		
	
	m_fDeltaFrame += CTimeManager::GetInstance().GetDeltaTime();
	
	if (m_fDeltaFrame >= m_fFrameSpeed)
	{
		++m_iCurFrame;

		if (m_iCurFrame >= m_iEndFrame)
			m_iCurFrame = m_iStartFrame;

		m_fDeltaFrame = 0.f;
	}
}

void CAnimation::ChangeAnimation(const TCHAR* _szKey, int _iStartFrame)
{
	if (!lstrcmp(m_szCurFrameKey, _szKey)) return;

	auto it = m_umapAnimationPair.find(_szKey);
	m_szCurFrameKey = _szKey;
	m_iStartFrame = it->second.first;
	m_iEndFrame = it->second.second;

	if (_iStartFrame > 0)
		m_iCurFrame = _iStartFrame;
	else
		m_iCurFrame = m_iStartFrame;
}

void CAnimation::RenderAnimation(HDC _hDC)
{
	if (m_pParentObject == nullptr) return;

	HDC	hMemDC = CBmpManager::GetInstance().FindBmpImg(m_szCurFrameKey);

	GdiTransparentBlt(_hDC,
		m_pParentObject->GetRect().left, //+ iScrollX,
		m_pParentObject->GetRect().top,	//+ iScrollY,
		(int)m_pParentObject->GetSize().x,
		(int)m_pParentObject->GetSize().y,
		hMemDC,
		m_iCurFrame * (int)m_pParentObject->GetSize().x,
		m_iLayer * (int)m_pParentObject->GetSize().y,
		(int)m_pParentObject->GetSize().x,
		(int)m_pParentObject->GetSize().y,
		m_dwEaseColor);
}

void CAnimation::Release()
{
	m_pParentObject = nullptr;
}

void CAnimation::PlayAnimation()
{
}

int CAnimation::StopAnimation()
{
	return 0;
}
