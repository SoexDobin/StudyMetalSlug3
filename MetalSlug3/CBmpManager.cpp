#include "pch.h"
#include "CBmpManager.h"
#include "Define.h"

CBmpManager* CBmpManager::m_pInstance = nullptr;

CBmpManager::CBmpManager()
{
}

CBmpManager::~CBmpManager()
{
    Release();
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

void CBmpManager::InsertBmp(const TCHAR* _pFilePath, const TCHAR* _pImgKey)
{
    auto umapIt = find_if(m_umapBitMap.begin(), m_umapBitMap.end()
        , [&](pair<const TCHAR*, CBmpObject*> _it) -> bool
        {
            return !lstrcmp(_it.first, _pImgKey);
        });

    if (m_umapBitMap.end() == umapIt)
    {
        CBmpObject* pBmp = new CBmpObject;
        pBmp->LoadBmp(_pFilePath);
        m_umapBitMap.insert({ _pImgKey, pBmp });
    }
}

HDC CBmpManager::FindBmpImg(const TCHAR* _pImgKey)
{
    auto umapIt = find_if(m_umapBitMap.begin(), m_umapBitMap.end()
        , [&](pair<const TCHAR*, CBmpObject*> _it) -> bool
        {
            return !lstrcmp(_it.first, _pImgKey);
        });

    if (umapIt == m_umapBitMap.end())
        return nullptr;

    return umapIt->second->GetMemDC();
}

void CBmpManager::Release()
{
    for_each(m_umapBitMap.begin(), m_umapBitMap.end()
        , [](pair<const TCHAR*, CBmpObject*> _it) {
            SafeDelete<CBmpObject*>(_it.second);
        });
    m_umapBitMap.clear();
}