#pragma once
#include "CEnemy.h"

class CAnimation;

class CRebelBazooca : public CEnemy
{
private:
	enum REBEL_STATE { CHASE, IDLE, SHOOT, DEAD, REBEL_END };
public:
	CRebelBazooca();
	~CRebelBazooca() override;

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void		Idle();
	void		Shoot();
	void		Chase();
	void		Dead();
	void		CheckPlatform();
	void		LoadAnimation();

private:
	bool			m_bDead;
	REBEL_STATE		m_eState;
	float			m_fBeside;
	float			m_fSpeed;
	float			m_fShootDelta;
	float			m_fInvisibleDelta;
	CAnimation*		m_pAnim;
	CGameObject*	 m_pPlatformCol;
	CGameObject*	m_pPlayer;
};

