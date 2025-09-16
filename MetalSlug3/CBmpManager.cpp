#include "pch.h"
#include "CBmpManager.h"

CBmpManager* CBmpManager::m_pInstance = nullptr;

CBmpManager::CBmpManager()
{
}

CBmpManager::~CBmpManager()
{
}

CBmpManager& CBmpManager::GetInstance()
{
    if (m_pInstance == nullptr)
        m_pInstance = new CBmpManager;

    return *m_pInstance;
}

void CBmpManager::DeleteInstance()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
        
}
