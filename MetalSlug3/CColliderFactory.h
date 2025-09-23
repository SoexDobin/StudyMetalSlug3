#pragma once

#include "CGameObject.h"
class CCollider;

class CColliderFactory
{
public:
	static CCollider* Create(CGameObject* _pParent, COLLIDER_TYPE _eType)
	{
		CCollider* pCol = new CCollider();
		pCol->SetParent(_pParent);
		pCol->SetPivot(_pParent->GetPivot());
		pCol->SetSize(_pParent->GetSize());
		pCol->SetColType(_eType);
		if (_eType == HITBOX)
			pCol->SetPenColor(RGB(0, 255, 0));
		else if (_eType == TRIGGER)
			pCol->SetPenColor(RGB(0, 0, 255));

		return pCol;
	}
};

