#include "pch.h"
#include "CAnimation.h"
#include "CGameObject.h"

CAnimation::CAnimation()
	: m_pParentObject(nullptr)
	,m_iStartFrame(0), m_iEndFrame(0), m_iCurFrame(0), m_iLayer(0)
	, m_fDeltaFrame(0.f), m_fFrameSpeed(0.f)
	, m_fCurFrameKey(L"")
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
const TCHAR* CAnimation::GetCurrentFrameKey()	{ return m_fCurFrameKey; }

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
	m_fCurFrameKey = _szkey;
}
void CAnimation::SetEaseColor(COLORREF& _dwRGB)
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
	m_fCurFrameKey = m_umapAnimationPair.begin()->first;
}

void CAnimation::UpdateAnimation(const TCHAR* _szKey, int _iStartFrame)
{

}

void CAnimation::RenderAnimation(HDC _hDC)
{
	if (m_pParentObject == nullptr) return;
	RECT tRect = m_pParentObject->GetRect();
	GdiTransparentBlt(_hDC,
		tRect.left, //+ iScrollX,
		tRect.top,	//+ iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iStart * (int)m_tInfo.fCX,
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));
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
