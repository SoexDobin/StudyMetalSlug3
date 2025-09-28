#pragma once
#include "CGameObject.h"

class CPlatformChecker : public CGameObject
{
public:
	CPlatformChecker();
	~CPlatformChecker() override;

public:
	bool			GetHasColWithPlatform()		{ return m_bFindPlatform; }
	float			GetColTopPosition()			{ return m_fTop; }

	void			SetOffset(const Vector2& _vOffset)	{ m_vOffset = _vOffset; }
	const Vector2	GetOffset()							{ return m_vOffset; }

public:
	void			Initialize()																	override;
	int				Update()																		override;
	void			LateUpdate()																	override;
	void			Render(HDC _hDC)																override;
	void			Release()																		override;
	void			OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag)	override;

private:
	bool			m_bFindPlatform;
	float			m_fTop;
	Vector2			m_vOffset;
};
