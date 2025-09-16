#pragma once
#include "Define.h"

class CGame
{
public:
	CGame();
	~CGame();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render();
	void		Release();

private:
	HDC			m_hDC;

	RECT	    m_tRect;
	HDC			m_hDCBack;
	HBITMAP		m_bmpBack;
};

