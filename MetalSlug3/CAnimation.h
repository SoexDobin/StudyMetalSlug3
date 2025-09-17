#pragma once

class CGameObject;

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

public:
	int				GetStartFrameIndex();
	int				GetEndFrameIndex();
	int				GetCurrentFrameIndex();
	int				GetFrameLayer();
	float			GetDeltaFrame();
	float			GetFrameSpeed();
	const TCHAR*	GetCurrentFrameKey();
	COLORREF		GetEaseColor();

	void			SetParent(CGameObject* _pObj);
	void			SetStartFrameIndex(int _idx);
	void			SetEndFrameIndex(int _idx);
	void			SetCurrentFrameIndex(int _idx);
	void			SetFrameLayer(int _idx);
	void			SetDeltaFrame(float _fDelta);
	void			SetFrameSpeed(float _fSpeed);
	void			SetCurrentFrameKey(const TCHAR* _szkey);
	void			SetEaseColor(COLORREF _dwRGB);

	void			AddAnimation(const TCHAR* _szKey, pair<int, int> _motionPair);
	void			DeleteAnimation(const TCHAR* _szKey);
	pair<int, int>	GetAnimationPairByTString(const TCHAR* _szKey);

public:
	void			Initialize();
	void			UpdateAnimation();
	void			ChangeAnimation(const TCHAR* _szKey, int _iStartFrame = 0);
	void			RenderAnimation(HDC _hDC);
	void			Release();

	void			PlayAnimation();
	int				StopAnimation();

private:
	int				m_iLastFacingX;

protected:
	CGameObject*	m_pParentObject;
	int				m_iStartFrame;
	int				m_iEndFrame;
	int				m_iCurFrame;
	int				m_iLayer;
	float			m_fDeltaFrame;
	float			m_fFrameSpeed;
	const TCHAR*	m_szCurFrameKey;
	COLORREF		m_dwEaseColor;
	unordered_map<const TCHAR*, pair<int, int>> m_umapAnimationPair;
};