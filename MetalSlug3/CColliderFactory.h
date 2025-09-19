#pragma once

#include "CGameObject.h"
class CCollider;


template <typename T>
class CColliderFactory
{
public:
	static CCollider* CreateHitBox(CGameObject* _pParent)
	{
		CCollider* pCol = new T();
		pCol->SetParent(_pParent);
		pCol->SetPivot(_pParent->GetPivot());
		pCol->SetSize(_pParent->GetSize());
		pCol->SetColType(HITBOX);
		pCol->SetPenColor(RGB(0, 255, 0));

		return pCol;
	}

	/*static CCollider* CreateTrigger(CGameObject* _pParent)
	{
		CCollider* pCol = new T();
		pCol->SetParent(pCol);
		pCol->SetPivot(_pParent->GetPivot());
		pCol->SetRenderType(HITBOX);
		pCol->SetPenColor(RGB(0, 0, 255));


		return pCol;
	}*/
};

