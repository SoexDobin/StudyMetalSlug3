#pragma once
#include "CGameObject.h"


class CSpawner : public CGameObject
{
public:
	CSpawner();
	~CSpawner() override;

public:
	void		SetInstanceCount(const int& _iCount) 	{ m_iInstanceCount = _iCount; }
	const int	GetInstanceCount() const				{ return m_iInstanceCount; }
	void		SetInstanceTime(const int& _iCount)		{ m_iInstanceTime = _iCount; }
	const int	GetInstanceTime() const					{ return m_iInstanceTime; }

public:
	void	Initialize() override;
	int		Update() override;
	void	LateUpdate() override;
	void	Render(HDC _hDC) override;
	void	Release() override;
	void	OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

private:
	int		m_iInstanceCount;
	int		m_iInstanceTime;
	float	m_fSpawnDelta;

	Vector2	n_iOffsetX[3]
	{
		{96.f, 0.f},
		{0.f, 0.f},
		{128.f, 0.f}
	};
};