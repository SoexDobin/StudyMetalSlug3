#include "pch.h"
#include "CTimeManager.h"

#ifdef _DEBUG
#include "CKeyManager.h" 
#endif

CTimeManager::CTimeManager() 
	: m_fDeltaTime(0.f)
	, m_liCurCount({ 0 }), m_liPrevCount({ 0 }), m_liFrequency({ 0 })
{
	ZeroMemory(&m_liCurCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_liPrevCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_liFrequency, sizeof(LARGE_INTEGER));
}

CTimeManager::~CTimeManager()
{

}

void CTimeManager::Initialize()
{
	QueryPerformanceCounter(&m_liPrevCount);
	QueryPerformanceFrequency(&m_liFrequency);
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&m_liCurCount);

	m_fDeltaTime = (m_liCurCount.QuadPart - m_liPrevCount.QuadPart) / static_cast<float>(m_liFrequency.QuadPart);
	m_liPrevCount = m_liCurCount;

#ifdef _DEBUG
	if (CKeyManager::GetInstance().KeyPressing('K'))
		std::wcout << m_fDeltaTime << L"\n";
#endif // _DEBUG
}