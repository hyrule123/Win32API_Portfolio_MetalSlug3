#include "ScoreManager.h"

DEFINITION_SINGLETON(CScoreManager);

CScoreManager::CScoreManager() :
	m_Score()
{

}

CScoreManager::~CScoreManager()
{

}

void CScoreManager::Init()
{

}

void CScoreManager::AddScore(int Score)
{
	m_Score += Score;
}

int CScoreManager::GetScore() const
{
	return m_Score;
}

void CScoreManager::InitScore()
{
	m_Score = 0;
}
