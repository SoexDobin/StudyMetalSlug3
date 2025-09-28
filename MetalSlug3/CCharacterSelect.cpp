#include "pch.h"
#include "CCharacterSelect.h"
#include "CAnimation.h"

#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CSoundManager.h"

CCharacterSelect::CCharacterSelect()
    : m_iHasSelect(false), m_iSelect(0), m_fAnimDelta(0.f)
{
}

CCharacterSelect::~CCharacterSelect()
{
}

void CCharacterSelect::Initialize()
{
    m_vSpawnPoint = Vector2(200.f, 560.f);
    LoadSelectionBmp();
    CSoundManager::GetInstance().PlayBGM(L"BGM_OST_Barracks.mp3", 0.2f);
}

pair<bool, SCENETAG> CCharacterSelect::Update()
{
    if (m_bDestroyScene) return pair<bool, SCENETAG>{SCENE_DESTROY, MISSION_SCENE_1};

    if (CKeyManager::GetInstance().KeyPressing(VK_RETURN) && !m_iHasSelect)
    {
        m_iHasSelect = true;
    }

    if (m_iHasSelect && m_fAnimDelta == 0.f)
    {
        // 사운드 재생
        CSoundManager::GetInstance().PlaySoundOnce(L"Charcter_Select_Ery.mp3", ANOUNCE, 0.2f);
        m_fAnimDelta += 2000 * DELTA; 
    }
    else if (m_iHasSelect)
    {
        m_fAnimDelta += 2000 * DELTA;
    }

    return pair<bool, SCENETAG>{SCENE_NOEVENT, SCENE_END};
}

void CCharacterSelect::LateUpdate()
{
    if (!m_iHasSelect) return;

    if (m_iSelect > 40)
    {
        --m_iSelect;
        m_bDestroyScene = SCENE_DESTROY;
    }

    if (100.f <= m_fAnimDelta)
    {
        ++m_iSelect;
        m_fAnimDelta = 0.01f;
    }
}

void CCharacterSelect::Render(HDC _hDC)
{ 
    HDC hMemDC = CBmpManager::GetInstance().FindBmpImg(L"Selection");

    int idx = m_iSelect;
    if (idx > 4) idx = 4;

    GdiTransparentBlt(_hDC
        , 0, 0
        , WINCX, WINCY
        , hMemDC
        , idx * WINCX, 0
        , WINCX, WINCY
        , RGB(255, 255, 255));
}

void CCharacterSelect::Release()
{

}

void CCharacterSelect::Sequence()
{
}

void CCharacterSelect::LoadSelectionBmp()
{
    CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/UI/Selection/Selection.bmp", L"Selection");
}
