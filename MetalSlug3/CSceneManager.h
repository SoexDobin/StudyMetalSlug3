#pragma once
#include "CSingleTon.h"
#include "Define.h"

class CUIObject;
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
	SCENETAG		GetCurSceneType()		{ return m_eCurScene; };
	CGameObject*	GetCurPlayer()			{ return m_pPlayer; }
	
	void			DisableSceneUI();
	void			EnableSceneUI();
	

private:
	CScene*			m_pScene;
	CScene*			m_pPrevScene;
	SCENETAG		m_eCurScene;
	

// [!]NOTICE	Don't Destroy On SCENE
private:
	CGameObject*	m_pPlayer;

	CUIObject*		m_pTimerUI;
	CUIObject*		m_pScoreUI;
	CUIObject*		m_pMultiHitUI;
	CUIObject*		m_pArmBoxUI;

	// ÄÚÀÎ, ¸ñ¼û
	// 
};

