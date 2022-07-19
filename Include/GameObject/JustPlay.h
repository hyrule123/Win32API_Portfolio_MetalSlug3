#pragma once
#include "Effect.h"

//�ܼ� ����Ʈ ��� Ŭ����
//���⿡ �ε��س��� ����Ʈ�� �̸����� �����ϸ� �ڵ����� �ش� ����Ʈ�� ����Ѵ�.
//�ݵ�� ��ġ�� �������ٰ�.(SetEssential)
class CJustPlay :
    public CEffect
{
	friend class CScene;

protected:
	CJustPlay();
	CJustPlay(const CJustPlay& Obj);
	virtual ~CJustPlay();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual bool LoadResource();
};

