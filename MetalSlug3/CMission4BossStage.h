#pragma once
#include "CScene.h"

class CMission4BossStage final : public CScene
{
public: 
	CMission4BossStage();
	~CMission4BossStage() override;

public:
	void						Initialize()		override;
	pair<bool, SCENETAG>		Update()			override;
	void						LateUpdate()		override;
	void						Render(HDC hDC)		override;
	void						Release()			override;
	void						Sequence()			override;

private:
	void					RenderBossLandscape(HDC _hDC);
	void					RenderFrontLandscape(HDC _hDC);

	void					CreatePlatform();
	void					CreatetTriggerArea();
	void					LoadBmpPyramidLandscape();
	void					LoadBmpEnemy();
private:

};

