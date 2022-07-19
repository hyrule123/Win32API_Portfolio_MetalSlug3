#pragma once
#include "Projectile.h"
class CRocketLauncher :
    public CProjectile
{
    friend class CScene;
protected:
    CRocketLauncher();
    CRocketLauncher(const CRocketLauncher& Obj);
    virtual ~CRocketLauncher();
public:
    virtual bool Init(CGameObject* Obj = nullptr);
    virtual bool LoadResource();
    virtual void Update(float DeltaTime);

protected:
    float m_TrailTimer;
    bool m_TrailOn;
    class CGameObject* m_Target;
    float m_MaxTurnDirX;    //x���� �ִ� ��ȸ��

protected:
    void RocketStartEnd();  //���� �ʹ� �ִϸ��̼� ������ ��ȭ
    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
    void HommingBegin(class CCollider* Src, class CCollider* Dest);
};

