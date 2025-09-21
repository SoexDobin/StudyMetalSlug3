#pragma once
#include "CProjectile.h"
#include "CHMProjectile.h"
#include "CObjectManager.h"

template <typename T>
class CProjectileFactory
{
public:
	static void CreateProjectile(Vector2 _vPivot)
	{
		CProjectile* pProjectile = new T();
		pProjectile->Initialize();
		pProjectile->SetPivot(_vPivot);
		CObjectManager::GetInstance().AddGameObject(pProjectile, PROJECTILE);
	}
	static void CreateProjectile(Vector2 _vPivot, Vector2 _vDir, const TCHAR* _szImgKey, int _iLayer)
	{
		CProjectile* pProjectile = new T();

		pProjectile->SetPivot(_vPivot);
		pProjectile->SetDirection(_vDir);
		pProjectile->SetImgKey(_szImgKey);
		pProjectile->SetImgLayer(_iLayer);

		pProjectile->Initialize();

		CObjectManager::GetInstance().AddGameObject(pProjectile, PROJECTILE);
	}
};

