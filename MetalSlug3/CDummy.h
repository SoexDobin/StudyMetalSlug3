#pragma once
#include "CGameObject.h"

#include "CAnimation.h"

class CDummy : public CGameObject
{
public:
	CDummy();
	virtual ~CDummy() override;

public:
	const bool&		GetColWithPlayer() const { return m_bColWithPlayer; }

public:
	void Initialize() override;

	int Update() override;

	void LateUpdate() override;

	void Render(HDC _hDC) override;

	void Release() override;

	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	bool	m_bColWithPlayer;
};

