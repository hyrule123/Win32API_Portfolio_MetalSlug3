#pragma once
#include "Projectile.h"
class CPlayerHMG :
    public CProjectile
{
    friend class CScene;

protected:
    CPlayerHMG();
    CPlayerHMG(const CPlayerHMG& Obj);
    virtual ~CPlayerHMG();

public:
    virtual bool Init(CGameObject* Obj = nullptr);
    virtual bool LoadResource();
    virtual void SetEssential(Vector2 Dir, Vector2 Pos);



private:
    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
};

