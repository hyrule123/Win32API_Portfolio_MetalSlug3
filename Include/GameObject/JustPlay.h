#pragma once
#include "Effect.h"

//단순 이펙트 재생 클래스
//여기에 로드해놓은 이펙트를 이름으로 생성하면 자동으로 해당 이펙트를 재생한다.
//반드시 위치를 설정해줄것.(SetEssential)
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

