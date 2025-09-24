#pragma once
#include "CScene.h"

class CGameObject;

class CMission4 final : public CScene
{
public:
	CMission4();
	~CMission4() override;

public:
	void						Initialize()		override;
	pair<bool, SCENETAG>		Update()			override;
	void						LateUpdate()		override;
	void						Render(HDC _hDC)	override;
	void						Release()			override;
	void						Sequence()			override;

private:
	void					RenderDessertLandscape(HDC _hDC);
	void					RenderFrontLandscape(HDC _hDC);

	void					CreatetTriggerArea();
	void					CreatePlatform();
	void					LoadBmpDessertLandscape();
	void					LoadBmpEnemy();

private:
	int						m_iScrollLockIdx;
	float					m_fDelta;

	vector<CGameObject*>	m_vecPlatform;

	int						m_iRuinBGIdx;
	list<const TCHAR*>		m_DessertBGKeyList;
	vector<const TCHAR*>	m_vecDessertRuinBGKey;
	vector<const TCHAR*>	m_vecDessertFrontgroundKey;
	

private:
	Vector2		n_vDesertMaxLockPoints[3] =
	{
		Vector2(2480.f, (float)WINCY),
		Vector2(6526.f, (float)WINCY),
		Vector2(  )
	};
};

