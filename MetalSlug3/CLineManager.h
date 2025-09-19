#pragma once
#include "CLine.h"
#include "CSingleTon.h"

class CLineManager final : public CSingleTon<CLineManager>
{
	friend class CSingleTon;
public:
	CLineManager();
	~CLineManager() override;
public:
	bool	CollisionLine(float fX, float* pY);
	void	AddLine(Vector2 _vFrom, Vector2 _vTo);
public:
	void		Initialize();
	int			Update();
	void		LateUpdate();
	void		Render(HDC hDC);
	void		Release();

public:
	void		SaveData();
	void		LoadData();

private:
	list<CLine*>		m_LineList;
	LINE				m_tLinePoint;
};

