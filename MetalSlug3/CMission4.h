#pragma once
#include "CScene.h"

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
	void					RenderLandscape(HDC _hDC);
	void					RenderFrontLandscape(HDC _hDC);
	void					LoadImageLandscape();
	

private:
	float					m_fDelta;
	list<const TCHAR*>		m_DessertBackgroundKeyList;
	vector<const TCHAR*>	m_vecDessertFrontgroundKey;
};

