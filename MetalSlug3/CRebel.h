#pragma once
#include "CEnemy.h"

class CAnimation;

class CRebel : public CEnemy
{	
private:
	enum REBEL_STATE { CHASE, IDLE, DEAD, REBEL_END };
public:
	CRebel();
	~CRebel() override;

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void		Idle();
	void		Chase();
	void		Dead();
	void		CheckPlatform();
	void		LoadAnimation();

private:
	bool			m_bDead;
	REBEL_STATE		m_eState;
	float			m_fBeside;
	float			m_fSpeed;
	float			m_fInvisibleDelta;
	CAnimation*		m_pAnim;
	CGameObject*	m_pPlatformCol;
	CGameObject*	m_pPlayer;

	OBJECT_TYPE		m_eLastHit;
};

