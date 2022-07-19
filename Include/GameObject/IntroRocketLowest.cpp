#include "IntroRocketLowest.h"

//������ؽ�Ʈ
#include "../GameManager.h"

#include "RocketThrust.h"
#include "../Scene/Scene.h"

CIntroRocketLowest::CIntroRocketLowest()
{
	SetTypeID<CIntroRocketLowest>();
}

CIntroRocketLowest::CIntroRocketLowest(const CIntroRocketLowest& Obj) :
	CIntroRocket(Obj)
{
}

CIntroRocketLowest::~CIntroRocketLowest()
{
}

bool CIntroRocketLowest::LoadResource()
{
	if (!CIntroRocket::LoadResource())
		return false;

	return true;
}

bool CIntroRocketLowest::Init(CGameObject* Obj)
{
	CIntroRocket::Init(Obj);

	//�Ʒ��ܺ��� �Ѵܰ辿 �÷���
	SetRenderLayer(ERenderLayer::Enemy);

	ChangeAnimation("BodyUpperRingInner");	//��� ���� ���ʺκ�
	ChangeAnimation("BodyLowest");			//����
	
	m_Thrust = m_Scene->CreateObject<CRocketThrust>("Thrust", this);
	m_Thrust->ThrustIgnite();

	return true;
}



void CIntroRocketLowest::Update(float DeltaTime)
{
	CIntroRocket::Update(DeltaTime);

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos();
		m_Pos.y += m_OffsetY;
	}

	CGameManager::GetInst()->DebugTextOut("LowestPos", m_Pos);

}

void CIntroRocketLowest::PostUpdate(float DeltaTime)
{
	CIntroRocket::PostUpdate(DeltaTime);

}