#pragma once
#include "Effect.h"


//SetEssential을 통해서 반드시 폭발체의 크기를 지정해줄것

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

