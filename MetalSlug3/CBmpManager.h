#pragma once

class CBmpManager
{
private:
	CBmpManager();
	~CBmpManager();
	CBmpManager(const CBmpManager& _rhs) = delete;
	CBmpManager& operator=(CBmpManager& _rhs) = delete;

public:
	static CBmpManager&		GetInstance();
	static void				DeleteInstance();

public:


private:
	static CBmpManager*		m_pInstance;
};

