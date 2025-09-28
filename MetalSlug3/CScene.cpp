#include "pch.h"
#include "CScene.h"

CScene::CScene() : m_bDestroyScene(SCENE_NOEVENT)
{
	ZeroMemory(&m_vSpawnPoint, sizeof(Vector2));
}

CScene::~CScene()
{
}