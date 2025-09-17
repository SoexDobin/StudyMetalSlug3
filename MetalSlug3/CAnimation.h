#pragma once

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render();
	void		Release();

public:
	int			GetStartFrameIndex();
	int			GetEndFrameIndex();
	int			GetCurrentFrameIndex();
	int			GetFrameLayer();


public:
	void			PlayAnimation();
	int				StopAnimation();

protected:
	int				m_iStartMotion;
	int				m_iEndMotion;
	int				m_iCurMotion;
	int				m_iLayer;
	float			m_tDeltaFrame;
	float			m_tFrameSpeed;
	const TCHAR*	m_tCurFrameKey;
	unordered_map<const TCHAR*, pair<int, int>> m_umapAnimationPair;
};

typedef struct tagFrame
{
	int		iStart;
	int		iEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;


}FRAME;
