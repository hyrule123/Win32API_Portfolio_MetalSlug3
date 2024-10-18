#include "IntroRocketMid.h"

//디버그텍스트
#include "../GameManager.h"

#include "../Scene/Scene.h"
#include "RocketThrust.h"

CIntroRocketMid::CIntroRocketMid()
{
	SetTypeID<CIntroRocketMid>();
}

CIntroRocketMid::CIntroRocketMid(const CIntroRocketMid& Obj) :
	CIntroRocket(Obj)
{
}

CIntroRocketMid::~CIntroRocketMid()
{
}

bool CIntroRocketMid::LoadResource()
{
	if (!CIntroRocket::LoadResource())
		return false;

	return true;
}

bool CIntroRocketMid::Init(CGameObject* Obj)
{
	CIntroRocket::Init(Obj);

	//아랫단부터 한단계씩 올려줌
	SetRenderLayer(ERenderLayer::Enemy);

	ChangeAnimation("BodyMiddle");
	ChangeAnimation("BodyUpperRingInner");

	m_Thrust = m_Scene->CreateObject<CRocketThrust>("Thrust", this);
	m_Thrust->ThrustIgnite();

	return true;
}



void CIntroRocketMid::Update(float DeltaTime)
{
	CIntroRocket::Update(DeltaTime);

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos();
		m_Pos.y += m_OffsetY;
	}


	CGameManager::GetInst()->DebugTextOut("Thrust", m_Thrust->GetPos());
}

void CIntroRocketMid::PostUpdate(float DeltaTime)
{
	CIntroRocket::PostUpdate(DeltaTime);

}