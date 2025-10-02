#pragma once
#include "CExplodeArea.h"
class CShoopDaWhoopArea : public CExplodeArea
{
public:
	CShoopDaWhoopArea();
	~CShoopDaWhoopArea() override;

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	int			m_iLivingFrame;
};

