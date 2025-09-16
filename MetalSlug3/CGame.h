#pragma once
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

};

