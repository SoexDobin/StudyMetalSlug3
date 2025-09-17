#pragma once
#include "CSingleTon.h"
#include "Define.h"

class CKeyManager final : public CSingleTon<CKeyManager>
{
	friend class CSingleTon;
private:
	CKeyManager();
	~CKeyManager() override;

public:
	bool		KeyPressing(int iKey);
	bool		KeyUp(int iKey);	
	bool		KeyDown(int iKey);	
	void		KeyUpdate();

private:
	bool					m_bKeyState[VK_MAX];
};

