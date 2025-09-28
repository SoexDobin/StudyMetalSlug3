#pragma once
#include "CGameObject.h"

class CExplodeArea : public CGameObject
{
public:
	CExplodeArea();
	~CExplodeArea() override;

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;
	virtual void OnCollision(CGameObject* _pCol, Vector2 _vColSize, COLLISION_COL_FLAG _eFlag) PURE;
public:
	OBJECT_TYPE GetDamageFlag() { return m_eDamageFlag; }
	void SetDamageFlag(OBJECT_TYPE _eFlag) { m_eDamageFlag = _eFlag; }
	int GetDamage() { return m_iDamage; }
	void SetDamage(const int& _iDamage) { m_iDamage = _iDamage; }

protected:
	int			m_iDamage;
	OBJECT_TYPE m_eDamageFlag;
};

