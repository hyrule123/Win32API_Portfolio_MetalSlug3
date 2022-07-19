#pragma once
#include "Explosion.h"



//������ ���÷��� �������� üũ��
//SetEssentials�� ���� ���ϴ� ũ�� ����.
class CDamageExplosion :
    public CExplosion
{
	friend class CScene;

protected:
	CDamageExplosion();
	CDamageExplosion(const CDamageExplosion& Obj);
	virtual ~CDamageExplosion();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);

private:
	int m_Damage;
	class CColliderCircle* m_Coll;

public:
	void SetEssential(EExplosionSize ExplosionSize, const Vector2& Pos, int Damage);
	

private:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	void Frame2Notify();
};

