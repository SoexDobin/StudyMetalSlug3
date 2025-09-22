#pragma once
#include "CEnemy.h"

class CAnimation;

class CManEater final : public CEnemy
{
public:
	CManEater();
	~CManEater() override;

public:
	void		Initialize()										override;
	int			Update()											override;
	void		LateUpdate()										override;
	void		Render(HDC _hDC)									override;
	void		Release()											override;
	void		OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;

private:
	void		CheckPlatform();
	void		ChasePlayer();
	void		Jump();
	void		Attack();
	void		Damage(CGameObject* _pDamageArg);
	void		Dead();


	void		LoadAnimation();
	
private:
	bool			m_bDead;
	CAnimation*		m_pAnim;
	CGameObject*	m_pPlatformCol;
	CGameObject*	m_pRefPlayer;

};

