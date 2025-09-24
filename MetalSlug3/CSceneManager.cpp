#include "pch.h"
#include "CSceneManager.h"
#include "CEri.h"
#include "CScene.h"
#include "CArmoryScene.h"
#include "CMission4.h"
#include "CMission4Pyramid.h"
#include "CMission4BossStage.h"
#include "CMouse.h"

// Managers
#include "CObjectManager.h"
#include "CCollisionManager.h"
#include "CGameObjectFactory.h"
#include "CScrollManager.h"
#include "CSoundManager.h"

CSceneManager::CSceneManager() 
	: m_pScene(nullptr), m_pPrevScene(nullptr), m_eCurScene(SCENE_END)
	, ref_pPlayer(nullptr)
{

}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	// TODO : 게임 흐름 구축 시 변경 해야 합니다.
	m_eCurScene = MISSION_SCENE_2;
	
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).push_back(new CEri());
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).front()->Initialize();
	ref_pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();
	ChangeScene(m_eCurScene);

	CObjectManager::GetInstance().AddGameObject(
		CGameObjectFactory<CMouse>::Create(), PLAYER);
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).back()->Initialize();
}

void CSceneManager::Update()
{	
	pair<bool, SCENETAG> bSceneState = m_pScene->Update();

	if (bSceneState.first && m_pPrevScene == nullptr)
	{
		m_pPrevScene = ChangeScene(bSceneState.second);
		SafeDelete<CScene*>(m_pPrevScene);
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

	CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(PLAYER));
	CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(ENEMY));
	CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(PROJECTILE));
	CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(PLATFORM));
	CCollisionManager::GetInstance().RenderCollisionBox(_hDC, CObjectManager::GetInstance().GetGameObjectList(NEUTRAL));
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
	case SELECT_SCENE:		break;
	case ARMORY_SCENE:		m_pScene = new CArmoryScene();			break;
	case MISSION_SCENE_3:	m_pScene = new CMission4BossStage();	break;
	case MISSION_SCENE_2:	m_pScene = new CMission4Pyramid();		break;
	case MISSION_SCENE_1:	m_pScene = new CMission4();				break;
	case EDIT:				break;

	case SCENE_END:
		break;
	default:
		break;
	}

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

void CSceneManager::NextSceneSequence()
{
	m_pScene->Sequence();
}
