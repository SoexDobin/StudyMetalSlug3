#pragma once
#include "CSingleTon.h"
#include "CGameObject.h"

class CCollisionManager final : public CSingleTon<CCollisionManager>
{
	friend class CSingleTon;
private:
	CCollisionManager();
	~CCollisionManager() override;
public:
	void CheckCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2, COLLISION_FLAG _eFlag);
	void RenderCollisionBox(HDC _hDC, list<CGameObject*>& _Src1);

private:
	void RTRCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2);
	void CTCCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2);

private:
	bool	m_bIsRender;
};

