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

	static CGameObject* Create(const Vector2& _vPivot, const Vector2& _vSize, CGameObject* _pParent = nullptr)
	{
		CGameObject* pObject = new T;
		pObject->SetPivot(_vPivot);
		pObject->SetSize(_vSize);
		pObject->SetParent(_pParent);
		pObject->Initialize();

		return pObject;
	}
};

