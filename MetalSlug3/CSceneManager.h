#pragma once
#include "CSingleTon.h"
#include "Define.h"

class CGameObject;
class CScene;

class CSceneManager final : public CSingleTon<CSceneManager>
{
	friend class CSingleTon;
private:
	CSceneManager();
	~CSceneManager() override;

public:
	void			Initialize();
	void			Update();
	void			LateUpdate();
	void			Render(HDC _hDC);
	void			Release();
	
	void			NextSceneSequence();
	CScene*			ChangeScene(SCENETAG _eTag);

public:
	CScene*			GetCurScene() 			{ return m_pScene; };
	
private:
	CScene*			m_pScene;
	CScene*			m_pPrevScene;
	SCENETAG		m_eCurScene;
	

// [!]NOTICE	Don't Destroy On SCENE
private:
	CGameObject*	ref_pPlayer;
};

