#pragma once
#include "Effect.h"

//������ ���÷��� �������� üũ��

class CRocketLauncherExplosion :
    public CEffect
{
	friend class CScene;

protected:
	CRocketLauncherExplosion();
	CRocketLauncherExplosion(const CRocketLauncherExplosion& Obj);
	virtual ~CRocketLauncherExplosion();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);

private:
	int m_Damage;
	EExplosionSize m_CurrentSize;	//�������� �Ͼ����
	std::string m_SizeName[(int)EExplosionSize::Max];

public:
	void SetEssential(const Vector2& Dir, const Vector2& Pos);
	

private:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	void Frame2Notify();
};

