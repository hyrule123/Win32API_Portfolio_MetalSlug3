#pragma once
#include "Enemy.h"

enum class EUFORoutine : UINT8
{
	Moveto,
	Wait,
	MovetoPlayer,
	AttackDelay,
	Attack,
	KeyCheck,//다음 페이즈로 넘어가기 위한 키 몬스터일 경우 다시 처음으로 돌아간다
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

