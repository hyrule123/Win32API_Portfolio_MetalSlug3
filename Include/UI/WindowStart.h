#pragma once
#include "WidgetWindow.h"
class CWindowStart :
	public CWidgetWindow
{
	friend class CScene;

protected:
	CWindowStart();
	virtual ~CWindowStart();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	bool m_PlayGameDelayOn;
	float m_Delay;

private:
	void PlayGameCallback();
	void OpenEditScene();
	void PlayGame();
};

