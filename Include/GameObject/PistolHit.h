#pragma once
#include "Effect.h"

//많이 생성되어야 하므로 해당 정보만 들고 있게 해서 최대한 가볍게 생성
class CPistolHit :
    public CEffect
{
	friend class CScene;

protected:
	CPistolHit();
	CPistolHit(const CPistolHit& Obj);
	virtual ~CPistolHit();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual bool LoadResource();

};

