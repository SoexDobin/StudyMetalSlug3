#pragma once
#include "CGameObject.h"

class CEri : public CGameObject
{
public:
	CEri();
	virtual ~CEri() override;


public:
	void		Initialize()										override;
	int			Update()											override;
	void		LateUpdate()										override;
	void		Render(HDC _hDC)									override;
	void		Release()											override;
	void		OnCollision(CGameObject* _pCol, Vector2 _vColSize)  override;

	
private:
	void		KeyInput();

private:
	
};

