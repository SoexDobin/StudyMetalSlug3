#include "pch.h"
#include "CMission4Pyramid.h"
#include "CManEater.h"
#include "CPlatform.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CGameObjectFactory.h"

CMission4Pyramid::CMission4Pyramid()
{
}

CMission4Pyramid::~CMission4Pyramid()
{
    Release();
}

void CMission4Pyramid::Initialize()
{
    LoadBmpPyramidLandscape();
    LoadBmpEnemy();

    CreatePlatform();
    for (int i = 1; i < 5; ++i)
    {
        CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CManEater>::Create(), ENEMY);
        CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(i * 600.f, 100.f));
    }
}

pair<bool, SCENETAG> CMission4Pyramid::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};

    CObjectManager::GetInstance().Update();

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
}

void CMission4Pyramid::Release()
{
}

void CMission4Pyramid::RenderPyramidLandscape(HDC _hDC)
{
    HDC	hBackDC = nullptr;
    HDC	hFieldDC = nullptr;

    hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Background");
    BitBlt(_hDC,
        0, WINCY - 849
        , 1536
        , 849
        , hBackDC
        , 0
        , 0
        , SRCCOPY);
    hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-2_Pyramid_Field1");
    GdiTransparentBlt(_hDC
        , SCROLLX, WINCY - 648 -1 + SCROLLY
        , 1848, 648 
        , hBackDC
        , 0, 0
        , 1848, 648
        , RGB(255, 255, 255));

    for (int i = 0; i < m_vecPyramidSecondFloorKey.size(); ++i)
    {
        hBackDC = CBmpManager::GetInstance().FindBmpImg(m_vecPyramidSecondFloorKey[i]);
        GdiTransparentBlt(_hDC
            , 477 + (768 * i) + SCROLLX, WINCY - 648 - 768 + SCROLLY
            , 768, 768
            , hBackDC
            , 0, 0
            , 768, 768
            , RGB(255, 255, 255));
    }
}

void CMission4Pyramid::CreatePlatform()
{
    for (int i = 0; i < 9; ++i)
    {
        CObjectManager::GetInstance()
            .AddGameObject(CGameObjectFactory<CPlatform>
                ::Create(Vector2(384.f + 768.f * i, (float)WINCY - 64.f)
                    , Vector2(768.f, 64.f)), PLATFORM);
    }

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(384.f, (float)WINCY - 144.f - 192.f)
                , Vector2(768.f, 64.f)), PLATFORM);
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

    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field2");
    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field3");
    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field4");
    m_vecPyramidSecondFloorKey.push_back(L"4-2_Pyramid_Field5");
}

void CMission4Pyramid::LoadBmpEnemy()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Idle.bmp"
        , L"ManEater_Idle");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Dead.bmp"
        , L"ManEater_Dead");
}
