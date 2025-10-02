#pragma once
#include "CExplodeArea.h"

class CAnimation;

class CBombExplode : public CExplodeArea
{
public:
	CBombExplode();
	~CBombExplode() override;

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void LoadBmp();

private:
	CAnimation*		m_pAnim;
	int				m_iLivingFrame;
};

