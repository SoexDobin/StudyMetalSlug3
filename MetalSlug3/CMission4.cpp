#include "pch.h"
#include <random>
#include "CMission4.h"
#include "CManEater.h"
#include "CPlatform.h"
#include "CChangeTrigger.h"
#include "CSequenceTrigger.h"
#include "CMissionStart4.h"
#include "CRebel.h"
#include "CRebelBazooca.h"
#include "CPrisonerTied.h"
#include "CGrenade.h"
#include "CIronLizzard.h"
#include "CFlameShot.h"
#include "CDiCokka.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CGameObjectFactory.h"
#include "CSoundManager.h"
#include "CLineManager.h"
#include "CParticleManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"

CMission4::CMission4() 
    : m_iScrollLockIdx(0), m_fDelta(0.f), m_iRuinBGIdx(0), m_fBackAttackDelta(-1.f), m_bIntroSource(false)
{
}

CMission4::~CMission4()
{
    Release();
}

void CMission4::Initialize()
{
    m_vSpawnPoint = Vector2(200.f, 560.f);
    LoadBmpDessertLandscape();
    LoadBmpEnemy();
    
    CreatetTriggerArea();
    CreatePlatform();

    m_vecItems.push_back(new CIronLizzard());
    m_vecItems.push_back(new CGrenade());

    CScrollManager::GetInstance().SetMinScrollLockX(0.f);
    CScrollManager::GetInstance().SetMinScrollLockY(0.f);

    CScrollManager::GetInstance().SetMaxScrollLockX(n_vDesertMaxLockPoints[m_iScrollLockIdx].x);
    CScrollManager::GetInstance().SetMaxScrollLockY(n_vDesertMaxLockPoints[m_iScrollLockIdx].y); 



    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPrisonerTied>::Create(), ENEMY);
    CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(500.f, 500.f));
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CPrisonerTied>::Create(), ENEMY);
    CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(2700.f, 500.f));


    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CDiCokka>::Create(), ENEMY);
    CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(5800.f, 460.f));
    CObjectManager::GetInstance()
        .AddGameObject(CGameObjectFactory<CDiCokka>::Create(), ENEMY);
    CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(4800.f, 460.f));

}

pair<bool, SCENETAG> CMission4::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_DESTROY, MISSION_SCENE_2};

    if (CKeyManager::GetInstance().KeyPressing(VK_F5) && !m_bIntroSource)
    {
        CSoundManager::GetInstance().PlayBGM(L"BGM_OST_Desert.mp3", 0.2f);
        CParticleManager::GetInstance().CreateParticle<CMissionStart4>();
        m_bIntroSource = true;
    }

    CObjectManager::GetInstance().Update();

    m_vSpawnPoint = { -SCROLLX + 200, 400};
    m_fDelta += 4000.f * DELTA;

    if (m_fBackAttackDelta >= 0.f)
        m_fBackAttackDelta += 200 * DELTA;

    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CMission4::LateUpdate()
{
    if (m_fBackAttackDelta >= 2000.f && m_iScrollLockIdx < 3)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> Iinterval(32, 64);
        Vector2 pScenePivot = CSceneManager::GetInstance().GetCurPlayer()->GetPivot();

        for (int i = 0; i < 17; ++i)
        {

            float fPosX = pScenePivot.x - (520.f + Iinterval(gen) * i);
            CObjectManager::GetInstance()
                .AddGameObject(CGameObjectFactory<CRebel>::Create(), ENEMY);
            CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(fPosX, 400.f));
        }
        m_fBackAttackDelta = 0.f;
    }


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
    for (auto& pItem : m_vecItems)
        SafeDelete<CGameObject*>(pItem);
}

