#pragma once
#include "Scene.h"
class CGameOver :
	public CScene
{
	friend class CSceneManager;

protected:
	CGameOver();
	virtual ~CGameOver();
	class CWindowGameOver* m_Window;

public:
	virtual bool Init();
	virtual void SetEssential(bool DeathGameOver);


};

