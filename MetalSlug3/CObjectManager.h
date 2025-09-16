#pragma once
#include "CGameObject.h"

class CObjectManager
{
private:
	CObjectManager();
	~CObjectManager();
	CObjectManager(const CObjectManager& _rhs) = delete;
	CObjectManager& operator=(const CObjectManager& _rhs) = delete;

public:
	list<CGameObject*>& GetGameObjectList(OBJECT_TYPE _eID) 
	{ 
		return m_vecObjectList[_eID];
	}

public:
	static CObjectManager&		GetInstance();
	static void					DeleteInstance();

	void						Initialize();
	void						Update();
	void						LateUpdate();
	void						Render(HDC _hDC);
	void						Release();

private:
	static CObjectManager*		m_pInstance;
	vector<list<CGameObject*>>	m_vecObjectList;
};

