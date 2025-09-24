#pragma once
#include "CScene.h"

class CMission4Pyramid final : public CScene
{
public:
	CMission4Pyramid();
	~CMission4Pyramid() override;

public:
	void								Initialize()		override;
	pair<bool, SCENETAG>				Update()			override;
	void								LateUpdate()		override;
	void								Render(HDC _hDC)	override;
	void								Release()			override;
	void								Sequence()			override;
private:
	void					RenderPyramidLandscape(HDC _hDC);
	void					RenderFrontLandscape(HDC _hDC);

	void					CreatePlatform();
	void					CreatetTriggerArea();
	void					LoadBmpPyramidLandscape();
	void					LoadBmpEnemy();

private:
	int						m_iScrollLockIdx;
	int						m_iWaterFallIdx;
	float					m_fWaterFallDelta;

	vector<const TCHAR*>	m_vecWaterFallKey;
	vector<const TCHAR*>	m_vecPyramidSecondFloorKey;
	vector<const TCHAR*>	m_vecPyramidTopFloorKey;

private:
	Vector2		n_vPyramidMinLockPoints[8] =
	{
		Vector2(0.f, 720.f),
		Vector2(0.f, 324.f + 720.f),
		Vector2(0.f, 694.f + 720.f),
		Vector2(0.f, 700.f + WINCY * 2.f),
		Vector2(0.f, 700.f + WINCY * 2.f),
		Vector2(0.f, 700.f + WINCY * 3.f),
		Vector2(0.f, 700.f + WINCY * 3.f),
		Vector2(0.f, 748.f + WINCY * 3.f),
	};
	Vector2		n_vPyramidMaxLockPoints[8] =
	{
		Vector2(1030.f,			0.f),
		Vector2(360.f + 768.f,	0.f),
		Vector2(3430.f,			0.f),
		Vector2(3430.f,			0.f),
		Vector2(-(3430.f - 480.f), 0.f),
		Vector2(3430.f - 480.f, 0.f),
		Vector2(3430.f * 3.f,	0.f),
		Vector2(9500.f,	0.f),
	};
};

