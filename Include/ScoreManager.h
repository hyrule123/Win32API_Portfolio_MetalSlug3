#pragma once

#include "GameInfo.h"
#include "Singleton.h"

class CScoreManager
{
public:
	void Init();


private:
	int m_Score;

public:
	void AddScore(int Score);
	int GetScore() const;
	void InitScore();

	DECLARE_SINGLETON(CScoreManager)
};

