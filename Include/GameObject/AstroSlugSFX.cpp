#include "AstroSlugSFX.h"

#include "../Scene/Scene.h"

#include "PlayerSpace.h"

//꼬리 SFXX
#include "AstroTail.h"

CAstroSlugSFX::CAstroSlugSFX():
	m_isTailSFX()
{
	SetTypeID<CAstroSlugSFX>();
}

CAstroSlugSFX::CAstroSlugSFX(const CAstroSlugSFX& Obj):
	CEffect(Obj),
	m_isTailSFX(Obj.m_isTailSFX)
{
	//이후 단계에서 설정되는 변수
	//bool m_isReady;

}

CAstroSlugSFX::~CAstroSlugSFX()
{
}

bool CAstroSlugSFX::LoadResource()
{
	CEffect::LoadResource();

	if (!LoadCSVPreset(TEXT("Player/AstroSlugSFX.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("Player/SlugOut.csv")))
		return false;

	return true;
}

void CAstroSlugSFX::Start()
{
	m_Start = true;

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos() + m_Offset;
	}
}

bool CAstroSlugSFX::Init(CGameObject* Obj)
{
	CEffect::Init(Obj);

	SetCullingDelete(ECullingDelete::All, false);

	if (m_Animation->FindAnimInfo(m_Name))
		SetAnimation(m_Name);

	if (m_Name.find("Small") != std::string::npos)
	{
		SetRenderLayer(ERenderLayer::EffectLow);
	}
	if (m_Name == "BoostSFXTail")
	{
		m_isTailSFX = true;

		SetRenderLayer(ERenderLayer::Default);
		
		//꼬리 강력부스터 연계되는 기능.
		SetEndFunction<CAstroSlugSFX>("TailBoost1", this, &CAstroSlugSFX::Booster1EndFunc);

		//꼬리 부스터 생성을 위한 EndFunction 등록
		SetEndFunction<CAstroSlugSFX>("TailLeft", this, &CAstroSlugSFX::TailLeftEnd);
		SetEndFunction<CAstroSlugSFX>("TailRight", this, &CAstroSlugSFX::TailRightEnd);
		SetEndFunction<CAstroSlugSFX>("TailStraight", this, &CAstroSlugSFX::TailStraightEnd);
		SetEndFunction<CAstroSlugSFX>("TailIdle", this, &CAstroSlugSFX::TailIdleEnd);
	}





	m_isReady = true;

	return true;
}

bool CAstroSlugSFX::SetSFX(const std::string& Name)
{
	if (!m_Animation->FindAnimInfo(Name))
		return false;

	SetAnimation(Name);

	m_isReady = true;

	return true;
}

void CAstroSlugSFX::TailBoost1ReverseSet()
{
	SetPlayReverse("TailBoost1", true, true);
	SetEndFunction("TailBoost1", this, &CAstroSlugSFX::TailBoost1ReverseEnd);
	ChangeAnimation("TailBoost1");
}



void CAstroSlugSFX::Booster1EndFunc()
{
	ChangeAnimation("TailBoost2");
}

void CAstroSlugSFX::TailLeftEnd()
{
	m_TailSFXDir.Set(-1.f, 1.f);
	CreateTailSFX();
}

void CAstroSlugSFX::TailRightEnd()
{
	m_TailSFXDir.Set(1.f, 1.f);
	CreateTailSFX();
}

void CAstroSlugSFX::TailStraightEnd()
{
	m_TailSFXDir.Set(0.f, 1.f);
	CreateTailSFX();
}

void CAstroSlugSFX::TailIdleEnd()
{
	m_TailSFXDir.Set(0.f, 1.f);
	CreateTailSFX();
}

void CAstroSlugSFX::CreateTailSFX()
{
	CAstroTail* TailSFX = m_Scene->CreateObject<CAstroTail>("TailSFX");
	TailSFX->SetEssential(m_TailSFXDir, Vector2(m_Pos.x, m_Pos.y + 10.f), 50.f);
	
}

void CAstroSlugSFX::TailBoost1ReverseEnd()
{
	ChangeAnimation("TailIdle");
	SetEndFunction<CAstroSlugSFX>("TailBoost1", this, &CAstroSlugSFX::Booster1EndFunc);
}


void CAstroSlugSFX::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CEffect::Update(DeltaTime);

	if (!m_isReady)
		SetActive(false);
}

void CAstroSlugSFX::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CEffect::PostUpdate(DeltaTime);
}

void CAstroSlugSFX::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CEffect::Render(hDC, DeltaTime);
}


