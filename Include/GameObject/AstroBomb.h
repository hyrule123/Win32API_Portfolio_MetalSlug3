//아스트로 슬러그에서 발사하는 폭탄
//애니메이션 이름으로 생성해서 좌/우측을 구분해준다.

#pragma once
#include "Projectile.h"

class CAstroBomb :
    public CProjectile
{
    friend class CScene;
protected:
    CAstroBomb();
    CAstroBomb(const CAstroBomb& Obj);
    virtual ~CAstroBomb();
public:
    virtual bool Init(CGameObject* Obj = nullptr);
    virtual bool LoadResource();
    virtual void Update(float DeltaTime);

    virtual void SetEssential(float PosX, float PosY ,bool SlowIgnition);

protected:
    bool m_isRight;
    class CGameObject* m_Target;
    float m_MaxTurnDirX;    //x방향 최대 선회각
    float m_Delay;    //폭탄은 양 사이드가 타이밍이 다르게 나감.
    bool  m_StartIgnition;                        //한쪽은 늦게, 한쪽은 빠르게 나가야함.

protected:
    void AstroBombStartEnd();  //로켓 초반 애니메이션 끝나고 점화
    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
    void HommingBegin(class CCollider* Src, class CCollider* Dest);
};

