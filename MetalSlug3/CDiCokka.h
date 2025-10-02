#pragma once
#include "CEnemy.h"

class CAnimation;

class CDiCokka : public CEnemy
{
private:
	enum DICOKKA_STATE { IDLE, READY, ATTACK, DESTROY, STATE_END };
public:
	CDiCokka();
	~CDiCokka() override;

public:
	void Initialize() override;

	int Update() override;

	void LateUpdate() override;

	void Render(HDC _hDC) override;

	void Release() override;

	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void		Destroy();
	void		Idle();
	void		Ready();
	void		Attack();
	void		CheckPlayer();
	void		LoadAnimation();
private:
	CAnimation*		m_pAnim;
	CGameObject*	m_pAttackCol;

	DICOKKA_STATE	m_eState;
	float			m_fInvisibleDelta;
	float			m_fDelta;
	float			m_fShootDelta;
};

