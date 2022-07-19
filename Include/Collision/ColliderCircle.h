#pragma once
#include "Collider.h"

#include "../GameInfo.h"

class CColliderCircle :
    public CCollider
{
    friend class CGameObject;
    friend class CCollisionManager;

protected:
    CColliderCircle();
    CColliderCircle(const CColliderCircle& col);
    virtual ~CColliderCircle();

protected:
    CircleInfo m_Info;


public:
    virtual void SetFixedPos(const Vector2& Pos);
    virtual void SetOffset(const Vector2& Offset);
    virtual void SetOffset(float _x, float _y);

    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionPoint(const Vector2& Point);

    CircleInfo GetInfo() const;
    void SetRadius(float Rad);



protected:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);


};

