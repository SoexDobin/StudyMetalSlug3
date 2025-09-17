#pragma once
#include "CScene.h"
class CArmoryScene : public CScene
{
public:
	CArmoryScene();
	virtual ~CArmoryScene() override;

public:
	virtual void						Initialize()		override;
	virtual pair<bool, SCENETAG>		Update()			override;
	virtual void						LateUpdate()		override;
	virtual void						Render(HDC _hDC)	override;
	virtual void						Release()			override;

private:
	void				CheatKeyInput();
};

