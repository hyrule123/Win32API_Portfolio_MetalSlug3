//�ƽ�Ʈ�� �����׿��� �߻��ϴ� ��ź
//�ִϸ��̼� �̸����� �����ؼ� ��/������ �������ش�.

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
    float m_MaxTurnDirX;    //x���� �ִ� ��ȸ��
    float m_Delay;    //��ź�� �� ���̵尡 Ÿ�̹��� �ٸ��� ����.
    bool  m_StartIgnition;                        //������ �ʰ�, ������ ������ ��������.

protected:
    void AstroBombStartEnd();  //���� �ʹ� �ִϸ��̼� ������ ��ȭ
    void CollisionBegin(class CCollider* Src, class CCollider* Dest);
    void HommingBegin(class CCollider* Src, class CCollider* Dest);
};

