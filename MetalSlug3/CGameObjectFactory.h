#pragma once
#include "CGameObject.h"

template <typename T>
class CGameObjectFactory
{
public:
	static CGameObject* Create()
	{
		CGameObject* pObject = new T;
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* Create(float fX, float fY)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPivot(Vector2(fX, fY));

		return pObject;
	}
};

