#pragma once
#include "Projectile.h"
class CSplash :
    public CProjectile
{
    friend class CScene;
protected:
    CSplash();
    CSplash(const CSplash& Obj);
    virtual ~CSplash();

protected:
    class CColliderCircle* m_Coll;
    bool m_SetActive;

public:
    virtual bool Init(CGameObject* Obj = nullptr);
    virtual void PostUpdate(float DeltaTime);

    void SetEssential(const Vector2& Pos, const Vector2& Offset, float Radius, ECollisionChannel Channel, int Damage);

    virtual void CollisionBegin(class CCollider* Src, class CCollider* Dest);
};

