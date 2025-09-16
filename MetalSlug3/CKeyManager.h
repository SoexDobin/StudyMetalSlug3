#pragma once
#include "Define.h"

class CKeyManager
{
private:
	CKeyManager();
	~CKeyManager();

public:
	bool		KeyPressing(int iKey);
	bool		KeyUp(int iKey);	
	bool		KeyDown(int iKey);	
	void		KeyUpdate();

public:
	static CKeyManager&			GetInstance();
	static void					DeleteInstance();

private:
	static CKeyManager*		m_pInstance;
	bool					m_bKeyState[VK_MAX];

};

