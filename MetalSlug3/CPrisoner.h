#pragma once
#include "CGameObject.h"

class CAnimation;
class CItem;

class CPrisoner : public CGameObject
{
public:
	CPrisoner();
	~CPrisoner() override;

public:
	CGameObject*	GetSupplyItem() const			{ return m_pSupplyItem; }
	void			SetSupplyItem(CGameObject* _pItem)	{ m_pSupplyItem = _pItem; }

public:

	// CGameObject을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void GoLeft();
	void Patrol();
	void LoadBmp();
	void CheckPlatform();

private:
	CGameObject*	m_pSupplyItem;
	CGameObject*	m_pPlatformCol;
	CAnimation*		m_pAnim;
	bool			m_bEndSupply;
	bool			m_bPatrolSwitch;
	bool			m_bRunAway;
	float			m_fPatrolDelta;
};

