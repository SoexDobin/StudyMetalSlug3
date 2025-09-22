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
	void	OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;

private:
	void	PlayerPlatformCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag);
	void	EnemyPlatformCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag);

private:

	const TCHAR*		m_szImgKey;
};

