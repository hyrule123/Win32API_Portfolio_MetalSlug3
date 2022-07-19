#pragma once
#include "JustPlay.h"
class CJustPlayFollow :
    public CJustPlay
{
	friend class CScene;

protected:
	CJustPlayFollow();
	CJustPlayFollow(const CJustPlayFollow& Obj);
	virtual ~CJustPlayFollow();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);

};

