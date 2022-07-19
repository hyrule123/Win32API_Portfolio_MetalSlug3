#pragma once
#include "Explosion.h"



//실제로 스플래시 데미지를 체크함
//SetEssentials를 통해 원하는 크기 지정.
class CDamageExplosion :
    public CExplosion
{
	friend class CScene;

protected:
	CDamageExplosion();
	CDamageExplosion(const CDamageExplosion& Obj);
	virtual ~CDamageExplosion();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);

private:
	int m_Damage;
	class CColliderCircle* m_Coll;

public:
	void SetEssential(EExplosionSize ExplosionSize, const Vector2& Pos, int Damage);
	

private:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	void Frame2Notify();
};

