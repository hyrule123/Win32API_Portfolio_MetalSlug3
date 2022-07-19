#pragma once
#include "Effect.h"


//����: ����ٳ�� �� ��� ���� ������Ʈ�� �ְ� ����
//�Ϲ����� ��� �׳� ����
class CAstroSlugSFX :
    public CEffect
{
	friend class CScene;
protected:
	CAstroSlugSFX();
	CAstroSlugSFX(const CAstroSlugSFX& Obj);
	virtual ~CAstroSlugSFX();
public:
	virtual bool LoadResource();
	virtual void Start();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	bool SetSFX(const std::string& Name);


public:
	//�ƽ�Ʈ�� �����׿��� �����ϴ� �Լ�. ���� �ν�Ʈ ������ �������
	void TailBoost1ReverseSet();

private:
	bool m_isTailSFX;
	Vector2 m_TailSFXDir;
	void Booster1EndFunc();
	void TailLeftEnd();
	void TailRightEnd();
	void TailStraightEnd();
	void TailIdleEnd();
	void CreateTailSFX();
	void TailBoost1ReverseEnd();



};

