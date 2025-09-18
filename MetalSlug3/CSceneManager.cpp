#include "pch.h"
#include "CSceneManager.h"
#include "CEri.h"
#include "CScene.h"
#include "CArmoryScene.h"

// Managers
#include "CObjectManager.h"
#include "CCollisionManager.h"

CSceneManager::CSceneManager() 
	: m_pScene(nullptr), m_eCurScene(SCENE_END), m_ePrevScene(SCENE_END)
{
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	// TODO : 게임 흐름 구축 시 변경 해야 합니다.
	m_eCurScene = ARMORY_SCENE;
	ChangeScene(m_eCurScene);
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).push_back(new CEri());
	CObjectManager::GetInstance().GetGameObjectList(PLAYER).front()->Initialize();
	ref_pPlayer = CObjectManager::GetInstance().GetGameObjectList(PLAYER).front();
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
		
}

void CSceneManager::Render(HDC _hDC)
{
	m_pScene->Render(_hDC);

	CCollisionManager::GetInstance().CheckCollision(
		CObjectManager::GetInstance().GetGameObjectList(PLAYER)
		, CObjectManager::GetInstance().GetGameObjectList(ENEMY)
		, RECT_TO_RECT);
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
	case MISSION_SCENE:		break;
	case EDIT:				break;

	case SCENE_END:
		break;
	default:
		break;
	}

	m_pScene->Initialize();

	return m_ePrevScene;
}
