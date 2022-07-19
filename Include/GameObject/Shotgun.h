#pragma once
#include "Projectile.h"

class CShotgun :
    public CProjectile
{
    friend class CScene;


protected:
    CShotgun();
    CShotgun(const CShotgun& Obj);
    virtual ~CShotgun();

    class CPlayer* m_Player;
    Vector2 m_Offset;

public:
    bool LoadResource();
    bool Init(CGameObject* Obj = nullptr);
    void PostUpdate(float DeltaTime);

    void SetEssential(class CPlayer* Player, const Vector2& Offset);

    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
    void ColliderDelete();
    void AnimEndDelete();
};

