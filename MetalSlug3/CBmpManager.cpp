#include "pch.h"
#include "CBmpManager.h"
#include "Define.h"

CBmpManager::CBmpManager()
{
}

CBmpManager::~CBmpManager()
{
    Release();
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