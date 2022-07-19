#include "Weapons.h"

CWeapons::CWeapons()
{
	SetTypeID<CWeapons>();
}

CWeapons::CWeapons(const CWeapons& Obj):
	CEffect(Obj)
{
}

CWeapons::~CWeapons()
{
}

bool CWeapons::Init(CGameObject* Obj)
{
	CEffect::Init(Obj);



	return true;
}

bool CWeapons::LoadResource()
{
	LoadCSVPreset(TEXT("Player/SlugOut.csv"));

	return true;
}

void CWeapons::Update(float DeltaTime)
{
	CEffect::Update(DeltaTime);
}

void CWeapons::PostUpdate(float DeltaTime)
{
}

void CWeapons::Render(HDC hDC, float DeltaTime)
{
}
