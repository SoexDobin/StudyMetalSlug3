#include "pch.h"
#include "CSceneManager.h"
#include "CEri.h"
#include "CEriSpawn.h"
#include "CScene.h"
#include "CArmoryScene.h"
#include "CMission4.h"
#include "CMission4Pyramid.h"
#include "CMission4BossStage.h"
#include "CMouse.h"
#include "CTimerUI.h"
#include "CScoreUI.h"
#include "CManEater.h"
#include "CMultiHitUI.h"
#include "CArmUI.h"
#include "CCharacterSelect.h"
#include "CMission4Complete.h"

// Managers
#include "CObjectManager.h"
#include "CCollisionManager.h"
#include "CGameObjectFactory.h"
#include "CScrollManager.h"
#include "CSoundManager.h"
#include "CLineManager.h"
#include "CUIManager.h"
#include "CTimeManager.h"
#include "CParticleManager.h"

CSceneManager::CSceneManager() 
	: m_pScene(nullptr), m_pPrevScene(nullptr), m_eCurScene(SCENE_END)
	, m_pPlayer(nullptr)
	, m_pTimerUI(nullptr), m_pScoreUI(nullptr), m_pMultiHitUI(nullptr), m_pArmBoxUI(nullptr)
	, m_bMissionComplete(false)
	, m_fCompleteDelta(0.f)
{

}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	// TODO : 게임 흐름 구축 시 변경 해야 합니다.
	m_eCurScene = BARRACKS_SCENE;

	m_pPlayer = new CEri();
	m_pPlayer->Initialize();
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).push_back(m_pPlayer);

	m_pTimerUI = new CTimerUI();
	m_pTimerUI->Initialize();
	CUIManager::GetInstance().AddUIObject(m_pTimerUI);

	m_pScoreUI = new CScoreUI();
	m_pScoreUI->Initialize();
	CUIManager::GetInstance().AddUIObject(m_pScoreUI);

	m_pMultiHitUI = new CMultiHitUI();
	m_pMultiHitUI->Initialize();
	CUIManager::GetInstance().AddUIObject(m_pMultiHitUI);
	
	m_pArmBoxUI = new CArmUI();
	m_pArmBoxUI->Initialize();
	CUIManager::GetInstance().AddUIObject(m_pArmBoxUI);

	ChangeScene(m_eCurScene);
#ifdef _DEBUG
	//CObjectManager::GetInstance().AddGameObject(
	//	CGameObjectFactory<CMouse>::Create(), PLAYER);
	//CObjectManager::GetInstance().GetGameObjectList(PLAYER).back()->Initialize();
#endif
}

void CSceneManager::Update()
{	
	pair<bool, SCENETAG> bSceneState = m_pScene->Update();

	if (bSceneState.first && m_pPrevScene == nullptr)
	{
		m_pPrevScene = ChangeScene(bSceneState.second);
		SafeDelete<CScene*>(m_pPrevScene);
	}

	if (dynamic_cast<CTimerUI*>(m_pTimerUI)->GetTime() <= 0.f)
	{
		dynamic_cast<CEri*>(m_pPlayer)->Dead();
		dynamic_cast<CTimerUI*>(m_pTimerUI)->SetTime(61.f);
	}

	if (m_bMissionComplete && m_fCompleteDelta > 0.f)
	{
		m_fCompleteDelta += DELTA;

		if (m_fCompleteDelta >= 6.f)
		{
			m_fCompleteDelta = -1.f;
			CParticleManager::GetInstance().CreateParticle<CMission4Complete>();
		}
		return;
	}


	if (m_bMissionComplete && m_fCompleteDelta == 0.f)
	{
		dynamic_cast<CEri*>(m_pPlayer)->CheckMissionComplete(true);
		dynamic_cast<CTimerUI*>(m_pTimerUI)->StopTime();
		
		
		m_fCompleteDelta += DELTA;
	}
}

void CSceneManager::LateUpdate()
{
	
	m_pScene->LateUpdate();

	CScrollManager::GetInstance().ScrollLock();
		
}

