#include "pch.h"
#include "CArmoryScene.h"
#include "CAnimation.h"
#include "CPlatform.h"
#include "CPrisonerTied.h"
#include "CChangeTrigger.h"

// Managers
#include "CGameObjectFactory.h"
#include "CObjectManager.h"
#include "CBmpManager.h"
#include "CScrollManager.h"
#include "CLineManager.h"
#include "CSoundManager.h"

CArmoryScene::CArmoryScene()
{
}

CArmoryScene::~CArmoryScene()
{
    Release();
}

void CArmoryScene::Initialize()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Idle.bmp"
        , L"ManEater_Idle");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Dead.bmp"
        , L"ManEater_Dead");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Move.bmp"
        , L"ManEater_Move");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Jump&Drop.bmp"
        , L"ManEater_Jump&Drop");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Attack_Left.bmp"
        , L"ManEater_Attack_Left");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Attack_Right.bmp"
        , L"ManEater_Attack_Right");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Attack_Blank.bmp"
        , L"ManEater_Attack_Blank");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/ManEater/ManEater_Blank.bmp"
        , L"ManEater_Blank");

    m_vSpawnPoint = Vector2(200.f, 470.f);
    CScrollManager::GetInstance().SetMinScrollLockX(0.f);
    CScrollManager::GetInstance().SetMinScrollLockY(0.f);
    CScrollManager::GetInstance().SetMaxScrollLockX(WINCX);
    CScrollManager::GetInstance().SetMaxScrollLockY(WINCY);
    CSoundManager::GetInstance().PlayBGM(L"BGM_OST_Desert.mp3", 0.2f);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(140.f, 600.f)
                , Vector2(560.f, 8.f)), PLATFORM); 


    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2((float)WINCX, 600.f)
                , Vector2((float)WINCX, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(140.f, 360.f)
                , Vector2(280.f, 8.f)), PLATFORM);

    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPlatform>
            ::Create(Vector2(580.f, 360.f)
                , Vector2(280.f, 8.f)), PLATFORM);


    CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CPrisonerTied>::Create(), ENEMY);
    CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(580.f, 450.f));
    dynamic_cast<CPrisonerTied*>(CObjectManager::GetInstance().GetGameObjectList(ENEMY).back())->SetHp(90);
    dynamic_cast<CPrisonerTied*>(CObjectManager::GetInstance().GetGameObjectList(ENEMY).back())->LockUnravel();

    CGameObject* pTri = nullptr;

    pTri = new CChangeTrigger();
    pTri->SetSize({ 100.f, 100.f });
    pTri->SetPivot({ 960.f, 200.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);
}

pair<bool, SCENETAG> CArmoryScene::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_DESTROY, MISSION_SCENE_1};
    
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
    SafeDelete<CGameObject*>(CObjectManager::GetInstance().GetGameObjectList(PLAYER).front());
    CObjectManager::GetInstance().GetGameObjectList(PLAYER).pop_front();

    CScrollManager::GetInstance().ForceScrollX(0);
    CScrollManager::GetInstance().ForceScrollY(0);
}

void CArmoryScene::CheatKeyInput()
{

}

void CArmoryScene::Sequence()
{
}
