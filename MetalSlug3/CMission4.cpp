#include "pch.h"
#include "CMission4.h"
#include "CManEater.h"
#include "CPlatform.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CGameObjectFactory.h"
#include "CSoundManager.h"
#include "CLineManager.h"

CMission4::CMission4() 
    : m_fDelta(0.f), m_iRuinBGIdx(0)
{
}

CMission4::~CMission4()
{
    Release();
}

void CMission4::Initialize()
{
    LoadBmpDessertLandscape();
    LoadBmpEnemy();
    
    CreatePlatform();
    for (int i = 1; i < 5; ++i)
    {
        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CManEater>::Create(), ENEMY);
        CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(i * 600.f, 100.f));
    }
    
    CSoundManager::GetInstance().PlayBGM(L"BGM_OST_Desert.mp3", 0.2f);

    CScrollManager::GetInstance().SetMinScrollLockX(0.f);
    CScrollManager::GetInstance().SetMaxScrollLockX(6526.f);

    CScrollManager::GetInstance().SetMinScrollLockY(0);
    CScrollManager::GetInstance().SetMaxScrollLockY(WINCY);

    CLineManager::GetInstance().AddLine(Vector2(5884.f, 570.f), Vector2(6200.f, 240.f));

    m_pTriNextScene = C;
}

pair<bool, SCENETAG> CMission4::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};

    CObjectManager::GetInstance().Update();

    m_fDelta += 4000.f * DELTA;

    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CMission4::LateUpdate()
{
    CObjectManager::GetInstance().LateUpdate();
}

void CMission4::Render(HDC _hDC)
{
    RenderDessertLandscape(_hDC);

    CObjectManager::GetInstance().Render(_hDC);

    RenderFrontLandscape(_hDC);
}

void CMission4::Release()
{
    CSoundManager::GetInstance().StopSound(SOUND_BGM);
}

void CMission4::RenderDessertLandscape(HDC _hDC)
{
    HDC	hBackDC = nullptr;
    int idx = 0;
    for (int i = 0; i < 2; ++i)
    {
        hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Desert_Background");
        BitBlt(_hDC,
            1536 * i + SCROLLX / 4, 0
            , 1536
            , 384
            , hBackDC
            , 0
            , 0
            , SRCCOPY);
    }
    for (auto it = m_DessertBGKeyList.begin();
        it != m_DessertBGKeyList.end(); ++it)
    {
        hBackDC = CBmpManager::GetInstance().FindBmpImg(*it);
        GdiTransparentBlt(_hDC
            , 768 * idx + (int)(SCROLLX / 1.5f), WINCY - 576
            , 768, 384
            , hBackDC
            , 0, 0
            , 768, 384
            , RGB(255, 255, 255));
        idx++;
    }
    hBackDC = CBmpManager::GetInstance().FindBmpImg(m_vecDessertRuinBGKey[m_iRuinBGIdx]);
    GdiTransparentBlt(_hDC
        , 768 * idx + (int)(SCROLLX / 1.5f), 0
        , 1416, 576
        , hBackDC
        , 0, 0
        , 1416, 576
        , RGB(255, 255, 255));
    
    for (int i = 0; i < 4; ++i)
    {
        if (i == 3)
        {
            hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Desert_FlagPoint_Field");
            GdiTransparentBlt(_hDC
                , 1536 * i + SCROLLX, 0
                , 1920, 720
                , hBackDC
                , 0, 0
                , 1920, 720
                , RGB(255, 255, 255));
            continue;
        }
        hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Desert_Field");
        GdiTransparentBlt(_hDC
            , 1536 * i + SCROLLX, 0
            , 1536, 720
            , hBackDC
            , 0, 0
            , 1536, 720
            , RGB(255, 255, 255));
    }   
    
    if (m_fDelta >= 300.f)
    {
        m_DessertBGKeyList.push_front(m_DessertBGKeyList.back());
        m_DessertBGKeyList.pop_back();
        m_fDelta = 0.f;

        ++m_iRuinBGIdx;
        m_iRuinBGIdx = m_iRuinBGIdx % 4;
    }
}

