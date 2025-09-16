#include "pch.h"
#include "CTimeManager.h"

CTimeManager* CTimeManager::m_pInstance = nullptr;

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

}

CTimeManager& CTimeManager::GetInstance()
{
	if (m_pInstance == nullptr)
		m_pInstance = new CTimeManager;

	return *m_pInstance;
}

void CTimeManager::DeleteInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}