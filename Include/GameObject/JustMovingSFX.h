#pragma once
#include "JustPlay.h"
class CJustMovingSFX :
    public CJustPlay
{
	friend class CScene;

protected:
	CJustMovingSFX();
	CJustMovingSFX(const CJustMovingSFX& Obj);
	virtual ~CJustMovingSFX();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void SetEssential(const Vector2& Dir, const Vector2& Offset, float MaxSpeed);
	virtual void Update(float DeltaTime);
};

