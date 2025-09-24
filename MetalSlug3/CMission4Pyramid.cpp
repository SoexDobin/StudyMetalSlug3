#include "pch.h"
#include "CMission4Pyramid.h"
#include "CManEater.h"
#include "CPlatform.h"
#include "CSequenceTrigger.h"
#include "CChangeTrigger.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CLineManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CGameObjectFactory.h"
#include "CSoundManager.h"

CMission4Pyramid::CMission4Pyramid()
    : m_iScrollLockIdx(0)
    , m_iWaterFallIdx(0), m_fWaterFallDelta(0.f)
{
}

CMission4Pyramid::~CMission4Pyramid()
{
    Release();
}

void CMission4Pyramid::Initialize()
{
    CObjectManager::GetInstance().GetGameObjectList(PLAYER).front()->SetPivot(Vector2(100, 650 - WINCY));

    CScrollManager::GetInstance().SetScrollX(0);
    CScrollManager::GetInstance().SetScrollY(WINCY);
    CScrollManager::GetInstance().SetMinScrollLockY(-WINCY);

    LoadBmpPyramidLandscape();
    LoadBmpEnemy();
    Sequence();

    CreatetTriggerArea();
    CreatePlatform();
    
    CSoundManager::GetInstance().PlayBGM(L"BGM_OST_Pyramid.mp3", 0.2f);
    m_fWaterFallDelta = 0.f;
}

pair<bool, SCENETAG> CMission4Pyramid::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_DESTROY, MISSION_SCENE_3};

    CObjectManager::GetInstance().Update();

    m_fWaterFallDelta += 6000.f * DELTA;

    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CMission4Pyramid::LateUpdate()
{
    CObjectManager::GetInstance().LateUpdate();
}

void CMission4Pyramid::Render(HDC _hDC)
{
    RenderPyramidLandscape(_hDC);

    CObjectManager::GetInstance().Render(_hDC);

    RenderFrontLandscape(_hDC);
}

void CMission4Pyramid::Release()
{
}

void CMission4Pyramid::Sequence()
{
    // TODO : 최소 스크롤 막을 수 있어야함
    // MINX 적용시 같이 하면 될 듯
    std::wcout << m_iScrollLockIdx << L"\n";
    

    CScrollManager::GetInstance().SetMinScrollLockX(n_vPyramidMinLockPoints[m_iScrollLockIdx].x);
    CScrollManager::GetInstance().SetMinScrollLockY(n_vPyramidMinLockPoints[m_iScrollLockIdx].y);

    if (n_vPyramidMaxLockPoints[m_iScrollLockIdx].x < 0.f)
        CScrollManager::GetInstance().SetMaxScrollLockX(-n_vPyramidMaxLockPoints[m_iScrollLockIdx].x, true);
    else
        CScrollManager::GetInstance().SetMaxScrollLockX(n_vPyramidMaxLockPoints[m_iScrollLockIdx].x, false);
    CScrollManager::GetInstance().SetMaxScrollLockY(n_vPyramidMaxLockPoints[m_iScrollLockIdx].y);
    ++m_iScrollLockIdx;
}

void CMission4Pyramid::RenderPyramidLandscape(HDC _hDC)
{
    HDC	hBackDC = nullptr;
    HDC	hFieldDC = nullptr;

    hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Background");
    BitBlt(_hDC,
        0, -849 + WINCY
        , 1536
        , 849
        , hBackDC
        , 0
        , 0
        , SRCCOPY);
    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Field1");
    GdiTransparentBlt(_hDC
        , SCROLLX, -648 + SCROLLY
        , 1848, 648 
        , hFieldDC
        , 0, 0
        , 1848, 648
        , RGB(255, 255, 255));
    for (int i = 0; i < m_vecPyramidSecondFloorKey.size(); ++i)
    {
        hFieldDC = CBmpManager::GetInstance().FindBmpImg(m_vecPyramidSecondFloorKey[i]);
        GdiTransparentBlt(_hDC
            , (120) * 3 + (768 * i) + SCROLLX, -1416 + SCROLLY
            , 768, 768
            , hFieldDC
            , 0, 0
            , 768, 768
            , RGB(255, 255, 255));
    }
    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Field6");
    GdiTransparentBlt(_hDC
        , 1966 + SCROLLX, -WINCY -1626 + SCROLLY
        , 1824, 930
        , hFieldDC
        , 0, 0
        , 1824, 930
        , RGB(255, 255, 255));

    for (int i = 0; i < m_vecPyramidTopFloorKey.size(); ++i)
    {
        hFieldDC = CBmpManager::GetInstance().FindBmpImg(m_vecPyramidTopFloorKey[i]);
        GdiTransparentBlt(_hDC
            , 3924.f + 1536.f * i + SCROLLX, -2896.f + SCROLLY
            , 1536, 768
            , hFieldDC
            , 0, 0
            , 1536, 768
            , RGB(255, 255, 255));
    }

    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_BossEntrance");
    GdiTransparentBlt(_hDC
        , 9000.f + SCROLLX, -2500.f + SCROLLY
        , 384, 384
        , hFieldDC
        , 0, 0
        , 384, 384
        , RGB(255, 255, 255));

}

