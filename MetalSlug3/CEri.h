#pragma once
#include "CGameObject.h"
#include "Define.h"

class CAnimation;
class CCollider;

class CEri : public CGameObject
{
	enum PLAYER_STATE { SIT, STAND, IDLE, MOVE, SHOOT, BOMB, CQC, JUMP, MOVEJUMP, DROP, RESPAWN, DEAD, PLAYER_STATE_END };

public:
	CEri();
	virtual ~CEri() override;

public:
	void			Initialize()										override;
	int				Update()											override;
	void			LateUpdate()										override;
	void			Render(HDC _hDC)									override;
	void			Release()											override;
	void			OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)  override;

public:
	const int&		GetArmo() const						{ return m_iArmo; }
	void			LoseArmo(const int& _iArmo)			{ m_iArmo -= _iArmo; }
	void			SetArmo(const int& _iArmo) 			{ m_iArmo = _iArmo; }

	const int&		GetBomb() const						{ return m_iBomb; }
	void			LoseBomb(const int& _iBomb)			{ m_iBomb -= _iBomb; }
	void			SetBomb(const int& _iBomb)			{ m_iBomb = _iBomb; }
	
	WEAPON_TYPE		GetCurWeapon() const				{ return m_eCurWeapon; }
	void			SetCurWeapon(WEAPON_TYPE _eCurWeapon)	{ m_eCurWeapon = _eCurWeapon; }

public:
	void			Dead();
	void			CheckMissionComplete(const bool& _bIsComplete) { m_bMissionComplete = _bIsComplete; }

private:
	void			BehaviourKeyInput();
	void			AttackKeyInput();

	void			Move();
	void			Jump();
	void			Drop();
	void			Shoot();

	void			ShootHeavyMachineGun(Vector2 _vPos);
	void			ShootIronLizzard(Vector2 _vPos);
	void			ShootFlameShot(Vector2 _vPos);

	void			CheckOutOfBound();

	void			CheckPlatform();
	void			IdentifyPlatform(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag);

	inline void		SetStandCollider();
	inline void		SetSitCollider();

	inline void		SetLegAnim(const TCHAR* _szKey, bool _bIsRepeat, PLAYER_STATE _eLegState, int _iLastFacingX = 0);
	inline void		SetBodyAnim(const TCHAR* _szKey, bool _bIsRepeat, PLAYER_STATE _eBodyState, int _iLastFacingX = 0);

	void			LoadEriBmp();
	void			LoadProjectileBmp();
	void			WinOffsetX(const float& _fCurSpeed);
	void			WinOffsetY(const float& _fCurSpeed);

private:
	CAnimation*		m_pBodyAnim;
	CAnimation*		m_pLegAnim;
	CAnimation*		m_pDeadAnim;
	CAnimation*		m_pSpawnAnim;
	CGameObject*	m_pSpawnAnimObj;
	CGameObject*	m_pCQCCol;
	CGameObject*	m_pPlatformCol;

	PLAYER_STATE	m_eCurBodyState;
	PLAYER_STATE	m_ePrevBodyState;
	PLAYER_STATE	m_eCurLegState;
	PLAYER_STATE	m_ePrevLegState;

	float			m_fInvisibleDelta;

	int				m_iScatterIdx;
	float			m_fShootDelta;
	float			m_fBombDelta;

	float			m_fMoveSpeed;
	float			m_fCrawlSpeed;
	
	bool			m_bIsJump;
	float			m_fJumpSpeed;

	bool			m_bIsDrop;

	bool			m_bMissionComplete;

	WEAPON_TYPE		m_eCurWeapon;
	int				m_iArmo;
	int				m_iBomb;
	
private:
	const float		n_fShootDelta				= 70.f;
	const float		n_fBombDelta				= 100.f;
	const float		n_fSeedJumpSpeed			= -1000.f;
	const float		n_fMaxFall					= 700.f;
	const float		n_fFallSpeed				= 1800.f;
	const Vector2	n_vPlayerColOffset			= Vector2(0.f, 96.f - 18.f);
	const Vector2	n_vPlayerColSitOffset		= Vector2(0.f, 96.f - 6.f);
	const Vector2	n_vShootOffset				= Vector2(0.f, 60.f);
	const Vector2	n_vShootSitOffset			= Vector2(0.f, 80.f);
	const Vector2	n_vSpawnSize				= Vector2(768.f, 144.f);
	const float		n_fScatterArg[5]			= { 0.f, 0.03f, -0.02f, 0.02f, -0.04f };
	const char		n_cJumpKey = 'D';
	const char		n_cAttackKey = 'A';
	const char		n_cBombKey = 'S';
};

