#include "BackModen.h"

#include "../Collision/ColliderBox.h"


CBackModen::CBackModen()
{
	SetTypeID<CBackModen>();

	m_HP = 1;
}

CBackModen::CBackModen(const CBackModen& Obj) :
	CCharacter(Obj)
{

}

CBackModen::~CBackModen()
{
}

bool CBackModen::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	

	return true;
}

void CBackModen::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	if (m_HP <= 0)
	{
		//폭발 재생 및 사망모션
	}


}

void CBackModen::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CBackModen::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}


int CBackModen::InflictDamage(int Damage)
{
	m_HP -= 1;

	return 1;
}
