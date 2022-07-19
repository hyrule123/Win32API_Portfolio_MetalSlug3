#include "Trail.h"

#include "../Scene/Scene.h"
#include "Explosion.h"

CTrail::CTrail():
	m_Frequency(),
	m_FrequencyMax(0.1f),
	m_Force()
{
	SetTypeID<CTrail>();

	m_EffectType = EEffectType::Duration;
}

CTrail::CTrail(const CTrail& Obj) :
	CEffect(Obj),
	m_Frequency(),
	m_FrequencyMax(Obj.m_FrequencyMax),
	m_Force(Obj.m_Force)
{
	//이후 단계에서 설정되는 변수
	//bool m_isReady;
}

CTrail::~CTrail()
{
}

bool CTrail::LoadResource()
{
	CEffect::LoadResource();

	return true;
}

bool CTrail::Init(CGameObject* Obj)
{
	m_isReady = true;

	return true;
}

void CTrail::SetFrequency(float Frequency)
{
	m_FrequencyMax = Frequency;
	m_Frequency = Frequency;
}


void CTrail::Trajectory(float Force)
{
	m_Force = Force;
	SetMaxSpeed(m_Force);
	SetDeAccel(0.9f);
	SetGravityAccel(5.f);
	SetPhysicsSimulate(true);
	Jump();
}

void CTrail::Straight(float Force)
{
	m_Force = Force;
	SetMaxSpeed(m_Force);
}


void CTrail::Update(float DeltaTime)
{

	if (!m_isReady)
		SetActive(false);

	CEffect::Update(DeltaTime);

	AddImpact(m_Force);

	m_Frequency -= DeltaTime;
	if (m_Frequency < 0.f)
	{
		m_Frequency = m_FrequencyMax;

		CExplosion* SFX = m_Scene->CreateObject<CExplosion>("SmallExplosion");
		SFX->SetEssential(EExplosionSize::Small, m_Pos);
		SFX->SetOffset(-17.f, -25.f);

	}

	
}

void CTrail::PostUpdate(float DeltaTime)
{
	CEffect::PostUpdate(DeltaTime);
}

void CTrail::Render(HDC hDC, float DeltaTime)
{
	CEffect::Render(hDC, DeltaTime);
}
