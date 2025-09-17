#include "pch.h"
#include "CObjectManager.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	Release();
}

void CObjectManager::AddGameObject(CGameObject* _pObj, OBJECT_TYPE _eType)
{
	m_vecObjectList[_eType].push_back(_pObj);
}

void CObjectManager::Initialize()
{
	m_vecObjectList.clear();
	m_vecObjectList.resize(OBJECT_END);
}

void CObjectManager::Update()
{
	for (auto& list : m_vecObjectList)
	{
		for (auto it = list.begin(); it != list.end(); )
		{
			bool isDestroy = (*it)->Update();

			if (isDestroy)
			{
				SafeDelete<CGameObject*>(*it);
				it = list.erase(it);
			}
			else
				++it;
		}
	}
}

void CObjectManager::LateUpdate()
{
	for (auto& list : m_vecObjectList)
	{
		for_each(list.begin(), list.end(), [](CGameObject* _p)
			{
				_p->LateUpdate();
			}
		);
	}
}

void CObjectManager::Render(HDC _hDC)
{
	for (auto& list : m_vecObjectList)
	{
		for_each(list.begin(), list.end(), [&](CGameObject* _p)
			{
				_p->Render(_hDC);
			}
		);
	}
}

void CObjectManager::Release()
{
	for (auto& list : m_vecObjectList)
	{
		for_each(list.begin(), list.end(), [](CGameObject* _p)
			{
				if (_p) {
					delete _p;
					_p = nullptr;
				}
			});
		list.clear();
	}
	m_vecObjectList.shrink_to_fit();
}