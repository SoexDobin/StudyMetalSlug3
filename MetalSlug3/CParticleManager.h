#pragma once
#include "CSingleTon.h"
#include "CParticle.h"

class CParticleManager final : public CSingleTon<CParticleManager>
{
	friend class CSingleTon;
public:
	CParticleManager();
	~CParticleManager() override;

public:
	template <typename T>
	void CreateParticle();
	template <typename T>
	void CreateParticle(Vector2 _vPivot);

	void Update();
	void Render(HDC _hDC);
	void Release();

	void Initialize();

private:
	list<CParticle*>	m_ParticleList;
};

template<typename T>
inline void CParticleManager::CreateParticle()
{
	CParticle* pParticle = new T();
	m_ParticleList.push_back(pParticle);
	pParticle->Initialize();
}
template<typename T>
inline void CParticleManager::CreateParticle(Vector2 _vPivot)
{
	CParticle* pParticle = new T();
	m_ParticleList.push_back(pParticle);
	pParticle->Initalize();
	pParticle->SetPivot(_vPivot);
}