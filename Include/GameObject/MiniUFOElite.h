#pragma once
#include "MiniUFO.h"
class CMiniUFOElite :
    public CMiniUFO
{

	friend class CScene;

protected:
	CMiniUFOElite();
	CMiniUFOElite(const CMiniUFOElite& Obj);
	virtual ~CMiniUFOElite();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void SetEssential(float DestPosX);
	virtual int InflictDamage(int Damage);

protected:
	//맞았을 때의 번호를 저장(히트이펙트 생성 후 원래대로 돌아가는 용도)
	//안 맞았을 경우 -1

	//void CollBegin(class CCollider* Src, class CCollider* Dest);
private:
	class CMainScene* m_MainScene;
	CooltimeChecker m_Timer;
	float m_HitTimer;
	INT8 m_SerialFire;	//연속 3발 발사
	bool m_InAction;
	virtual void Routine(float DeltaTime);
	void AttackEndFunc();
	void MiniUFOEliteDeathEndFunc();

};

