#pragma once
#include "GameObject.h"


//단순하게 RugHime의 위치정보를 모아서 저장하는 클래스.
class CRugname :
    public CGameObject
{
	friend class CScene;

protected:
	CRugname();
	CRugname(const CRugname& Obj);
	virtual ~CRugname();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual bool LoadResource();

private:
    class CRugnamePart* m_RugNameLower;
    class CRugnamePart* m_RugNameUpper;
};

