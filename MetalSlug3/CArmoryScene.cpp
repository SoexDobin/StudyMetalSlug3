#include "pch.h"
#include "CArmoryScene.h"

// Managers
#include "CObjectManager.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CArmoryScene::CArmoryScene()
{
}

CArmoryScene::~CArmoryScene()
{
    Release();
}

void CArmoryScene::Initialize()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Landscape/BackGround/4-1_Dessert_Background.bmp"
        , L"4-1_Dessert_Background");
}

pair<bool, SCENETAG> CArmoryScene::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};

    CObjectManager::GetInstance().Update();


    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CArmoryScene::LateUpdate()
{
    CheatKeyInput();
    CObjectManager::GetInstance().LateUpdate();
}

void CArmoryScene::Render(HDC _hDC)
{
    int iScrollX = (int)CScrollManager::GetInstance().GetScrollX();
    int iScrollY = (int)CScrollManager::GetInstance().GetScrollY();

    HDC	hMemDC = CBmpManager::GetInstance().FindBmpImg(L"4-1_Dessert_Background");

    BitBlt(_hDC,
        512 + iScrollX / 4,
        128 + iScrollY / 4,
        512,
        128,
        hMemDC,
        0,
        0,
        SRCCOPY);
    CObjectManager::GetInstance().Render(_hDC);
}

void CArmoryScene::Release()
{
}

void CArmoryScene::CheatKeyInput()
{

}
