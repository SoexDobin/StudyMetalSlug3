#pragma once
#include "CGameObject.h"

class CAnimation;

class CPlatform : public CGameObject
{
public:
	CPlatform();
	virtual ~CPlatform() override;

public:
	void	Initialize()										override;
	int		Update()											override;
	void	LateUpdate()										override;
	void	Render(HDC _hDC)									override;
	void	Release()											override;
	void	OnCollision(CGameObject* _pCol, Vector2 _vColSize)	override;

private:

};

