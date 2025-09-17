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

private:
	SCENETAG		ChangeScene(SCENETAG _eTag);

private:
	CScene*			m_pScene;
	SCENETAG		m_eCurScene;
	SCENETAG		m_ePrevScene;

// [!]NOTICE	Don't Destroy On SCENE
private:
	CGameObject*	ref_pPlayer;
};

