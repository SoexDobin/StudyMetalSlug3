#pragma once
#include "CSingleTon.h"

class CScoreManager : public CSingleTon<CScoreManager>
{
	friend class CSingleTon;
private:
	CScoreManager();
	~CScoreManager() override;

public:
	const int&	GetScore() const				{ return m_iScore; }
	void		ACCScore(const int& _iScore)	{ m_iScore += _iScore; }
	void		ForceScore(const int& _iScore)	{ m_iScore = _iScore; }
	void		Initialize();

private:
	int		m_iScore;
};