void CMission4Pyramid::RenderFrontLandscape(HDC _hDC)
{
    HDC hFrontDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Frontground1");
    GdiTransparentBlt(_hDC
        , 6996.f + 384.f + SCROLLX, -2958.f + SCROLLY
        , 768, 768
        , hFrontDC
        , 0, 0
        , 768, 768
        , RGB(255, 255, 255));
    hFrontDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Frontground2");
    GdiTransparentBlt(_hDC
        , 6996.f + 384.f + 768.f + SCROLLX, -2957.f + SCROLLY
        , 768, 768
        , hFrontDC
        , 0, 0
        , 768, 768
        , RGB(255, 255, 255));
    // 18 -3    

    hFrontDC = CBmpManager::GetInstance().FindBmpImg(m_vecWaterFallKey[m_iWaterFallIdx]);
    GdiTransparentBlt(_hDC
        , 6996.f + 384.f + 768.f + 18.f + SCROLLX, -2957.f - 3.f + SCROLLY
        , 384, 768
        , hFrontDC
        , 0, 0
        , 384, 768
        , RGB(255, 255, 255));

    hFrontDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_FrontBossEntrance");
    GdiTransparentBlt(_hDC
        , 9000.f + SCROLLX, -2500.f + SCROLLY
        , 384, 384
        , hFrontDC
        , 0, 0
        , 384, 384
        , RGB(255, 255, 255));

    if (m_fWaterFallDelta >= 200.f)
    {
        m_fWaterFallDelta = 0.f;

        ++m_iWaterFallIdx;
        m_iWaterFallIdx = m_iWaterFallIdx % 4;
    }

    
}

void CMission4Pyramid::CreatePlatform()
{
    CLineManager::GetInstance().AddLine(Vector2(-10, WINCY - WINCY), Vector2(256.f, 450.f - WINCY));

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(256.f + 576.f, 482.f - WINCY)
                , Vector2(768.f + 384.f, 64.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(506.f, 320.f - WINCY)
                , Vector2(280.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(988.f, 320.f - WINCY)
                , Vector2(280.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(1050.f, 200.f - WINCY)
                , Vector2(1060.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(796.f, 80.f - WINCY)
                , Vector2(280.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(1370.f, 80.f - WINCY)
                , Vector2(560.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(1940.f, -40.f - WINCY)
                , Vector2(2080.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(3430.f, -40.f - WINCY)
                , Vector2(280.f, 8.f)), PLATFORM);


    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(1082.f, -160.f - WINCY)
                , Vector2(280.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(1662.f, -160.f - WINCY)
                , Vector2(560.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2197.f, -160.f - WINCY)
                , Vector2(210.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2646.f, -160.f - WINCY)
                , Vector2(360.f, 8.f)), PLATFORM);


    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(1685.f, -280.f - WINCY)
                , Vector2(1040.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2766.f, -280.f - WINCY)
                , Vector2(700.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2958.f, -400.f - WINCY)
                , Vector2(560.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(3098.f, -520.f - WINCY)
                , Vector2(560.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(3430.f, -640.f - WINCY)
                , Vector2(560.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(3024.f, -760.f - WINCY)
                , Vector2(280.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2598.f, -880.f - WINCY)
                , Vector2(700.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2598.f, -880.f - WINCY)
                , Vector2(700.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2256.f, -1720.f)
                , Vector2(280.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2184.f, -1840.f)
                , Vector2(280.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2278.f, -1960.f)
                , Vector2(140.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2330.f, -2080.f)
                , Vector2(420.f, 8.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2636.f, -2200.f)
                , Vector2(280.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(2980.f, -2320.f)
                , Vector2(280.f, 8.f)), PLATFORM);    
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(3504.f, -2320.f)
                , Vector2(280.f, 8.f)), PLATFORM);

    for (auto& platform : CObjectManager::GetInstance().GetGameObjectList(PLATFORM))
    {
        dynamic_cast<CPlatform*>(platform)->SetProjectilePass(true);
    }

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(4225.f, -2320.f + 48.f)
                , Vector2(550.f, 96.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(4750.f, -2320.f - 48.f)
                , Vector2(500.f, 96.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(5250.f, -2320.f - 88.f)
                , Vector2(500.f, 96.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(5960.f, -2320.f - 88.f - 96.f)
                , Vector2(920.f, 96.f)), PLATFORM);
    for (int i = 0; i < 4; ++i)
    {
        CObjectManager::GetInstance()
            .AddGameObject(CGameObjectFactory<CPlatform>
                ::Create(Vector2(6640.f+ (i * 500.f), -2320.f - 88.f)
                    , Vector2(500.f, 96.f)), PLATFORM);
    }

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(8470.f, -2700.f)
                , Vector2(140.f, 32.f)), PLATFORM);
        
    CLineManager::GetInstance().AddLine(Vector2(8080.f, -2456.f), Vector2(8400.f, -2700.f));


    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(9300.f, -2350.f)
                , Vector2(50.f, 32.f)), PLATFORM);
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(9100.f, -2350.f)
                , Vector2(50.f, 32.f)), PLATFORM);
}

