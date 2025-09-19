#include "pch.h"
#include "CArmoryScene.h"
#include "CAnimation.h"
#include "CPlatform.h"
#include "CManEater.h"

// Managers
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CBmpManager.h"
#include "CScrollManager.h"
#include "CLineManager.h"

CArmoryScene::CArmoryScene()
{
}

CArmoryScene::~CArmoryScene()
{
    Release();
}

void CArmoryScene::Initialize()
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

    CLineManager::GetInstance().Initialize();
    CLineManager::GetInstance().AddLine(Vector2(100, WINCY - 100), Vector2( 800, WINCY - 100));
    CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CManEater>::Create(), ENEMY);
}

pair<bool, SCENETAG> CArmoryScene::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
    
    CObjectManager::GetInstance().Update();
    CLineManager::GetInstance().Update();

    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CArmoryScene::LateUpdate()
{
    CheatKeyInput();
    CObjectManager::GetInstance().LateUpdate();
    CLineManager::GetInstance().LateUpdate();
}

void CArmoryScene::Render(HDC _hDC)
{
    int iX = (int)CScrollManager::GetInstance().GetScrollX();
    int iY = (int)CScrollManager::GetInstance().GetScrollY();

    HDC	hBackDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Background");
    BitBlt(_hDC,
        0 + iX / 4,
        0,
        1536,
        384,
        hBackDC,
        0,
        0,
        SRCCOPY);

    HDC	hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field1");
    GdiTransparentBlt(_hDC
        , 768 * 0 + iX, WINCY - 584 + iY
        , 768, 384
        , hFieldDC
        , 0, 0
        , 768, 384
        , RGB(255,255,255));

    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field2");
    GdiTransparentBlt(_hDC
        , 768 * 1 + iX, WINCY - 584 + iY
        , 768, 384
        , hFieldDC
        , 0, 0
        , 768, 384
        , RGB(255, 255, 255));

    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field3");
    GdiTransparentBlt(_hDC
        , 768 * 2 + iX, WINCY - 584 + iY
        , 768, 384
        , hFieldDC
        , 0, 0
        , 768, 384
        , RGB(255, 255, 255));
    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Field4");
    GdiTransparentBlt(_hDC
        , 768 * 3 + iX, WINCY - 584 + iY
        , 768, 384
        , hFieldDC
        , 0, 0
        , 768, 384
        , RGB(255, 255, 255));


    CObjectManager::GetInstance().Render(_hDC);
    CLineManager::GetInstance().Render(_hDC);
}

void CArmoryScene::Release()
{
}

void CArmoryScene::CheatKeyInput()
{

}
