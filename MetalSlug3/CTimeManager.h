#pragma once
#include "CSingleTon.h"

class CTimeManager final : public CSingleTon<CTimeManager>
{
	friend class CSingleTon;
private:
	CTimeManager();
	~CTimeManager() override;
public:
	float					GetDeltaTime()		{ return m_fDeltaTime; }
	void					Initialize();
	void					Update();

private:
	float						m_fDeltaTime;
	LARGE_INTEGER				m_liCurCount;
	LARGE_INTEGER				m_liPrevCount;
	LARGE_INTEGER				m_liFrequency;
};