void CMission4::RenderFrontLandscape(HDC _hDC)
{
    int iX = static_cast<int>(CScrollManager::GetInstance().GetScrollX());
    int iY = static_cast<int>(CScrollManager::GetInstance().GetScrollY());

    HDC	hFrontDC = nullptr;
    for (int i = 0; i < m_vecDessertFrontgroundKey.size(); ++i)
    {
        hFrontDC = CBmpManager::GetInstance().FindBmpImg(m_vecDessertFrontgroundKey[i]);

        int iScroll = static_cast<int>(SCROLLX / 1.5f);
        GdiTransparentBlt(_hDC
            , 768 * i + iScroll, WINCY - 192
            , 768, 192
            , hFrontDC
            , 0, 0
            , 768, 192
            , RGB(255, 255, 255));
    }
}

void CMission4::CreatePlatform()
{
    for (int i = 0; i < 9; ++i)
    {
        CObjectManager::GetInstance()
            .AddGameObject(CGameObjectFactory<CPlatform>
                ::Create(Vector2(384.f + 768.f * i, (float)WINCY - 128.f)
                    , Vector2(768.f, 64.f)), PLATFORM);
    }

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(6584.f, (float)WINCY - 512.f + 64.f)
                , Vector2(768.f, 64.f)), PLATFORM);
}

void CMission4::LoadBmpDessertLandscape()
{

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Background.bmp"
        , L"4-1_Desert_Background");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Background1.bmp"
        , L"4-1_Desert_Background1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Background2.bmp"
        , L"4-1_Desert_Background2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Background3.bmp"
        , L"4-1_Desert_Background3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Background4.bmp"
        , L"4-1_Desert_Background4");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Ruin1.bmp"
        , L"4-1_Desert_Ruin1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Ruin2.bmp"
        , L"4-1_Desert_Ruin2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Ruin3.bmp"
        , L"4-1_Desert_Ruin3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-1_Desert_Ruin4.bmp"
        , L"4-1_Desert_Ruin4");

    //CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-1_Dessert_Field1.bmp"
    //    , L"4-1_Dessert_Field1");
    //CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-1_Dessert_Field2.bmp"
    //    , L"4-1_Dessert_Field2");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Desert_Frontground1.bmp"
        , L"4-1_Desert_Frontground1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Desert_Frontground2.bmp"
        , L"4-1_Desert_Frontground2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Desert_Frontground3.bmp"
        , L"4-1_Desert_Frontground3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Desert_Frontground4.bmp"
        , L"4-1_Desert_Frontground4");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/FrontGround/4-1_Desert_Frontground5.bmp"
        , L"4-1_Desert_Frontground5");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-1_Desert_Field.bmp"
        , L"4-1_Desert_Field");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-1_Desert_FlagPoint_Field.bmp"
        , L"4-1_Desert_FlagPoint_Field");

    m_vecDessertRuinBGKey.push_back(L"4-1_Desert_Ruin1");
    m_vecDessertRuinBGKey.push_back(L"4-1_Desert_Ruin2");
    m_vecDessertRuinBGKey.push_back(L"4-1_Desert_Ruin3");
    m_vecDessertRuinBGKey.push_back(L"4-1_Desert_Ruin4");

    m_DessertBGKeyList.push_back(L"4-1_Desert_Background1");
    m_DessertBGKeyList.push_back(L"4-1_Desert_Background2");
    m_DessertBGKeyList.push_back(L"4-1_Desert_Background3");
    m_DessertBGKeyList.push_back(L"4-1_Desert_Background4");

    m_vecDessertFrontgroundKey.push_back(L"4-1_Desert_Frontground1");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Desert_Frontground2");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Desert_Frontground3");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Desert_Frontground4");
    m_vecDessertFrontgroundKey.push_back(L"4-1_Desert_Frontground5");
}

void CMission4::LoadBmpEnemy()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Idle.bmp"
        , L"ManEater_Idle");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Dead.bmp"
        , L"ManEater_Dead");
}
