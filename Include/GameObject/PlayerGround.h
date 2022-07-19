#pragma once
#include "Player.h"
class CPlayerGround :
    public CPlayer
{
	friend class CScene;

protected:
	bool m_isAttacking;

	class CColliderBox* m_Collider;



protected:
	CPlayerGround();
	CPlayerGround(const CPlayerGround& Obj);
	virtual ~CPlayerGround();

public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

	virtual int InflictDamage(int Damage);



private:
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();


	void GunFire();
	void FireNotify();
	void AttackEnd();
};

