#pragma once
#include "Define.h"

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
	void				SetSceneState(bool _bSet) 		{ m_bDestroyScene = _bSet; }
	bool				GetSceneState() const			{ return m_bDestroyScene; }
	void				DestroyScene() 					{ m_bDestroyScene = SCENE_DESTROY; }

protected:
	bool				m_bDestroyScene;
};