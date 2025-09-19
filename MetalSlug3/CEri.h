#pragma once
#include "CGameObject.h"

class CAnimation;
class CCollider;

class CEri : public CGameObject
{
	enum PLAYER_STATE { SIT, STAND, IDLE, MOVE, SHOOT, CQC, JUMP, MOVEJUMP, DROP, PLAYER_STATE_END };
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
	void		BehaviourKeyInput();
	void		AttackKeyInput();
	void		Move();
	void		Jump();
	void		Drop();
	void		Shoot();

	void		LoadEriBmp();
	void		LoadProjectileBmp();
	void		WinOffset(const float& _fCurSpeed);

private:
	CAnimation*		m_pBodyAnim;
	CAnimation*		m_pLegAnim;
	CGameObject*	m_pCQCCollider;

	PLAYER_STATE	m_eCurBodyState;
	PLAYER_STATE	m_ePrevBodyState;
	PLAYER_STATE	m_eCurLegState;
	PLAYER_STATE	m_ePrevLegState;

	int				m_iScatterIdx;

	float			m_fMoveSpeed;
	float			m_fCrawlSpeed;
	
	bool			m_bIsJump;
	float			m_fJumpDeltaTime;
	
private:
	const float		m_fGravity = 1.f;
	const float		m_fJumpSeed = 1.f;
	const char		m_cJumpKey = 'D';
	const char		m_cAttackKey = 'A';
};

