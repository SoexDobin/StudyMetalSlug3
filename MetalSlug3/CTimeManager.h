#pragma once

class CTimeManager
{
private:
	CTimeManager();
	~CTimeManager();
	CTimeManager(const CTimeManager& _rhs) = delete;
	CTimeManager& operator=(const CTimeManager& _rhs) = delete;

public:
	float					GetDeltaTime()		{ return m_fDeltaTime; }
	void					Initialize();
	void					Update();

public:
	static CTimeManager&	GetInstance();
	static void				DeleteInstance();

private:
	static CTimeManager*		m_pInstance;

	float						m_fDeltaTime;
	LARGE_INTEGER				m_liCurCount;
	LARGE_INTEGER				m_liPrevCount;
	LARGE_INTEGER				m_liFrequency;
};

