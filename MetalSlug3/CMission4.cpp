#include "pch.h"
#include "CMission4.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"

CMission4::CMission4() 
    : m_fDelta(0.f)
{
}

CMission4::~CMission4()
{
}

void CMission4::Initialize()
{
    LoadImageLandscape();
}

pair<bool, SCENETAG> CMission4::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};


    CObjectManager::GetInstance().Update();

    m_fDelta += 4000.f * CTimeManager::GetInstance().GetDeltaTime();

    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CMission4::LateUpdate()
{
    CObjectManager::GetInstance().LateUpdate();
}

void CMission4::Render(HDC _hDC)
{
    RenderLandscape(_hDC);

    CObjectManager::GetInstance().Render(_hDC);

    RenderFrontLandscape(_hDC);
}

void CMission4::Release()
{
}

void CMission4::RenderLandscape(HDC _hDC)
{
    int iX = static_cast<int>(CScrollManager::GetInstance().GetScrollX());
    int iY = static_cast<int>(CScrollManager::GetInstance().GetScrollY());

    HDC	hBackDC = nullptr;
    HDC	hFieldDC = nullptr;
    int idx = 0;
    for (int i = 0; i < 2; ++i)
    {
        hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Background");
        BitBlt(_hDC,
            1536 * i + iX / 4,
            0 + iY / 4,
            1536,
            384,
            hBackDC,
            0,
            0,
            SRCCOPY);
    }
    for (auto it = m_DessertBackgroundKeyList.begin();
        it != m_DessertBackgroundKeyList.end(); ++it)
    {
        hBackDC = CBmpManager::GetInstance().FindBmpImg(*it);
        GdiTransparentBlt(_hDC
            , 768 * idx + iX / 2, WINCY - 574 + iY
            , 768, 384
            , hBackDC
            , 0, 0
            , 768, 384
            , RGB(255, 255, 255));
        idx++;
    }
    for (int i = 0; i < 1; ++i)
    {
        hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Background5");
        GdiTransparentBlt(_hDC
            , 1536 * i + iX / 3, WINCY - 384 + iY / 3
            , 1536, 384
            , hBackDC
            , 0, 0
            , 1536, 384
            , RGB(255, 255, 255));
    }
    for (int i = 0; i < 4; ++i)
    {
        if (i % 2 == 0)
            hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field1");
        else
            hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field2");
    
        GdiTransparentBlt(_hDC
            , 768 * i + iX, WINCY - 256 + iY
            , 768, 192
            , hFieldDC
            , 0, 0
            , 768, 192
            , RGB(255, 255, 255));
    }
    
    
    if (m_fDelta >= 300.f)
    {
        m_DessertBackgroundKeyList.push_front(m_DessertBackgroundKeyList.back());
        m_DessertBackgroundKeyList.pop_back();
        m_fDelta = 0.f;
    }
}

void CMission4::RenderFrontLandscape(HDC _hDC)
{
    int iX = static_cast<int>(CScrollManager::GetInstance().GetScrollX());
    int iY = static_cast<int>(CScrollManager::GetInstance().GetScrollY());

    HDC	hFrontDC = nullptr;
    for (int i = 0; i < 4; ++i)
    {
        hFrontDC = CBmpManager::GetInstance().FindBmpImg(m_vecDessertFrontgroundKey[i]);

        int iScroll = static_cast<float>(iX * 1.2f);
        GdiTransparentBlt(_hDC
            , 768 * i + iScroll, WINCY - 192 + iY
            , 768, 192
            , hFrontDC
            , 0, 0
            , 768, 192
            , RGB(255, 255, 255));
    }
}

void CMission4::LoadImageLandscape()
{

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Dessert_Background.bmp"
        , L"4-1_Dessert_Background");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Dessert_Background1.bmp"
        , L"4-1_Dessert_Background1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Dessert_Background2.bmp"
        , L"4-1_Dessert_Background2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Dessert_Background3.bmp"
        , L"4-1_Dessert_Background3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Dessert_Background4.bmp"
        , L"4-1_Dessert_Background4");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-1_Dessert_Field1.bmp"
        , L"4-1_Dessert_Field1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-1_Dessert_Field2.bmp"
        , L"4-1_Dessert_Field2");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Dessert_Frontground1.bmp"
        , L"4-1_Dessert_Frontground1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Dessert_Frontground2.bmp"
        , L"4-1_Dessert_Frontground2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Dessert_Frontground3.bmp"
        , L"4-1_Dessert_Frontground3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Dessert_Frontground4.bmp"
        , L"4-1_Dessert_Frontground4");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Dessert_Background5.bmp"
        , L"4-1_Dessert_Background5");



    m_DessertBackgroundKeyList.push_back(L"4-1_Dessert_Background1");
    m_DessertBackgroundKeyList.push_back(L"4-1_Dessert_Background2");
    m_DessertBackgroundKeyList.push_back(L"4-1_Dessert_Background3");
    m_DessertBackgroundKeyList.push_back(L"4-1_Dessert_Background4");

    m_vecDessertFrontgroundKey.push_back(L"4-1_Dessert_Frontground1");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Dessert_Frontground2");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Dessert_Frontground3");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Dessert_Frontground4");
}
