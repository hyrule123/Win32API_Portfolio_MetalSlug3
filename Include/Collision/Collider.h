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
    //마우스 클릭 Y소팅을 위해 충돌체 최하단의 좌표값을 저장해두는 변수
    float m_Bottom;

public:
    float GetBottom() const;
    void SetCheckMouseCollision(bool On);
    bool GetCheckMouseCollision()   const;
    

protected:

    bool m_Start;

    //좌표를 원하는 곳으로 지정
    bool m_isFixed;
    Vector2 m_FixedPos;

    //충돌일 일어났을 경우 해당 충돌점을 저장하기 위한 변수
    Vector2             m_HitPoint;

    EColliderType       m_ColliderType;

    //자신이 소속된 채널 저장
    ECollisionChannel   m_Profile;

    //충돌 시작/끝에 호출할 함수
    std::function<void(CCollider*, CCollider*)> m_CollisionBegin;
    std::function<void(CCollider*, CCollider*)> m_CollisionEnd;

    //전 프레임에서 충돌 중이었던 충돌체들을 여기에 저장해놓는다.
    std::list<CSharedPtr<CCollider>> m_listCollision;
    
    //마우스 관련
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
    //좌표 수동 설정 및 좌표 반환
    virtual void SetFixedPos(const Vector2& Pos) = 0;
    virtual void SetOffset(const Vector2& Offset);
    virtual void SetOffset(float _x, float _y);


    const Vector2& GetPos()   const;

    const Vector2& GetHitPoint()    const;

    //충돌했을 때 함수를 호출해주는 메소드
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

    //충돌 시작 또는 충돌 종료 때 동작할 함수를 등록한다.
    template <typename T>
    void SetCollisionBeginFunc(T* Obj, void(T::* Func)(CCollider*, CCollider*))
    {
        //함수를 bind해서 넣어 준다.
        m_CollisionBegin = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
    template <typename T>
    void SetCollisionEndFunc(T* Obj, void(T::* Func)(CCollider*, CCollider*))
    {
        //함수를 bind해서 넣어 준다.
        m_CollisionEnd = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }


public:
    virtual void Start();
    virtual void Update(float DeltaTime) = 0;
    virtual void PostUpdate(float DeltaTime) = 0;
    virtual void Render(HDC hDC, float DeltaTime) = 0;

    //충돌체 모양 별로 재정의하여 사용
    virtual bool Collision(CCollider* Dest) = 0;
};


