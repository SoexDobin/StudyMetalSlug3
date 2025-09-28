#include "pch.h"
#include "CAnimation.h"
#include "CGameObject.h"

// Managers
#include "CBmpManager.h"
#include "CTimeManager.h"
#include "CScrollManager.h"

CAnimation::CAnimation()
	: m_pParentObject(nullptr), m_iLastFacingX(1), m_bRepeat(true), m_bEndOneLoop(false), m_bLockFacing(false)
	,m_iStartFrame(0), m_iEndFrame(0), m_iCurFrame(0), m_iLayer(0)
	, m_fDeltaFrame(0.f), m_fFrameSpeed(0.f)
	, m_szCurFrameKey(L"")
{
	ZeroMemory(&m_dwEaseColor, sizeof(COLORREF));
	ZeroMemory(&m_vSizeOffset, sizeof(Vector2));
}

CAnimation::~CAnimation()
{
	Release();
}

int CAnimation::GetLastFacingX()				{ return m_iLastFacingX; }
bool CAnimation::GetLoop()						{ return m_bRepeat; }
bool CAnimation::GetEndOneLoop()				{ return m_bEndOneLoop; }
bool CAnimation::GetLockFacing()				{ return m_bLockFacing; }
int CAnimation::GetStartFrameIndex()			{ return m_iStartFrame; }
int CAnimation::GetEndFrameIndex()				{ return m_iEndFrame; }
int CAnimation::GetCurrentFrameIndex()			{ return m_iCurFrame; }
int CAnimation::GetFrameLayer()					{ return m_iLayer; }
float CAnimation::GetDeltaFrame()				{ return m_fDeltaFrame; }
float CAnimation::GetFrameSpeed()				{ return m_fFrameSpeed; }
Vector2 CAnimation::GetSizeOffset()
{
	return m_vSizeOffset;
}
const TCHAR* CAnimation::GetCurrentFrameKey()	{ return m_szCurFrameKey; }

COLORREF CAnimation::GetEaseColor()				{ return m_dwEaseColor; }
void CAnimation::SetParent(CGameObject* _pObj)	{ m_pParentObject = _pObj; }
void CAnimation::SetLastFacingX(int _idx)		{ m_iLastFacingX = _idx; }
void CAnimation::SetLoop(bool _bIsRepeat)		{ m_bRepeat = _bIsRepeat; }
void CAnimation::SetEndOneLoop(bool _bstate)	{ m_bEndOneLoop = _bstate; }
void CAnimation::SetLockFacing(bool _bIsLock)	{ m_bLockFacing = _bIsLock; }
void CAnimation::SetStartFrameIndex(int _idx)	{ m_iStartFrame = _idx; }
void CAnimation::SetEndFrameIndex(int _idx)		{ m_iEndFrame = _idx; }
void CAnimation::SetCurrentFrameIndex(int _idx)	{ m_iCurFrame = _idx; }
void CAnimation::SetFrameLayer(int _idx)		{ m_iLayer = _idx; }
void CAnimation::SetDeltaFrame(float _fDelta)	{ m_fDeltaFrame = _fDelta; }
void CAnimation::SetFrameSpeed(float _fSpeed)	{ m_fFrameSpeed = _fSpeed; }
void CAnimation::SetSizeOffset(const Vector2& _vSizeeoff)
{
	m_vSizeOffset = _vSizeeoff;
}
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
	if (m_pParentObject->GetFace().x > 0 && !m_bLockFacing)
	{
		m_iLastFacingX = static_cast<int>(m_pParentObject->GetFace().x);
		m_iLayer = 0;
	}
	else if (m_pParentObject->GetFace().x < 0 && !m_bLockFacing)
	{
		m_iLastFacingX = static_cast<int>(m_pParentObject->GetFace().x);
		m_iLayer = 1; 
	}
	else if (m_bLockFacing == false)
	{
		if (m_iLastFacingX > 0) m_iLayer = 0;
		else m_iLayer = 1;
	}
		
	if (m_bRepeat == false && m_iCurFrame == m_iEndFrame - 1) 
	{
		m_bEndOneLoop = true;
		return;
	}

	m_fDeltaFrame += DELTA;
	
	if (m_fDeltaFrame >= m_fFrameSpeed)
	{
		++m_iCurFrame;

		if (m_iCurFrame >= m_iEndFrame)
		{
			m_bEndOneLoop = true;
			m_iCurFrame = m_iStartFrame;
		}
			
		m_fDeltaFrame = 0.f;
	}
}

void CAnimation::ChangeAnimation(const TCHAR* _szKey, int _iStartFrame)
{
	if (!lstrcmp(m_szCurFrameKey, _szKey)) return;

	m_bEndOneLoop = false;
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
	if (m_szCurFrameKey == L"" || m_szCurFrameKey == nullptr) return;

	int iImgSizeX = static_cast<int>(m_pParentObject->GetSize().x - m_vSizeOffset.x);
	int iImgSizeY = static_cast<int>(m_pParentObject->GetSize().y - m_vSizeOffset.y);

	HDC	hMemDC = CBmpManager::GetInstance().FindBmpImg(m_szCurFrameKey);

	GdiTransparentBlt(_hDC,
		m_pParentObject->GetRect().left + (static_cast<int>(m_vSizeOffset.x) / 2) + SCROLLX,
		m_pParentObject->GetRect().top + (static_cast<int>(m_vSizeOffset.y) / 2) + SCROLLY,
		iImgSizeX,
		iImgSizeY,
		hMemDC,
		m_iCurFrame * iImgSizeX,
		m_iLayer * iImgSizeY,
		iImgSizeX,
		iImgSizeY,
		m_dwEaseColor);
}

void CAnimation::Release()
{

}

void CAnimation::PlayAnimation()
{
}

int CAnimation::StopAnimation()
{
	return 0;
}
