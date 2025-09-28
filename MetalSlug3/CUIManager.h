#pragma once
#include "CSingleTon.h"
#include "CUIObject.h"

class CUIManager : public CSingleTon<CUIManager>
{
	friend class CSingleTon;
private:
	CUIManager();
	~CUIManager() override;

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

	void		AddUIObject(CUIObject* _pUIObj);
private:
	list<CUIObject*>		m_UIObjectList;
};