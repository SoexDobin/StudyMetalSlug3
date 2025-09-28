#include "pch.h"
#include "CMissionStart4.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
CMissionStart4::CMissionStart4()
    : m_iIndex(0), m_fDelta(0.f)
{
}

CMissionStart4::~CMissionStart4()
{
}

void CMissionStart4::Initialize()
{
    m_vSize = Vector2(768.f, 192.f);
    m_vPivot = Vector2(WINCX >> 1, WINCY >> 1);
    CSoundManager::GetInstance().PlaySoundOnce(L"Mission4_Start.mp3", ANOUNCE, 0.2f);
}

int CMissionStart4::Update()
{
    if (m_bDestroy) return OBJ_DESTROY;

    __super::UpdateGameObject();

    m_fDelta += 200 * DELTA;

    return OBJ_NOEVENT;
}

void CMissionStart4::LateUpdate()
{
    if (m_fDelta >= 100.f)
    {
        m_fDelta = 0.f;
        ++m_iIndex;
    }
    if (m_iIndex > 5)
    {
        m_iIndex = 3;
        m_bDestroy = OBJ_DESTROY;
    }
}

void CMissionStart4::Render(HDC _hDC)
{
    HDC hMemDC = nullptr;
    if (m_iIndex % 2 == 0)
        hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Mission4Start");
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

void CMissionStart4::Release()
{
}

void CMissionStart4::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
