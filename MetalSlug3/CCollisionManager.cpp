#include "pch.h"
#include "Define.h"
#include "CCollisionManager.h"
#include "CGameObject.h"
#include "CKeyManager.h"

CCollisionManager::CCollisionManager() : m_bIsRender(true)
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::CheckCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2, COLLISION_FLAG _eFlag)
{
	switch (_eFlag)
	{
	case RECT_TO_RECT:		RTRCollision(_Src1, _Src2); break;
	case CIRCLE_TO_CIRCLE:	CTCCollision(_Src1, _Src2); break;
	case RECT_TO_CIRCLE:	RTRCollision(_Src1, _Src2); break;
	case CIRCLE_TO_RECT:	RTRCollision(_Src1, _Src2); break;
	default: break;
	}
}

void CCollisionManager::RenderCollisionBox(HDC _hDC, list<CGameObject*>& _Src)
{
	if (CKeyManager::GetInstance().KeyDown(VK_F1))
		m_bIsRender = !m_bIsRender;

	if (m_bIsRender == false) return;

	for (auto& obj : _Src)
	{
		if (obj->GetCollider() == nullptr) continue;

		obj->GetCollider()->RenderCollider(_hDC);
	}
}

void CCollisionManager::RTRCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2)
{
	for (auto& Src1 : _Src1)
	{
		for (auto& Src2 : _Src2)
		{
			if (Src1->GetCollider() == nullptr) continue;
			if (Src2->GetCollider() == nullptr) continue;

			CCollider* pCol1 = Src1->GetCollider();
			CCollider* pCol2 = Src2->GetCollider();

			Vector2 vFrom	= pCol1->GetPivot();
			Vector2 vTo		= pCol2->GetPivot();
			Vector2 vDst = vFrom - vTo;
			vDst.x = fabsf(vDst.x);
			vDst.y = fabsf(vDst.y);

			Vector2 vFromSize(pCol1->GetSize()), vToSize(pCol2->GetSize());
			Vector2 fSumHalfSize = (vFromSize + vToSize) * 0.5f;

			bool isCol = fSumHalfSize.x >= vDst.x && fSumHalfSize.y >= vDst.y;
			if (isCol == true)
			{
				pCol1->SetCollision(true);
				pCol2->SetCollision(true);
				// 반지름의 합 - 중점 사이 거리
				Vector2 vDifference = fSumHalfSize - vDst;
				vDifference = Vector2(fabsf(vDifference.x), fabsf(vDifference.y));
				if (vDifference.x > vDifference.y) // To(Src)기준 상하 충돌
				{
					vDifference.x = 0.f;
					if (vFrom.y < vTo.y) // 상 충돌
					{
						Src1->OnCollision(Src2, vDifference, DOWN_COL);
						Src2->OnCollision(Src1, vDifference, UP_COL);
					}
					else if (vFrom.y >= vTo.y) // 하 충돌
					{
						Src1->OnCollision(Src2, vDifference, UP_COL);
						Src2->OnCollision(Src1, vDifference, DOWN_COL);
					}
				}
				else if (vDifference.x <= vDifference.y) // To(Src)의 좌우 충돌
				{
					vDifference.y = 0.f;
					if (vFrom.x < vTo.x) // 우 충돌
					{
						Src1->OnCollision(Src2, vDifference, RIGHT_COL);
						Src2->OnCollision(Src1, vDifference, LEFT_COL);
					}
					else if (vFrom.x >= vTo.x) // 좌 충돌
					{
						Src1->OnCollision(Src2, vDifference, LEFT_COL);
						Src2->OnCollision(Src1, vDifference, RIGHT_COL);
					}
				}
			}
			else
			{
				Src1->GetCollider()->SetCollision(false);
				Src2->GetCollider()->SetCollision(false);
			}
		}
	}
}


void CCollisionManager::CTCCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2)
{
	for (auto& Src1 : _Src1)
	{
		if (Src1->GetCollider() == nullptr) continue;

		for (auto& Src2 : _Src2)
		{
			if (Src2->GetCollider() == nullptr) continue;

			Vector2 vDst = Src1->GetPivot() - Src2->GetPivot();
			float fHypot = sqrtf(vDst.GetSquared());
			float fSumRadius = (Src1->GetSize().x + Src2->GetSize().x) * 0.5f;

			if (fSumRadius >= fHypot)
			{
				Src1->GetCollider()->SetCollision(true);
				Src2->GetCollider()->SetCollision(true);
				Src1->OnCollision(Src2, Vector2::UnitX, COL_END);
				Src2->OnCollision(Src1, Vector2::UnitX, COL_END);
			}
			else
			{
				Src1->GetCollider()->SetCollision(false);
				Src2->GetCollider()->SetCollision(false);
			}
		}
	}
}

