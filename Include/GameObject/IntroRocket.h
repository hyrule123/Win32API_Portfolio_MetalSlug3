#pragma once
#include "GameObject.h"

//모든 로켓의 공통 클래스.
class CIntroRocket :
    public CGameObject
{
	friend class CScene;
protected:
	CIntroRocket();
	CIntroRocket(const CIntroRocket& Obj);
	virtual ~CIntroRocket();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);

protected:
	float m_OffsetY;
	float m_TriggerYPos;
	bool m_Separtion;
	bool m_CapSeparateSFXNotified;


	void SideThrustStartEndFunc();
	void SideThrustMaxEndFunc();
	void SeparateSFXNotify();
public:
	bool GetCapSeparateSFXNotified() const;
	void Separate();
	void SetOffsetY(float OffsetY);
	void NoseSeparation(float DirX);
};

