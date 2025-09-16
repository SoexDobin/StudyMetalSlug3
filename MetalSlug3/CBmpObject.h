#pragma once
class CBmpObject
{
public:
	CBmpObject();
	virtual ~CBmpObject();

public:
	HDC		GetMemDC()			{ return m_hMemDC; }

public:
	void	LoadBmp(const TCHAR* pFilePath);
	void	Release();

private:
	HDC		m_hMemDC;
	HBITMAP	m_hBitmap;
	HBITMAP	m_hPrevBmp;
};

