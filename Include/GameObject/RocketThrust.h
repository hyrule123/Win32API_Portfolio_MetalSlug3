#pragma once
#include "IntroRocket.h"

class CRocketThrust :
	public CIntroRocket
{
	friend class CScene;
protected:
	CRocketThrust();
	CRocketThrust(const CRocketThrust& Obj);
	virtual ~CRocketThrust();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	void ThrustIgnite();
};

