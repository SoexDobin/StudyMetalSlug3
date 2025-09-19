#pragma once
#include "CGameObject.h"

class CCQCArea : public CGameObject
{
public:
	CCQCArea();
	virtual ~CCQCArea() override;

public:
	bool	CatchEnemyCQCZone() { return m_bCatchEnemy; }

private:
	void Initialize() override;

	int Update() override;

	void LateUpdate() override;

	void Render(HDC _hDC) override;

	void Release() override;

	void OnCollision(CGameObject* _pCol, Vector2 _vColSize) override;

private:
	CGameObject*	m_pPlayer;
	bool			m_bCatchEnemy;
};

