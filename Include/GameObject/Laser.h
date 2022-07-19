#pragma once
#include "Projectile.h"


class CLaser :
    public CProjectile
{
    friend class CScene;

protected:

    class CColliderLine* m_Coll;
    class CGameObject* m_CollidingObj;
    float m_CollidingObjYPos;  //���� �浹���� ���ӿ�����Ʈ�� y��

    UINT8 m_ColorNum[4];    //4���� ���� �÷�
    UINT8 m_Index;
    bool  m_ChangeIndex;

    float m_KeepFire;    //��� �߻� ������
    float m_KeepFireRefVal;

    float m_DamageFreq;  //�������� �ִ� �󵵼�. ������ ���� �ð��� ���߾ ���ȴ�.
    float m_DamageFreqRefVal;

    Vector2 m_Offset;

    bool m_TurnOff; //������ �߻� ����� �� �ѹ��� ����ɼ��ְ� �ϴ� ����

protected:
    CLaser();
    CLaser(const CLaser& Obj);
    virtual ~CLaser();

public:
    virtual bool Init(CGameObject* Obj = nullptr);
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual bool LoadResource();

    void SetKeepFire(bool KeepFire);
    float GetKeepFire() const;
    void LaserLineOffEnd();
    void LaserHitNotify();


    virtual void SetEssential(const Vector2& Dir, const Vector2& Pos, const Vector2& Offset);
    virtual void SetEssential(float DirX, float DirY, float PosX, float PosY, float OffsetX, float OffsetY);



private:
    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
    void CollisionEnd(class CCollider* Src, class CCollider* Dest);
    static bool SortCollY(const CSharedPtr<class CCollider>& Src, const CSharedPtr<class CCollider>& Dest);
};

