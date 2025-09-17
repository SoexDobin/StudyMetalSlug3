#include "pch.h"
#include "CArmoryScene.h"

// Managers
#include "CObjectManager.h"

CArmoryScene::CArmoryScene()
{
}

CArmoryScene::~CArmoryScene()
{
    Release();
}

void CArmoryScene::Initialize()
{
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
    CObjectManager::GetInstance().Render(_hDC);
}

void CArmoryScene::Release()
{
}

void CArmoryScene::CheatKeyInput()
{

}
