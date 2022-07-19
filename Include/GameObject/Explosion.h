#pragma once
#include "Effect.h"


//SetEssential�� ���ؼ� �ݵ�� ����ü�� ũ�⸦ �������ٰ�

class CExplosion :
    public CEffect
{
	friend class CScene;

protected:
	CExplosion();
	CExplosion(const CExplosion& Obj);
	virtual ~CExplosion();

public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	bool SetEssential(EExplosionSize ExplosionSize, const Vector2& Pos);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

protected:
	EExplosionSize m_CurrentSize;
	std::string m_vecSizeName[(int)EExplosionSize::Max];
};

