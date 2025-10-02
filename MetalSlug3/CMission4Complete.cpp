#include "pch.h"
#include "CMission4Complete.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CSoundManager.h"

CMission4Complete::CMission4Complete()
    : m_iIndex(0), m_fDelta(0.f)
{
}

CMission4Complete::~CMission4Complete()
{
    Release();
}

void CMission4Complete::Initialize()
{
    m_vSize = Vector2(768.f, 192.f);
    m_vPivot = Vector2(WINCX >> 1, WINCY >> 1);
    CSoundManager::GetInstance().PlaySound(L"Mission_Complete.mp3", ANOUNCE, 0.5f);
}

int CMission4Complete::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_fDelta += 200 * DELTA;

    return OBJ_NOEVENT;
}

void CMission4Complete::LateUpdate()
{
    if (m_iIndex > 7) return;

    if (m_fDelta >= 100.f)
    {
        m_fDelta = 0.f;
        ++m_iIndex;
    }
}

void CMission4Complete::Render(HDC _hDC)
{
        HDC hMemDC = nullptr;
    if (m_iIndex % 2 == 0)
        hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Mission4Complete");
    else
        hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Mission4Start_Blank");

    int iSizeX = static_cast<int>(m_vSize.x);
    int iSizeY = static_cast<int>(m_vSize.y);

    GdiTransparentBlt(_hDC
        , 96, 264
        , iSizeX, iSizeY
        , hMemDC
        , 0, 0
        , iSizeX, iSizeY
        , RGB(255, 255, 255));

}

void CMission4Complete::Release()
{
}

void CMission4Complete::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
