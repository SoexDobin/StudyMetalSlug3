#include "pch.h"
#include "CLineManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"

CLineManager::CLineManager()
{
	ZeroMemory(&m_tLinePoint, sizeof(LINE));
}

CLineManager::~CLineManager()
{
	Release();
}

bool CLineManager::CollisionLine(float fX, float* pY)
{
	if (m_LineList.empty())
		return false;

	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_LineList)
	{
		if (fX >= pLine->GetLeftPoint().x &&
			fX <= pLine->GetRightPoint().x)
		{
			pTargetLine = pLine;
		}
	}

	if (!pTargetLine)
		return false;


	float x1 = pTargetLine->GetLeftPoint().x;
	float y1 = pTargetLine->GetLeftPoint().y;

	float x2 = pTargetLine->GetRightPoint().x;
	float y2 = pTargetLine->GetRightPoint().y;

	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

void CLineManager::AddLine(Vector2 _vFrom, Vector2 _vTo)
{
	m_LineList.push_back(new CLine(_vFrom, _vTo));
}

void CLineManager::Initialize()
{
	

}

int CLineManager::Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= (int)CScrollManager::GetInstance().GetScrollX();

	if (CKeyManager::GetInstance().KeyDown(VK_LBUTTON))
	{
		if (!m_tLinePoint.vLeft.x && !m_tLinePoint.vLeft.y)
		{
			m_tLinePoint.vLeft.x = (float)pt.x;
			m_tLinePoint.vLeft.y = (float)pt.y;
		}
		else
		{
			m_tLinePoint.vRight.x = (float)pt.x;
			m_tLinePoint.vRight.y = (float)pt.y;

			m_LineList.push_back(new CLine(m_tLinePoint.vLeft, m_tLinePoint.vRight));

			m_tLinePoint.vLeft.x = m_tLinePoint.vRight.x;
			m_tLinePoint.vLeft.x = m_tLinePoint.vRight.y;
		}
	}

	return 0;
}

void CLineManager::LateUpdate()
{

}

void CLineManager::Render(HDC hDC)
{
	for (auto& pLine : m_LineList)
		pLine->Render(hDC);
}

void CLineManager::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), [](CLine* _pLine) 
		{
			if (_pLine)
			{
				delete _pLine;
				_pLine = nullptr;
			}
		});
	m_LineList.clear();
}

void CLineManager::SaveData()
{
	HANDLE	hFile = CreateFile(L"../Data/Line.dat", 
								GENERIC_WRITE,		
								NULL,				
								NULL,				
								CREATE_ALWAYS,		
								FILE_ATTRIBUTE_NORMAL,
								NULL); 

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"Save Failed", _T("Error"), MB_OK);
		return;
	}

	DWORD	dwByte(0);

	for (auto& pLine : m_LineList)
	{
		WriteFile(hFile, &(pLine->GetLine()), sizeof(LINE), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Save Success", _T("√‡«œ"), MB_OK);
}

void CLineManager::LoadData()
{
	HANDLE	hFile = CreateFile(L"../Data/Line.dat", 
		GENERIC_READ,		
		NULL,				
		NULL,				
		OPEN_EXISTING,		
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"Load Failed", _T("Error"), MB_OK);
		return;
	}

	DWORD	dwByte(0);
	LINE	tLine{};

	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(LINE), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_LineList.push_back(new CLine(tLine.vLeft, tLine.vRight));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Load Success", _T("Success"), MB_OK);
}
