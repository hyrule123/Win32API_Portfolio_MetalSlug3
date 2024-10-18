#pragma once
#include "IntroRocket.h"


class CIntroRocketLowest :
	public CIntroRocket
{
	friend class CScene;
protected:
	CIntroRocketLowest();
	CIntroRocketLowest(const CIntroRocketLowest& Obj);
	virtual ~CIntroRocketLowest();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);

protected:
	class CRocketThrust* m_Thrust;

};