void CSceneManager::Render(HDC _hDC)
{
	m_pScene->Render(_hDC);

	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, CObjectManager::GetInstance().GetGameObjectList(ENEMY)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, CObjectManager::GetInstance().GetGameObjectList(ITEM)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, CObjectManager::GetInstance().GetGameObjectList(PROJECTILE)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, CObjectManager::GetInstance().GetGameObjectList(EXPLODE)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, CObjectManager::GetInstance().GetGameObjectList(PLATFORM)
		, RECT_TO_RECT);

	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(NEUTRAL)
		, CObjectManager::GetInstance().GetGameObjectList(ENEMY)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(NEUTRAL)
		, CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, RECT_TO_RECT);

	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PROJECTILE)
		, CObjectManager::GetInstance().GetGameObjectList(ENEMY)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(ENEMY)
		, CObjectManager::GetInstance().GetGameObjectList(PLATFORM)
		, RECT_TO_RECT);

	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(NEUTRAL)
		, CObjectManager::GetInstance().GetGameObjectList(PLATFORM)
		, RECT_TO_RECT);

	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(NEUTRAL)
		, CObjectManager::GetInstance().GetGameObjectList(PLATFORM)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PROJECTILE)
		, CObjectManager::GetInstance().GetGameObjectList(PLATFORM)
		, RECT_TO_RECT);
	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(ENEMY)
		, CObjectManager::GetInstance().GetGameObjectList(EXPLODE)
		, RECT_TO_RECT);

#ifdef _DEBUG
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(PLAYER));
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(ITEM));
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(ENEMY));
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(PROJECTILE));
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(PLATFORM));
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(NEUTRAL));
	//CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(EXPLODE));
#endif 	
}

void CSceneManager::Release()
{
	SafeDelete<CScene*>(m_pScene);
}

CScene* CSceneManager::ChangeScene(SCENETAG _eTag)
{
	CSoundManager::GetInstance().StopSound(SOUND_BGM);
	CScene* _pPrev = m_pScene;
	m_eCurScene		= _eTag;

	switch (_eTag)
	{
	case MAIN_SCENE:		break;
	case BARRACKS_SCENE:	
		m_pScene = new CCharacterSelect();
		DisableSceneUI();
		break;
	case MISSION_SCENE_3:	m_pScene = new CMission4BossStage();	break;
	case MISSION_SCENE_2:	m_pScene = new CMission4Pyramid();		break;
	case MISSION_SCENE_1:	
		m_pScene = new CMission4();		
		EnableSceneUI();
		break;
	case EDIT:				break;

	case SCENE_END:
		break;
	default:
		break;
	}

	CLineManager::GetInstance().Release();
	for (auto& pObj : CObjectManager::GetInstance().GetGameObjectList(PLATFORM))
	{
		pObj->SetDestroy();
	}
	for (auto& pObj : CObjectManager::GetInstance().GetGameObjectList(ENEMY))
	{
		pObj->SetDestroy();
	}
	for (auto& pObj : CObjectManager::GetInstance().GetGameObjectList(NEUTRAL))
	{
		pObj->SetDestroy();
	}
	for (auto& pObj : CObjectManager::GetInstance().GetGameObjectList(PROJECTILE))
	{
		pObj->SetDestroy();
	}
	for (auto& pObj : CObjectManager::GetInstance().GetGameObjectList(PARTICLE))
	{
		pObj->SetDestroy();
	} 
	for (auto& pObj : CObjectManager::GetInstance().GetGameObjectList(EXPLODE))
	{
		pObj->SetDestroy();
	}

	m_pScene->Initialize();

	return _pPrev;
}

void CSceneManager::DisableSceneUI()
{
	m_pTimerUI->SetDisableUI(true);
	m_pScoreUI->SetDisableUI(true);
	m_pMultiHitUI->SetDisableUI(true);
	m_pArmBoxUI->SetDisableUI(true);
}

void CSceneManager::EnableSceneUI()
{
	m_pTimerUI->SetDisableUI(false);
	m_pScoreUI->SetDisableUI(false);
	m_pMultiHitUI->SetDisableUI(false);
	m_pArmBoxUI->SetDisableUI(false);
}

void CSceneManager::NextSceneSequence()
{
	m_pScene->Sequence();
	dynamic_cast<CTimerUI*>(m_pTimerUI)->SetTime(61.f);
}
