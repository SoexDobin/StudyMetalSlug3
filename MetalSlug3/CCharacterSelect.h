#pragma once
#include "CScene.h"

class CAnimation;

class CCharacterSelect : public CScene
{
public:
	CCharacterSelect();
	~CCharacterSelect() override;

public:
	void Initialize() override;

	pair<bool, SCENETAG> Update() override;

	void LateUpdate() override;

	void Render(HDC _hDC) override;

	void Release() override;

	void Sequence() override;

public:

	void LoadSelectionBmp();

private:
	bool	m_iHasSelect;
	int		m_iSelect;
	float	m_fAnimDelta;
};

