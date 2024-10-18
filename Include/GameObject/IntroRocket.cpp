#include "IntroRocket.h"

CIntroRocket::CIntroRocket():
	m_OffsetY(),
	m_TriggerYPos(),
	m_Separtion(),
	m_CapSeparateSFXNotified()
{
	SetTypeID<CIntroRocket>();
}

CIntroRocket::CIntroRocket(const CIntroRocket& Obj) :
	CGameObject(Obj),
	m_OffsetY(Obj.m_OffsetY),
	m_TriggerYPos(Obj.m_TriggerYPos),
	m_Separtion(Obj.m_Separtion),
	m_CapSeparateSFXNotified(Obj.m_CapSeparateSFXNotified)
{
}

CIntroRocket::~CIntroRocket()
{
}



bool CIntroRocket::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/IntroRocket.csv")))
		return false;


	return true;
}

bool CIntroRocket::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	//오브젝트 컬링해제.
	SetCulling(false);

	m_Size.Set(150.f, 1250.f);

	SetEndFunction<CIntroRocket>("SideThrustStart", this, &CIntroRocket::SideThrustStartEndFunc);
	
	if (m_Name == "HeadRightSideWhole")
	{
		SetEndFunction<CIntroRocket>("CapSeparateSFX", this, &CIntroRocket::SeparateSFXNotify);
	}

	//로켓 분리및 점화 시점 지정
	m_TriggerYPos = 30.f;

	return true;
}

void CIntroRocket::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_Separtion)
	{
		m_OffsetY += 1000.f * DeltaTime;
	}

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos();
		m_Pos.y += m_OffsetY;
	}

}

void CIntroRocket::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CIntroRocket::SideThrustStartEndFunc()
{
	ChangeAnimation("SideThrustMax");
	SetEndFunction<CIntroRocket>("SideThrustMax", this, &CIntroRocket::SideThrustMaxEndFunc, true);
}

void CIntroRocket::SideThrustMaxEndFunc()
{
	//쓰러스트 점화
}

void CIntroRocket::SeparateSFXNotify()
{
	m_CapSeparateSFXNotified = true;
}

bool CIntroRocket::GetCapSeparateSFXNotified() const
{
	return m_CapSeparateSFXNotified;
}

void CIntroRocket::Separate()
{
	m_Separtion = true;
}

void CIntroRocket::SetOffsetY(float OffsetY)
{
	m_OffsetY = OffsetY;
}

void CIntroRocket::NoseSeparation(float DirX)
{
	SetDir(DirX, 0.f);
	SetMaxSpeed(200.f);
	AddImpact(200.f);


	SetDeAccel(0.98f);
	SetGravityAccel(15.f);
	SetPhysicsSimulate(true);
	Jump();
}
