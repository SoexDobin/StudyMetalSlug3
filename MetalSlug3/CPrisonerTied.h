#pragma once
#include "CEnemy.h"

class CAnimation;

class CPrisonerTied : public CEnemy
{
public:
	CPrisonerTied();
	~CPrisonerTied() override;

public:
	void	LockUnravel() { m_bLockUnravel = true; }

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void CheckPlatform();
	void LoadBmp();

private:
	CGameObject*	m_pPlatformCol;
	CAnimation*		m_pAnim;
	bool			m_bIsUnravel;


	bool			m_bLockUnravel;
};

