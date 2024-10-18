#include "Effect.h"

#include "../Resource/Animation/Animation.h"
#include "../Resource/Animation/AnimationInfo.h"

CEffect::CEffect():
	m_Duration(),
	m_Time(),
	m_EffectType(EEffectType::Once),
	m_Offset(),
	m_isReady()
{
	SetTypeID<CEffect>();
	m_RenderLayer = ERenderLayer::EffectLow;
}

CEffect::CEffect(const CEffect& Obj):
	CGameObject(Obj),
	m_Duration(),
	m_EffectType(),
	m_Offset(),
	m_isReady(),
	m_Time()
{
	//위의 변수들 모두 Init 이후에 설정되는 변수들임
}

CEffect::~CEffect()
{
}

bool CEffect::LoadResource()
{
	CreateAnimation();

	return true;
}

bool CEffect::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);


	//애니메이션에 애니메이션 종료 함수도 등록
	m_Animation->SetCurrentAnimEndFunc<CEffect>(this, &CEffect::EffectEnd);

	//마스터 오브젝트가 등록되어 있으면 bool m_MasterObject도 true로 바꿔준다.


	return true;
}

void CEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos() + m_Offset;
	}

	if (m_EffectType == EEffectType::Duration)
	{
		m_Duration -= DeltaTime;

		if (m_Duration < 0.f)
		{
			SetActive(false);
		}
	}
}

void CEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffect::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}

void CEffect::SetEssential(const Vector2& Pos)
{
	m_Pos = Pos;

	m_isReady = true;
}

void CEffect::EffectEnd()
{
	//만약 한번만 재생이라면 바로 false
	if (m_EffectType == EEffectType::Once)
	{
		SetActive(false);
	}
}

void CEffect::SetOffset(const Vector2& Offset)
{
	m_Offset = Offset;
}

void CEffect::SetOffset(float OffsetX, float OffsetY)
{
	m_Offset.x = OffsetX;
	m_Offset.y = OffsetY;
}

void CEffect::SetOffsetX(float OffsetX)
{
	m_Offset.x = OffsetX;
}

void CEffect::SetOffsetY(float OffsetY)
{
	m_Offset.y = OffsetY;
}

EEffectType CEffect::GetEffect() const
{
	return m_EffectType;
}

void CEffect::SetEffect(EEffectType Type)
{
	m_EffectType = Type;

	if (Type == EEffectType::Loop || Type == EEffectType::Duration)
	{
		m_Animation->SetLoopFlag();
	}
}

void CEffect::SetDuration(float Duration)
{
	m_EffectType = EEffectType::Duration;
	m_Duration = Duration;
}
