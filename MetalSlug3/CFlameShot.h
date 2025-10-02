#pragma once
#include "CItem.h"
class CFlameShot :
    public CItem
{
public:
    CFlameShot();
    ~CFlameShot() override;
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) override;
};

