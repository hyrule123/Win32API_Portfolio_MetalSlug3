#pragma once
#include "Enemy.h"

enum class EMiniUFORoutine
{
	Delay,
	Moveto,
	Wait,
	Attack
};

enum class EMiniUFOLayerInfo
{
	UFOBody,
	AttackSFX
};

class CMiniUFO :
    public CEnemy
{
	friend class CScene;

protected:
	CMiniUFO();
	CMiniUFO(const CMiniUFO& Obj);
	virtual ~CMiniUFO();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual int InflictDamage(int Damage);
	virtual void SetEssential(float DestPosX);

protected:
	//�¾��� ���� ��ȣ�� ����(��Ʈ����Ʈ ���� �� ������� ���ư��� �뵵)
	//�� �¾��� ��� -1

	//void CollBegin(class CCollider* Src, class CCollider* Dest);
private:
	class CMainScene* m_MainScene;
	CooltimeChecker m_Timer;
	float m_HitTimer;
	virtual void Routine(float DeltaTime);
	void AttackEndFunc();
	void MiniUFODeathEndFunc();

};

