#include "pch.h"
#include "Define.h"
#include "CUIManager.h"

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
	Release();
}

void CUIManager::Initialize()
{
	AddFontResourceEx(L"../Resource/Font/metal-slug-colour.colr.ttf", FR_PRIVATE, 0);
}

void CUIManager::Update()
{
	for (auto it = m_UIObjectList.begin(); it != m_UIObjectList.end(); )
	{
		bool isDestroy = (*it)->Update();

		if (isDestroy)
		{
			SafeDelete<CUIObject*>(*it);
			it = m_UIObjectList.erase(it);
		}
		else
			++it;
	}
}

void CUIManager::LateUpdate()
{
	for_each(m_UIObjectList.begin(), m_UIObjectList.end(), [](CUIObject* _p)
		{
			_p->LateUpdate();
		}
	);
}

void CUIManager::Render(HDC _hDC)
{
	for_each(m_UIObjectList.begin(), m_UIObjectList.end(), [&](CUIObject* _p)
		{
			_p->Render(_hDC);
		}
	);
}

void CUIManager::Release()
{
	for_each(m_UIObjectList.begin(), m_UIObjectList.end(), [](CUIObject* _p)
		{
			if (_p) {
				delete _p;
				_p = nullptr;
			}
		});
	m_UIObjectList.clear();
	RemoveFontResourceEx(L"../Resource/Font/metal-slug-colour.ttf", FR_PRIVATE, 0);
}

void CUIManager::AddUIObject(CUIObject* _pObj)
{
	m_UIObjectList.push_back(_pObj);
}
