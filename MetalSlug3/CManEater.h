#pragma once
#include "CEnemy.h"

class CAnimation;

class CManEater final : public CEnemy
{
private:
	enum MANEATER_STATE {IDLE, ATTACK, JUMP, DROP, MOVE, DEAD, MANEATER_END};
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
	void		CheckAttack();
	void		CheckBehaviour();
	bool		CheckDropCol();
	void		CheckPlatform();

	void		Idle();
	void		Chase();
	void		Jump();
	void		Drop();
	void		Attack();
	void		Damage(CGameObject* _pDamageArg);
	void		Dead();

	void		LoadAnimation();
	inline void	SetAnim(const TCHAR* _szKey, bool _bIsRepeat, MANEATER_STATE _eState, int _iLastFacingX = 0);
	inline void	SetAttackAnim(const TCHAR* _szKey, bool _bIsRepeat, MANEATER_STATE _eState, int _iLastFacingX = 0);
	
private:
	bool			m_bDead;
	float			m_fInvisibleDelta;
	float			m_fStateDelta;
	float			m_fMoveDelta;
	float			m_fJumpDelta;
	float			m_fDropDelta;

	Damage_TYPE		m_eLastHit;

	CGameObject*	m_pPlayer;
	MANEATER_STATE	m_eCurState;
	MANEATER_STATE	m_ePrevState;
	CAnimation*		m_pAnim;

	CGameObject*	m_pPlatformCol;
	CGameObject*	m_pForwardCol;
	CGameObject*	m_pUpperCol;
	CGameObject*	m_pLowerCol;
	CGameObject*	m_pAttackCol;

	CGameObject*	m_pAttackAnimObj;
	CAnimation*		m_pAttackAnim;
private:
	const float		n_fDelayDelta = 100.f;
	const float		n_fSeedJumpSpeed = -1300.f;
	const float		n_fMaxFall = 500.f;
	const float		n_fFallSpeed = 1800.f;
};