void CMission4Pyramid::CreatetTriggerArea()
{
    CGameObject* pTri = nullptr;

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 300.f, 700.f });
    pTri->SetPivot({ 820.f, 300.f - WINCY });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);


    pTri = new CSequenceTrigger();
    pTri->SetSize({ 100.f, 100.f });
    pTri->SetPivot({ 1000.f, -720.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 100.f, 100.f });
    pTri->SetPivot({ 3430.f, -1420.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 100.f, 100.f });
    pTri->SetPivot({ 2700.f, -1700.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 140.f, 140.f });
    pTri->SetPivot({ 2276.f, -2000.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 140.f, 140.f });
    pTri->SetPivot({ 2560.f, -2260.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 140.f, 300.f });
    pTri->SetPivot({ 3504.f, -2500.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 560.f, 140.f });
    pTri->SetPivot({ 6000.f, -2600.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    CGameObject* pEntranceBoss = new CChangeTrigger();
    pTri->SetSize({ 300.f, 300.f });
    pTri->SetPivot({ 9000.f, -2500.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);
}

void CMission4Pyramid::LoadBmpPyramidLandscape()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4-2_Pyramid_Background.bmp"
        , L"4-2_Pyramid_Background");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field1.bmp"
        , L"4-2_Pyramid_Field1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field2.bmp"
        , L"4-2_Pyramid_Field2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field3.bmp"
        , L"4-2_Pyramid_Field3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field4.bmp"
        , L"4-2_Pyramid_Field4");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field5.bmp"
        , L"4-2_Pyramid_Field5");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field6.bmp"
        , L"4-2_Pyramid_Field6");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field7.bmp"
        , L"4-2_Pyramid_Field7");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field8.bmp"
        , L"4-2_Pyramid_Field8");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_Field9.bmp"
        , L"4-2_Pyramid_Field9");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_Frontground1.bmp"
        , L"4-2_Pyramid_Frontground1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_Frontground2.bmp"
        , L"4-2_Pyramid_Frontground2");
   
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_FrontWaterFall1.bmp"
        , L"4-2_Pyramid_FrontWaterFall");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_FrontWaterFall2.bmp"
        , L"4-2_Pyramid_FrontWaterFall2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_FrontWaterFall3.bmp"
        , L"4-2_Pyramid_FrontWaterFall3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_FrontWaterFall4.bmp"
        , L"4-2_Pyramid_FrontWaterFall4");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4-2_Pyramid_BossEntrance.bmp"
        , L"4-2_Pyramid_FrontBossEntrance");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4-2_Pyramid_FrontBossEntrance.bmp"
        , L"4-2_Pyramid_FrontBossEntrance");

    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field2");
    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field3");
    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field4");
    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field5");

    m_vecPyramidTopFloorKey.push_back(L"4-2_Pyramid_Field7");
    m_vecPyramidTopFloorKey.push_back(L"4-2_Pyramid_Field8");
    m_vecPyramidTopFloorKey.push_back(L"4-2_Pyramid_Field9");

    m_vecWaterFallKey.push_back(L"4-2_Pyramid_FrontWaterFall1");
    m_vecWaterFallKey.push_back(L"4-2_Pyramid_FrontWaterFall2");
    m_vecWaterFallKey.push_back(L"4-2_Pyramid_FrontWaterFall3");
    m_vecWaterFallKey.push_back(L"4-2_Pyramid_FrontWaterFall4");
}

void CMission4Pyramid::LoadBmpEnemy()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Idle.bmp"
        , L"ManEater_Idle");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Dead.bmp"
        , L"ManEater_Dead");
}
