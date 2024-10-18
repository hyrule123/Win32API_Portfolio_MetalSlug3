#include "RocketThrust.h"

//디버그텍스트
#include "../GameManager.h"


CRocketThrust::CRocketThrust()
{
	SetTypeID<CRocketThrust>();
}

CRocketThrust::CRocketThrust(const CRocketThrust& Obj) :
	CIntroRocket(Obj)
{
}

CRocketThrust::~CRocketThrust()
{
}

bool CRocketThrust::LoadResource()
{
	if (!CIntroRocket::LoadResource())
		return false;

	return true;
}

bool CRocketThrust::Init(CGameObject* Obj)
{
	CIntroRocket::Init(Obj);


	SetRenderLayer(ERenderLayer::Default);

	ChangeAnimation("ThrustNormal");

	return true;
}



void CRocketThrust::Update(float DeltaTime)
{
	CIntroRocket::Update(DeltaTime);

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos();
		m_Pos.y += m_OffsetY;
	}
}

void CRocketThrust::PostUpdate(float DeltaTime)
{
	CIntroRocket::PostUpdate(DeltaTime);

}

void CRocketThrust::ThrustIgnite()
{
	ChangeAnimation("ThrustOn");
}
