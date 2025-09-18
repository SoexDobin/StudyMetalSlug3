#pragma once
#include "CSingleTon.h"

class CCollisionManager : public CSingleTon<CCollisionManager>
{
	friend class CSingleTon;
public:
	void CheckCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2, COLLISION_FLAG _eFlag);
	void RenderCollisionBox(HDC _hDC);

private:
	void RTRCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2);
	void CTCCollision(list<CGameObject*>& _Src1, list<CGameObject*>& _Src2);

private:
	bool	m_bIsRender = false;
};

