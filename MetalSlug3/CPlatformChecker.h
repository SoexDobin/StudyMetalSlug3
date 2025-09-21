#pragma once
#include "CGameObject.h"

class CPlatformChecker : public CGameObject
{
public:
	CPlatformChecker();
	~CPlatformChecker() override;

public:
	bool			GetHasColWithPlatform()		{ return m_iFindPlatform; }
	float			GetColTopPosition()			{ return m_fTop; }
public:
	void			Initialize()																	override;
	int				Update()																		override;
	void			LateUpdate()																	override;
	void			Render(HDC _hDC)																override;
	void			Release()																		override;
	void			OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;

private:
	bool			m_iFindPlatform;
	float			m_fTop;
};
