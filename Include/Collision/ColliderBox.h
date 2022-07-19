#pragma once
#include "Collider.h"

#include "../GameInfo.h"

class CColliderBox :
    public CCollider
{
    friend class CGameObject;
    friend class CCollisionManager;

protected:
    CColliderBox();
    CColliderBox(const CColliderBox& col);
    virtual ~CColliderBox();

protected:
    BoxInfo m_Info;
    Vector2 m_Size;
    Vector2 m_Pivot;


    //float m_Width;
    //float m_Height;

public:
    BoxInfo GetInfo() const;
    
    virtual void SetFixedPos(const Vector2& Pos);
    virtual void SetOffset(const Vector2& Offset);
    virtual void SetOffset(float _x, float _y);

    //void SetExtent(float Width, float Height);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionPoint(const Vector2& Point);

    void SetSize(const Vector2& Size);
    void SetSize(float SizeX, float SizeY);
    void SetPivot(const Vector2& Pivot);
    void SetPivot(float PivotX, float PivotY);
    

    

protected:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
};

