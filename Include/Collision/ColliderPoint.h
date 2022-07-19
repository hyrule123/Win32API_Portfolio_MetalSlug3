#pragma once
#include "Collider.h"
class CColliderPoint :
    public CCollider
{
    friend class CGameObject;
    friend class CCollisionManager;
    friend class CInput;

protected:
    CColliderPoint();
    CColliderPoint(const CColliderPoint& col);
    virtual ~CColliderPoint();

protected:
    Vector2 m_Info;


public:
    const Vector2& GetInfo() const;
    void SetPoint(const Vector2& Point);
    virtual bool Collision(CCollider* Dest);


protected:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
};

