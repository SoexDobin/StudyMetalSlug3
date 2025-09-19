#pragma once
#include "CGameObject.h"

class CAnimation;

class CField : public CGameObject
{
public:
	CField();
	virtual ~CField() override;
	
public:
	CAnimation*		GetAnimation()							{ return m_pAnim; }
	void			SetAnimation(CAnimation* _pAnim)		{ m_pAnim = _pAnim; }
	const TCHAR*	GetAnimationKEy()						{ return m_szAnimKey; }
	void			SetAnimation(const TCHAR* _szKey)		{ m_szAnimKey = _szKey; }

public:
	void	Initialize()										override;
	int		Update()											override;
	void	LateUpdate()										override;
	void	Render(HDC _hDC)									override;
	void	Release()											override;
	void	OnCollision(CGameObject* _pCol, Vector2 _vColSize)	override;

private:
	CAnimation*		m_pAnim;
	const TCHAR*	m_szAnimKey;
};

