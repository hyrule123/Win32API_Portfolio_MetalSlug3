#pragma once
#include "../Ref.h"

#include "../GameInfo.h"


class CCollider :
    public CRef
{
    friend class CGameObject;
    friend class CCollisionManager;

protected:
    CCollider();
    CCollider(const CCollider& col);
    virtual ~CCollider() = 0;

public:
    virtual bool Init();

protected:
    class CGameObject*  m_OwnerObj;
    class CScene*       m_Scene;
    Vector2             m_Offset;


    bool m_CheckMouseCollision;
    //���콺 Ŭ�� Y������ ���� �浹ü ���ϴ��� ��ǥ���� �����صδ� ����
    float m_Bottom;

public:
    float GetBottom() const;
    void SetCheckMouseCollision(bool On);
    bool GetCheckMouseCollision()   const;
    

protected:

    bool m_Start;

    //��ǥ�� ���ϴ� ������ ����
    bool m_isFixed;
    Vector2 m_FixedPos;

    //�浹�� �Ͼ�� ��� �ش� �浹���� �����ϱ� ���� ����
    Vector2             m_HitPoint;

    EColliderType       m_ColliderType;

    //�ڽ��� �Ҽӵ� ä�� ����
    ECollisionChannel   m_Profile;

    //�浹 ����/���� ȣ���� �Լ�
    std::function<void(CCollider*, CCollider*)> m_CollisionBegin;
    std::function<void(CCollider*, CCollider*)> m_CollisionEnd;

    //�� �����ӿ��� �浹 ���̾��� �浹ü���� ���⿡ �����س��´�.
    std::list<CSharedPtr<CCollider>> m_listCollision;
    
    //���콺 ����
    bool m_isMouseColliding;
    std::function<void(const Vector2&, CCollider*)> m_CollisionBeginMouse;
    std::function<void(const Vector2&, CCollider*)> m_CollisionEndMouse;
public:
    std::list<CSharedPtr<CCollider>>* GetCollisionList();
    bool CheckColliding(class CGameObject* Obj);
    bool CheckCollidingAny();
    bool CheckMouseColliding() const;
    void SetMouseColliding(bool isMouseColliding);
    virtual bool CollisionPoint(const Vector2& Point);
    void CallCollisionBeginMouse(const Vector2& Point);
    void CallCollisionEndMouse(const Vector2& Point);
    template <typename T>
    void SetCollisionBeginMouse(T* Obj, void (T::* Func)(const Vector2&, CCollider*))
    {
        m_CollisionBeginMouse = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void SetCollisionEndMouse(T* Obj, void (T::* Func)(const Vector2&, CCollider*))
    {
        m_CollisionEndMouse = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }

public:
    //��ǥ ���� ���� �� ��ǥ ��ȯ
    virtual void SetFixedPos(const Vector2& Pos) = 0;
    virtual void SetOffset(const Vector2& Offset);
    virtual void SetOffset(float _x, float _y);


    const Vector2& GetPos()   const;

    const Vector2& GetHitPoint()    const;

    //�浹���� �� �Լ��� ȣ�����ִ� �޼ҵ�
    void CallCollisionBegin(CCollider* Dest);
    void CallCollisionEnd(CCollider* Dest);

    class CGameObject*  GetOwnerObj()  const;
    class CScene* GetOwnerScene()  const;
    EColliderType GetColliderType() const;

    const ECollisionChannel GetCollisionProfile() const;
    void SetCollisionProfile(ECollisionChannel Profile);

    void AddCollisionList(CCollider* col);
    bool CheckCollisionList(CCollider* col);
    void DeleteCollisionList(CCollider* col);
    void ClearCollisionList();

    //�浹 ���� �Ǵ� �浹 ���� �� ������ �Լ��� ����Ѵ�.
    template <typename T>
    void SetCollisionBeginFunc(T* Obj, void(T::* Func)(CCollider*, CCollider*))
    {
        //�Լ��� bind�ؼ� �־� �ش�.
        m_CollisionBegin = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
    template <typename T>
    void SetCollisionEndFunc(T* Obj, void(T::* Func)(CCollider*, CCollider*))
    {
        //�Լ��� bind�ؼ� �־� �ش�.
        m_CollisionEnd = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }


public:
    virtual void Start();
    virtual void Update(float DeltaTime) = 0;
    virtual void PostUpdate(float DeltaTime) = 0;
    virtual void Render(HDC hDC, float DeltaTime) = 0;

    //�浹ü ��� ���� �������Ͽ� ���
    virtual bool Collision(CCollider* Dest) = 0;
};


