#pragma once
#include "CEnemy.h"

class CManEater final : public CEnemy
{
public:
	CManEater();
	virtual ~CManEater() override;

public:
	void		Initialize()										override;
	int			Update()											override;
	void		LateUpdate()										override;
	void		Render(HDC _hDC)									override;
	void		Release()											override;
	void		OnCollision(CGameObject* _pCol, Vector2 _vColSize)	override;

private:

};

