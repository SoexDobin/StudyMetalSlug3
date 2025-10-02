#include "pch.h"
#include "CParticleManager.h"
#include "CBmpManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
	Release();
}


void CParticleManager::Update()
{
	for (auto it = m_ParticleList.begin(); it != m_ParticleList.end(); )
	{
		bool bIsDestroy = (*it)->Update();

		if (bIsDestroy)
		{
			SafeDelete<CParticle*>(*it);
			it = m_ParticleList.erase(it);
		}
		else
			++it;
	}
}

void CParticleManager::LateUpdate()
{
	for_each(m_ParticleList.begin(), m_ParticleList.end()
		, [](CParticle* _pParticle) -> void
		{
			_pParticle->LateUpdate();
		});
}

void CParticleManager::Render(HDC _hDC)
{
	for_each(m_ParticleList.begin(), m_ParticleList.end()
		, [=](CParticle* _pParticle) -> void
		{
			_pParticle->Render(_hDC);
		});
}

void CParticleManager::Release()
{
	for_each(m_ParticleList.begin(), m_ParticleList.end()
		, [](CParticle* _p) -> void
		{
			if (_p)
			{
				delete _p;
				_p = nullptr;
			}
		});
}

void CParticleManager::Initialize()
{
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/BulletHit.bmp"
		, L"BulletHit");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/Mission4Start/Mission4Start.bmp"
		, L"Mission4Start");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/Mission4Start/Mission4Start_Blank.bmp"
		, L"Mission4Start_Blank");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/Mission4Complete/Mission4Complete.bmp"
		, L"Mission4Complete");

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/SDW/SDW1.bmp", L"SDW1");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/SDW/SDW2.bmp", L"SDW2");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/SDW/SDW3.bmp", L"SDW3");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/SDW/SDW4.bmp", L"SDW4");

	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/Explode/Big_Explode.bmp"
		, L"Big_Explode");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/Explode/Medium_Explode.bmp"
		, L"Medium_Explode");
	CBmpManager::GetInstance().InsertBmp(L"../Resource/Bmp/Particle/Explode/Bomb_Explode.bmp"
		, L"Bomb_Explode");
}

