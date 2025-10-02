#pragma once
#include "Define.h"
#include "CGameObject.h"

class CScene 
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void					Initialize()		PURE;
	virtual pair<bool, SCENETAG>	Update()			PURE;
	virtual void					LateUpdate()		PURE;
	virtual void					Render(HDC hDC)		PURE;
	virtual void					Release()			PURE;
	virtual void					Sequence()			PURE;

public:
	Vector2				GetSpawnPoint()	const				{ return m_vSpawnPoint; }
	void				SetSpawnPoint(const Vector2& _vPt)	{ m_vSpawnPoint = _vPt; }
	bool				GetSceneState() const				{ return m_bDestroyScene; }
	void				DestroyScene() 						{ m_bDestroyScene = SCENE_DESTROY; }
	vector<CGameObject*>&		GetItems()					{ return m_vecItems; }

protected:
	bool				m_bDestroyScene;
	Vector2				m_vSpawnPoint;
	vector<CGameObject*>		m_vecItems;
};