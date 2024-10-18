#include "JustPlayFollow.h"

CJustPlayFollow::CJustPlayFollow()
{
	SetTypeID<CJustPlayFollow>();
}

CJustPlayFollow::CJustPlayFollow(const CJustPlayFollow& Obj) :
	CJustPlay(Obj)
{
}

CJustPlayFollow::~CJustPlayFollow()
{
}

bool CJustPlayFollow::Init(CGameObject* Obj)
{
	CEffect::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	//트레일 이펙트의 렌더링 레이어는 default
	m_RenderLayer = ERenderLayer::EffectLow;

	return true;
}

void CJustPlayFollow::Update(float DeltaTime)
{

}


bool CJustPlayFollow::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/RocketLauncherTrail.csv")))
		return false;


	return true;
}