void CMission4::Sequence()
{
    ++m_iScrollLockIdx;
    CScrollManager::GetInstance().SetMaxScrollLockX(n_vDesertMaxLockPoints[m_iScrollLockIdx].x);
    CScrollManager::GetInstance().SetMaxScrollLockY(n_vDesertMaxLockPoints[m_iScrollLockIdx].y);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> Iinterval(32, 64);

    float fPosX = 0.f;
    Vector2 pScenePivot = CSceneManager::GetInstance().GetCurPlayer()->GetPivot();

    if (m_iScrollLockIdx == 1)
    {
        for (int i = 0; i < 20; ++i)
        {
            fPosX = pScenePivot.x + (float)WINCX + Iinterval(gen) * i;
            CObjectManager::GetInstance()
                .AddGameObject(CGameObjectFactory<CRebel>::Create(), ENEMY);
            CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(fPosX, 400.f));
        }
        m_fBackAttackDelta = 0.f;
    }
    else if (m_iScrollLockIdx < 5)
    {
        for (int i = 0; i < 20; ++i)
        {
            fPosX = pScenePivot.x + (float)WINCX + Iinterval(gen) * i;
            CObjectManager::GetInstance()
                .AddGameObject(CGameObjectFactory<CRebel>::Create(), ENEMY);
            CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(fPosX, 400.f));
        }   
        for (int i = 0; i < 5; ++i)
        {
            fPosX = pScenePivot.x + (float)WINCX + Iinterval(gen) * i;
            CObjectManager::GetInstance()
                .AddGameObject(CGameObjectFactory<CRebelBazooca>::Create(), ENEMY);
            CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(Vector2(fPosX, 400.f));
        }
    }
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

void CMission4::CreatetTriggerArea()
{
    CGameObject* pTri = nullptr;

    pTri = new CChangeTrigger();
    pTri->SetSize({ 100.f, 100.f });
    pTri->SetPivot({ 6600.f, 200.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 800.f, 1000.f });
    pTri->SetPivot({ 960.f, 500.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 800.f, 1000.f });
    pTri->SetPivot({ 2000.f, 500.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 800.f, 1000.f });
    pTri->SetPivot({ 3200.f, 500.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);

    pTri = new CSequenceTrigger();
    pTri->SetSize({ 800.f, 1000.f });
    pTri->SetPivot({ 5000.f, 500.f });
    pTri->Initialize();
    CObjectManager::GetInstance()
        .AddGameObject(pTri, NEUTRAL);
}

void CMission4::CreatePlatform()
{
    for (int i = 0; i < 9; ++i)
    {
        CGameObject* pPlatform = CGameObjectFactory<CPlatform>
            ::Create(Vector2(384.f + 768.f * i, (float)WINCY - 128.f)
                , Vector2(768.f, 64.f));

        m_vecPlatform.push_back(pPlatform);
        CObjectManager::GetInstance()
            .AddGameObject(pPlatform, PLATFORM);
    }
    CGameObject* pPlatform = CGameObjectFactory<CPlatform>
        ::Create(Vector2(6584.f, (float)WINCY - 512.f + 64.f)
            , Vector2(768.f, 64.f));

    CObjectManager::GetInstance()
        .AddGameObject(pPlatform, PLATFORM);

    CLineManager::GetInstance().AddLine(Vector2(5884.f, 570.f), Vector2(6200.f, 240.f));
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
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/Rebel/Rebel_Idle.bmp"
        , L"Rebel_Idle");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/Rebel/Rebel_Dead.bmp"
        , L"Rebel_Dead");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/Rebel/Rebel_Move.bmp"
        , L"Rebel_Move");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/Rebel/Rebel_Bazooca_Shoot.bmp"
        , L"Rebel_Bazooca_Shoot");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/Rebel/Rebel_Bazooca_Idle.bmp"
        , L"Rebel_Bazooca_Idle");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Bazooca/Bazooca.bmp"
        , L"Bazooca");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Projectile/Cokka/Di_Cokka_Shoot_Projectile.bmp"
        , L"Di_Cokka_Shoot_Projectile");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerTied.bmp"
        , L"PrisonerTied");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerUnravel.bmp"
        , L"PrisonerUnravel");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerMove.bmp"
        , L"PrisonerMove");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Prisoner/PrisonerSupply.bmp"
        , L"PrisonerSupply");
}


