#pragma once
#include "Projectile.h"

enum class EProjectileEnemy
{
	UFOLaser,
	MiniUFOAttack,
	DaiManjiAttack,
	End
};

class CProjectileEnemy :
	public CProjectile
{
	friend class CScene;
protected:
	CProjectileEnemy();
	CProjectileEnemy(const CProjectileEnemy& Obj);
	virtual ~CProjectileEnemy();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual void SetEssential(EProjectileEnemy Type, const Vector2& Dir, const Vector2& Pos);


private:
	EProjectileEnemy m_ProjectileType;
	std::string m_vecProjectileName[(int)EProjectileEnemy::End];
	class CColliderBox* m_Coll;
	class CColliderCircle* m_CollCircle;
	virtual void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	
};

