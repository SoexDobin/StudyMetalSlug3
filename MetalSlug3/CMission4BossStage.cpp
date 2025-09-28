#include "pch.h"
#include "CMission4BossStage.h"
#include "CPlatform.h"
#include "CSolDaeLokker.h"

#include "CTimeManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CBmpManager.h"
#include "CGameObjectFactory.h"
#include "CLineManager.h"

CMission4BossStage::CMission4BossStage()
    : m_pSolDaeLokker(nullptr), m_pSolDaeLokkerDummy(nullptr)
    , m_iBGIdx(0), m_fBGDelta(0.f), m_fIntroDelta(0.f)
{
}

CMission4BossStage::~CMission4BossStage()
{
    Release();
}

void CMission4BossStage::Initialize()
{
    CScrollManager::GetInstance().ForceScrollY(0.f);
    CScrollManager::GetInstance().SetMinScrollLockX(0.f);
    CScrollManager::GetInstance().SetMinScrollLockY(0.f);
    CScrollManager::GetInstance().SetMaxScrollLockX(WINCX);

    CObjectManager::GetInstance().GetGameObjectList(PLAYER).front()->SetPivot(Vector2(200.f, -100.f));
    CObjectManager::GetInstance().GetGameObjectList(PLAYER).front()->SetDirection(Vector2::Zero);
    CLineManager::GetInstance().AddLine(Vector2::Zero, Vector2(WINCX, 0));

    LoadBmpBossStageLandscape();
    LoadBmpEnemy();
    Sequence();

    CreatetTriggerArea();
    CreatePlatform();

    m_pSolDaeLokker = CGameObjectFactory<CSolDaeLokker>::Create();
    CObjectManager::GetInstance().AddGameObject(m_pSolDaeLokker, ENEMY);

    CSoundManager::GetInstance().PlayBGM(L"BGM_OST_Steel_Beast_6_Beets.mp3", 0.3f);
}

pair<bool, SCENETAG> CMission4BossStage::Update()
{
    CObjectManager::GetInstance().Update();

    m_fBGDelta += 4000.f * DELTA;
    m_fIntroDelta += -40.f * DELTA;

    return pair<bool, SCENETAG>();
}

void CMission4BossStage::LateUpdate()
{

    CLineManager::GetInstance().Release();
    CObjectManager::GetInstance().LateUpdate();
}

void CMission4BossStage::Render(HDC _hDC)
{
    RenderBossLandscape(_hDC);

    CObjectManager::GetInstance().Render(_hDC);

    RenderFrontLandscape(_hDC);
}

void CMission4BossStage::Release()
{
}

void CMission4BossStage::Sequence()
{
}

void CMission4BossStage::RenderBossLandscape(HDC _hDC)
{
    HDC	hBackDC = nullptr;
    HDC	hFieldDC = nullptr;

    hBackDC = CBmpManager::GetInstance().FindBmpImg(m_vecBGKey[m_iBGIdx]);
    BitBlt(_hDC
        , 0, 0
        , 960, 792
        , hBackDC
        , 0, 0
        , SRCCOPY);
    hFieldDC = CBmpManager::GetInstance().FindBmpImg(L"4_Boss_Field");
    GdiTransparentBlt(_hDC
        , 0, 384 + 72
        , 960, 384
        , hFieldDC
        , 0, 0
        , 960, 384
        , RGB(255, 255, 255));


    if (m_fBGDelta >= 600.f)
    {
        m_fBGDelta = 0.f;

        ++m_iBGIdx;
        m_iBGIdx = m_iBGIdx % 8;
    }
}

void CMission4BossStage::RenderFrontLandscape(HDC _hDC)
{
    HDC hFrontDC = CBmpManager::GetInstance().FindBmpImg(L"4_Boss_Frontground");
    GdiTransparentBlt(_hDC
        , 0, 384 + 72
        , 960, 384
        , hFrontDC
        , 0, 0
        , 960, 384
        , RGB(255, 255, 255));

    if (m_fIntroDelta <= -400.f) return;

    hFrontDC = CBmpManager::GetInstance().FindBmpImg(L"4_Boss_FrontCloud");
    GdiTransparentBlt(_hDC
        , 0, static_cast<int>(m_fIntroDelta)
        , 960, 384
        , hFrontDC
        , 0, 0
        , 960, 384
        , RGB(255, 255, 255));
}

void CMission4BossStage::CreatePlatform()
{
    CObjectManager::GetInstance().AddGameObject(
        CGameObjectFactory<CPlatform>
        ::Create(Vector2(480.f, 688.f), Vector2(760.f, 64.f)), PLATFORM);
}

void CMission4BossStage::CreatetTriggerArea()
{
}

void CMission4BossStage::LoadBmpBossStageLandscape()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background1.bmp"
        , L"4_Boss_Background1");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background2.bmp"
        , L"4_Boss_Background2");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background3.bmp"
        , L"4_Boss_Background3");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background4.bmp"
        , L"4_Boss_Background4");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background5.bmp"
        , L"4_Boss_Background5");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background6.bmp"
        , L"4_Boss_Background6");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background7.bmp"
        , L"4_Boss_Background7");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/BackGround/4_Boss_Background8.bmp"
        , L"4_Boss_Background8");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Field/4_Boss_Field.bmp"
        , L"4_Boss_Field");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4_Boss_Frontground.bmp"
        , L"4_Boss_Frontground");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Landscape/Frontground/4_Boss_FrontCloud.bmp"
        , L"4_Boss_FrontCloud");

    m_vecBGKey.push_back(L"4_Boss_Background1");
    m_vecBGKey.push_back(L"4_Boss_Background2");
    m_vecBGKey.push_back(L"4_Boss_Background3");
    m_vecBGKey.push_back(L"4_Boss_Background4");
    m_vecBGKey.push_back(L"4_Boss_Background5");
    m_vecBGKey.push_back(L"4_Boss_Background6");
    m_vecBGKey.push_back(L"4_Boss_Background7");
    m_vecBGKey.push_back(L"4_Boss_Background8");
}

void CMission4BossStage::LoadBmpEnemy()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/SolDaeLokker_None.bmp"
        , L"SolDaeLokker_None");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/SolDaeLokker.bmp"
        , L"SolDaeLokker");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/Dead/SolDaeLokker_Dead.bmp"
        , L"SolDaeLokker_Dead");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/SDW/SolDaeLokker_SDW_Attack.bmp"
        , L"SolDaeLokker_SDW_Attack");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/SDW/SolDaeLokker_SDW_Ready.bmp"
        , L"SolDaeLokker_SDW_Ready");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/SDW/SolDaeLokker_SDW_End.bmp"
        , L"SolDaeLokker_SDW_End");

    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/YelCorn/SolDaeLokker_YelCorn_Attack.bmp"
        , L"SolDaeLokker_YelCorn_Attack");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/YelCorn/SolDaeLokker_YelCorn_Ready.bmp"
        , L"SolDaeLokker_YelCorn_Ready");
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Enemy/SolDaeLokker/YelCorn/SolDaeLokker_YelCorn_End.bmp"
        , L"SolDaeLokker_YelCorn_End");

}