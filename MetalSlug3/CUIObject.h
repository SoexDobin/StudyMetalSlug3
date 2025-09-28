#pragma once
#include "CGameObject.h"

class CUIObject : public CGameObject
{
public:
	CUIObject();
	virtual ~CUIObject() override; 

public:
	void Initialize() PURE;
	int  Update() PURE;
	void LateUpdate() PURE;
	void Render(HDC _hDC) PURE;
	void Release() PURE;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) PURE;

public:
	bool 	GetDisableUI() { return m_bDisable; }
	void	SetDisableUI(bool _bIsDisable) { m_bDisable = _bIsDisable; }

protected:
	bool	m_bDisable;
};

