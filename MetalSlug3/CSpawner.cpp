#include "pch.h"
#include "CSpawner.h"
#include "CTimeManager.h"
#include "CObjectManager.h"
#include "CGameObjectFactory.h"
#include "CManEater.h"

CSpawner::CSpawner()
	: m_fSpawnDelta(0.f), m_iInstanceCount(0), m_iInstanceTime(0)
{
}

CSpawner::~CSpawner()
{
	Release();
}

void CSpawner::Initialize()
{
	m_iInstanceCount = 2;
	m_iInstanceTime = 2;
	m_fSpawnDelta = 100.f;
}

int CSpawner::Update()
{
	if (m_bDestroy) return OBJ_DESTROY;
	
	__super::UpdateGameObject();

	m_fSpawnDelta += 100 * DELTA;

	return OBJ_NOEVENT;
}

void CSpawner::LateUpdate()
{
	if (m_fSpawnDelta >= 100.f)
	{
		--m_iInstanceTime;
		m_fSpawnDelta = 0.f;
		for (int i = 0; i < m_iInstanceCount; ++i)
		{
			Vector2 vPos = m_vPivot + n_iOffsetX[i];

			CObjectManager::GetInstance().AddGameObject(CGameObjectFactory<CManEater>::Create(), ENEMY);
			CObjectManager::GetInstance().GetGameObjectList(ENEMY).back()->SetPivot(vPos);
		}
	}

	if (m_iInstanceTime < 0) m_bDestroy = true;
}

void CSpawner::Render(HDC _hDC)
{
}

void CSpawner::Release()
{
}

void CSpawner::OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)
{
}
