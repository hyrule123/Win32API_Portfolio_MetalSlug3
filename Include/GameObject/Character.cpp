#include "Character.h"



CCharacter::CCharacter():
	m_HP(1)
{
	SetTypeID<CCharacter>();
}

CCharacter::CCharacter(const CCharacter& Obj):
	CGameObject(Obj),
	m_HP(Obj.m_HP)
{
}

CCharacter::~CCharacter()
{
}



bool CCharacter::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	return true;
}

void CCharacter::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


}

void CCharacter::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CCharacter::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}


int CCharacter::InflictDamage(int Damage)
{
	return CGameObject::InflictDamage(Damage);
}




