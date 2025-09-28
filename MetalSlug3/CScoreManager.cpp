#include "pch.h"
#include "CScoreManager.h"

CScoreManager::CScoreManager()
	: m_iScore(0)
{
}

CScoreManager::~CScoreManager()
{
}

void CScoreManager::Initialize()
{
	m_iScore = 0;
}