#include "pch.h"
#include "CBmpObject.h"
#include "Define.h"

CBmpObject::CBmpObject()
	: m_hMemDC(nullptr), m_hBitmap(nullptr), m_hPrevBmp(nullptr)
{

}

CBmpObject::~CBmpObject()
{
	Release();
}

void CBmpObject::LoadBmp(const TCHAR* _pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap 
		= (HBITMAP)LoadImage
		(NULL,		
		_pFilePath,  
		IMAGE_BITMAP,  
		0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hPrevBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

#ifdef _DEBUG
	if (m_hBitmap == nullptr)
	{
		std::wcout << L"Can not find bmp! at : " << _pFilePath << L"\n";
	}
#endif
}

void CBmpObject::Release()
{
	SelectObject(m_hMemDC, (HBITMAP)m_hPrevBmp);
	DeleteDC(m_hMemDC);
	DeleteObject(m_hBitmap);
}