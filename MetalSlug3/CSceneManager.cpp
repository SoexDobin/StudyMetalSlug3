#include "pch.h"
#include "CSceneManager.h"
#include "CEri.h"
#include "CScene.h"
#include "CArmoryScene.h"
#include "CMission4.h"
#include "CMission4Pyramid.h"
#include "CMouse.h"

// Managers
#include "CObjectManager.h"
#include "CCollisionManager.h"
#include "CGameObjectFactory.h"
#include "CScrollManager.h"

CSceneManager::CSceneManager() 
	: m_pScene(nullptr), m_eCurScene(SCENE_END), m_ePrevScene(SCENE_END)
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
	m_eCurScene = MISSION_SCENE;
	ChangeScene(m_eCurScene);
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).push_back(new CEri());
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).front()->Initialize();
	ref_pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();
	
	CObjectManager::GetInstance().AddGameObject(
		CGameObjectFactory<CMouse>::Create(), PLAYER);
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).back()->Initialize();
}

void CSceneManager::Update()
{
	// TODO : 씬 인스터스 및 씬 체인지
	// SelectPlayer
	// Armory 테스트맵
	// Edit 맵
	// Mission 
	// Boss
	
	pair<bool, SCENETAG> bSceneState = m_pScene->Update();

	if (bSceneState.first) ChangeScene(bSceneState.second);
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

SCENETAG CSceneManager::ChangeScene(SCENETAG _eTag)
{
	m_ePrevScene	= m_eCurScene;
	m_eCurScene		= _eTag;
	SafeDelete<CScene*>(m_pScene);

	if (_eTag == PREV_SCENE)
		return ChangeScene(m_ePrevScene);

	switch (_eTag)
	{
	case MAIN_SCENE:		break;
	case SELECT_SCENE:		break;
	case ARMORY_SCENE:		m_pScene = new CArmoryScene();		break;
	//case MISSION_SCENE:		m_pScene = new CMission4Pyramid();			 break;
	case MISSION_SCENE:		m_pScene = new CMission4();			 break;
	case EDIT:				break;

	case SCENE_END:
		break;
	default:
		break;
	}

	m_pScene->Initialize();

	return m_ePrevScene;
}
