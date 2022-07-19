#pragma once
#include "GameObject.h"


//�ܼ��ϰ� RugHime�� ��ġ������ ��Ƽ� �����ϴ� Ŭ����.
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

