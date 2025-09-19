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
	float			m_fShootDelta;

	float			m_fMoveSpeed;
	float			m_fCrawlSpeed;
	
	bool			m_bIsJump;
	float			m_fJumpDeltaTime;
	
private:
	const Vector2	n_vStandOffset = Vector2(0.f, 60.f);
	const Vector2	n_vSitOffset = Vector2(0.f, 80.f);
	const float		n_fScatterArg[5] = { 0.f, 0.02f, -0.01, 0.01f, -0.03f };
	const float		n_fGravity = 1.f;
	const float		n_fJumpSeed = 1.f;
	const char		n_cJumpKey = 'D';
	const char		n_cAttackKey = 'A';
};

