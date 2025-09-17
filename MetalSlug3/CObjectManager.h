#pragma once
#include "CSingleTon.h"
#include "CGameObject.h"

class CObjectManager final : public CSingleTon<CObjectManager>
{
	friend class CSingleTon;
private:
	CObjectManager();
	~CObjectManager() override;

public:
	list<CGameObject*>& GetGameObjectList(OBJECT_TYPE _eID) 
	{ 
		return m_vecObjectList[_eID];
	}

public:
	void						AddGameObject(CGameObject* _pObj, OBJECT_TYPE _eType);

public:
	void						Initialize();
	void						Update();
	void						LateUpdate();
	void						Render(HDC _hDC);
	void						Release();

private:
	vector<list<CGameObject*>>	m_vecObjectList;
};

