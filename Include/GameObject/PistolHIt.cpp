#include "PistolHit.h"

CPistolHit::CPistolHit()
{
	SetTypeID<CPistolHit>();
}

CPistolHit::CPistolHit(const CPistolHit& Obj):
	CEffect(Obj)
{
}

CPistolHit::~CPistolHit()
{
}

bool CPistolHit::Init(CGameObject* Obj)
{
	CEffect::Init();

	SetAnimation("PistolHit");

	m_isReady = true;

	return true;
}

bool CPistolHit::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/PistolHit.csv")))
		return false;

	return true;
}
