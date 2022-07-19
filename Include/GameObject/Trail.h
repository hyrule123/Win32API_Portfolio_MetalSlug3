#pragma once
#include "Effect.h"


//잔해가 날아가면서 폭발 이펙트를 남김.
class CTrail :
    public CEffect
{
	friend class CScene;

protected:
	CTrail();
	CTrail(const CTrail& Obj);
	virtual ~CTrail();

public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);

protected:
	float m_Frequency;
	float m_FrequencyMax;
public://이미지 생성 주기를 설정하지 않으면 작동 불가.
	void SetFrequency(float Frequency);

protected:
	float m_Force;
public:
	void Trajectory(float Force);	//궤적을 그리며 폭파 이펙트
	void Straight(float Force);	//직선 방향으로 폭파 이펙트

public:
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

