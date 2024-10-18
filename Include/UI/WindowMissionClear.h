#pragma once
#include "WidgetWindow.h"

enum MissionClearFlags : UINT8
{
	FlickerStart = 1 << 0,
	ScatterStart = 1 << 1
};

class CWindowMissionClear :
	public CWidgetWindow
{
	friend class CScene;

protected:
	CWindowMissionClear();
	CWindowMissionClear(const CWindowMissionClear& widget);
	virtual ~CWindowMissionClear();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

private:
	std::vector<class CMovingTextImage*> m_vecText;
	CooltimeChecker m_Timer;
	UINT8 m_BoolFlag;
	float m_MaxSpeed;


public:
	void RenderMissionComplete();
};

