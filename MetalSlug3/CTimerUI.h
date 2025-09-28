#pragma once
#include "CUIObject.h"

class CTimerUI : public CUIObject
{
public:
	CTimerUI();
	~CTimerUI() override;

public:
	const float&	GetTime() { return m_fGameTime; }
	void			SetTime(const float& _fTime) { m_fGameTime = _fTime; }

public:
	void Initialize() override;
	int  Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;
	void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	void	LoadBmp();
	void	ChangeNumberIdx();

private:
	bool	m_bStopTimer;	
	float	m_fGameTime;
	int		m_iTensDigit;
	int		m_iUnitsDigit;
};

