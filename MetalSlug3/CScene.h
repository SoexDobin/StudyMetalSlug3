#pragma once
#include "Define.h"

class CScene abstract
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

public:
	bool				GetSceneState()		{ return m_bDestroyScene; }
	void				DestroyScene()		{ m_bDestroyScene = SCENE_DESTROY; }

protected:
	bool				m_bDestroyScene;
};