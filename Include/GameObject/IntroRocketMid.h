#pragma once
#include "IntroRocket.h"
class CIntroRocketMid :
    public CIntroRocket
{
	friend class CScene;
protected:
	CIntroRocketMid();
	CIntroRocketMid(const CIntroRocketMid& Obj);
	virtual ~CIntroRocketMid();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);

protected:
	class CRocketThrust* m_Thrust;

};

