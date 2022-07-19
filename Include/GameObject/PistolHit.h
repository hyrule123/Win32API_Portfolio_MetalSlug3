#pragma once
#include "Effect.h"

//���� �����Ǿ�� �ϹǷ� �ش� ������ ��� �ְ� �ؼ� �ִ��� ������ ����
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

