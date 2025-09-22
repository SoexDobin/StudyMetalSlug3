#pragma once
#include "CScene.h"

class CMission4Pyramid final : public CScene
{
public:
	CMission4Pyramid();
	~CMission4Pyramid() override;

public:
	void					Initialize()		override;
	pair<bool, SCENETAG>	Update()			override;
	void					LateUpdate()		override;
	void					Render(HDC _hDC)	override;
	void					Release()			override;

private:
	void					RenderPyramidLandscape(HDC _hDC);

	void					CreatePlatform();
	void					LoadBmpPyramidLandscape();
	void					LoadBmpEnemy();

private:

	vector<const TCHAR*>	m_vecPyramidSecondFloorKey;
};

