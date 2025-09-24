#pragma once
#include "CGameObject.h"
class CChangeTrigger :
    public CGameObject
{
public:
	CChangeTrigger();
	virtual ~CChangeTrigger() override;
public:


public:
	void			Initialize()																	override;
	int				Update()																		override;
	void			LateUpdate()																	override;
	void			Render(HDC _hDC)																override;
	void			Release()																		override;
	void			OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;
};

