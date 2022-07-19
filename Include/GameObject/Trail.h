#pragma once
#include "Effect.h"


//���ذ� ���ư��鼭 ���� ����Ʈ�� ����.
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
public://�̹��� ���� �ֱ⸦ �������� ������ �۵� �Ұ�.
	void SetFrequency(float Frequency);

protected:
	float m_Force;
public:
	void Trajectory(float Force);	//������ �׸��� ���� ����Ʈ
	void Straight(float Force);	//���� �������� ���� ����Ʈ

public:
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

