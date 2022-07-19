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
    float m_MaxTurnDirX;    //x방향 최대 선회각

protected:
    void RocketStartEnd();  //로켓 초반 애니메이션 끝나고 점화
    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
    void HommingBegin(class CCollider* Src, class CCollider* Dest);
};

