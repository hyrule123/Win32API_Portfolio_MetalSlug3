#pragma once
#include "Enemy.h"

enum class EUFORoutine : UINT8
{
	Moveto,
	Wait,
	MovetoPlayer,
	AttackDelay,
	Attack,
	KeyCheck,//���� ������� �Ѿ�� ���� Ű ������ ��� �ٽ� ó������ ���ư���
	Flee


};

class CUFO :
    public CEnemy
{
	friend class CScene;

protected:
	CUFO();
	CUFO(const CUFO& Obj);
	virtual ~CUFO();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);
	virtual void SetEssential(bool isKey);

protected:


	//void CollBegin(class CCollider* Src, class CCollider* Dest);
private:

	CooltimeChecker m_Timer;
	virtual void Routine(float DeltaTime);
	void AttackFrame7Notify();

};

