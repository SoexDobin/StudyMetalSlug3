#pragma once

#include "CGameObject.h"
#include <functional>
class CCollider;

#include "CHitBox.h"
#include "CTrigger.h"

class CColliderFactory
{
public:
	static CCollider* Create(CGameObject* _pParent, COLLIDER_TYPE _eType, function<bool()>* _pPred, function<void()>* _pCallBack)
	{
		CCollider* pCol = ColFactory(_eType, _pPred, _pCallBack);
		pCol->SetParent(_pParent);
		pCol->SetPivot(_pParent->GetPivot());
		pCol->SetSize(_pParent->GetSize());

		return pCol;
	}

private:
	static CCollider* ColFactory(COLLIDER_TYPE _eType, function<bool()>* _pPred, function<void()>* _pCallBack)
	{
		switch (_eType)
		{
		case HITBOX:
		{
			CHitBox* pHit = new CHitBox();
			pHit = new CHitBox();
			pHit->SetPenColor(RGB(0, 255, 0));
			pHit->SetColType(_eType);
			return pHit;
		}
		case TRIGGER:
		{
			CTrigger* pTri = new CTrigger();
			pTri->SetPenColor(RGB(0, 0, 255));
			pTri->SetPredicate(_pPred);
			pTri->SetCallBack(_pCallBack);
			pTri->SetColType(_eType);
			return pTri;
		}
		case COL_TYPE_END:
			break;
		default:
			break;
		}

		return nullptr;
	}
};

