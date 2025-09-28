#pragma once
#include "CGameObject.h"

class CAnimation;

class CEriSpawn : public CGameObject
{
public:
	CEriSpawn();
	~CEriSpawn() override;

public:
	void		Initialize() override;
	int			Update() override;
	void		LateUpdate() override;
	void		Render(HDC _hDC) override;
	void		Release() override;
	void		OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;

public:
	CAnimation* GetSpawnAnimation() { return m_pSpawnAnim; }

private:
	CAnimation* m_pSpawnAnim;
};

