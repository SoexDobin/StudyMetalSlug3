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
			it = m_ParticleList.erase(it);
			(*it)->Update();
		}

		(*it)->LateUpdate();
	}
}

void CParticleManager::Render(HDC _hDC)
{
	for_each(m_ParticleList.begin(), m_ParticleList.end()
		, [=](CParticle* pParticle) -> void
		{
			pParticle->Render(_hDC);
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
}

