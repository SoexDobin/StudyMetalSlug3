#pragma once
#include "CExplodeArea.h"

class CManEaterArea : public CExplodeArea
{
public:
	CManEaterArea();
	~CManEaterArea() override;

public:


private:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	int m_iLivingFrame;
};

