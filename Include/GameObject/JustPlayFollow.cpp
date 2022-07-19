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

	//Ʈ���� ����Ʈ�� ������ ���̾�� default
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