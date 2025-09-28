#pragma once
#include "CGameObject.h"

class CCQCChecker : public CGameObject
{
public:
	CCQCChecker();
	virtual ~CCQCChecker() override;

public:
	bool	CatchEnemyCQCZone() { return m_bCatchEnemy; }

public:
	void			Initialize()																	override;
	int				Update()																		override;
	void			LateUpdate()																	override;
	void			Render(HDC _hDC)																override;
	void			Release()																		override;
	void			OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;

private:
	bool			m_bCatchEnemy;
};

