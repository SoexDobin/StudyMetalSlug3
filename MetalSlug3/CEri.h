#pragma once
#include "CGameObject.h"

class CAnimation;
class CCollision;

class CEri : public CGameObject
{
	enum PLAYER_STATE { SIT, IDLE, MOVE, SHOOT, CQC, JUMP, MOVEJUMP, DROP, PLAYER_STATE_END };
public:
	CEri();
	virtual ~CEri() override;

public:
	int			GetLastFacingX()	{ return m_iLastFaceX; }

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

	void		LoadEriBmp();

private:
	CAnimation*		m_pBodyAnim;
	CAnimation*		m_pLegAnim;
	CCollision*		m_pHitBox;
	CCollision*		m_pCQCCollider;

	PLAYER_STATE	m_eCurBodyState;
	PLAYER_STATE	m_ePrevBodyState;
	PLAYER_STATE	m_eCurLegState;
	PLAYER_STATE	m_ePrevLegState;
	
	int				m_iLastFaceX;

private:
	const char		m_cJumpKey = 'D';
	const char		m_cAttackKey = 'A';
	const TCHAR*	m_bmpPATH = L"../Resource/Bmp/Eri/";
};

