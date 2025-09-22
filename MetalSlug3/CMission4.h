#pragma once
#include "CScene.h"

class CGameObject;

class CMission4 final : public CScene
{
public:
	CMission4();
	~CMission4() override;

public:
	void					Initialize()		override;
	pair<bool, SCENETAG>	Update()			override;
	void					LateUpdate()		override;
	void					Render(HDC _hDC)	override;
	void					Release()			override;

private:
	void					RenderDessertLandscape(HDC _hDC);
	void					RenderFrontLandscape(HDC _hDC);

	void					CreatePlatform();
	void					LoadBmpDessertLandscape();
	void					LoadBmpEnemy();

private:
	float					m_fDelta;

	int						m_iRuinBGIdx;
	list<const TCHAR*>		m_DessertBGKeyList;
	vector<const TCHAR*>	m_vecDessertRuinBGKey;
	vector<const TCHAR*>	m_vecDessertFrontgroundKey;

	CGameObject*			m_pTriNextScene;

private:
	Vector2		n_iDesertLockPoints[3] =
	{
		Vector2(),
		Vector2(),
		Vector2(),
	};

	//Vector2		n_DesertScrollOffset = Vector2( , 0.f);
};

