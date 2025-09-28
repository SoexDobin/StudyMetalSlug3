#pragma once
#include "CUIObject.h"
class CArmUI : public CUIObject
{
public:
	CArmUI();
	~CArmUI() override;

public:
	void Initialize() override;
	int  Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;
private:
	void LoadBmp();

private:
	TCHAR	m_szArmoBuffer[16];
	TCHAR	m_szBombBuffer[16];
};

