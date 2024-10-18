#include "RugnamePart.h"

CRugnamePart::CRugnamePart():
	m_RugnameLayer()
{
	SetTypeID<CRugnamePart>();
}

CRugnamePart::CRugnamePart(const CRugnamePart& Obj) :
	CJustPlay(Obj),
	m_RugnameLayer(Obj.m_RugnameLayer)
{
}

CRugnamePart::~CRugnamePart()
{
}

bool CRugnamePart::Init(CGameObject* Obj)
{
	CJustPlay::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	SetCullingDelete(ECullingDelete::All, false);

	//트레일 이펙트의 렌더링 레이어는 default
	m_RenderLayer = ERenderLayer::Default;

	SetEffect(EEffectType::Loop);

	SetPivot(0.f, 0.f);

	return true;
}

void CRugnamePart::Start()
{
	m_Start = true;

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos() + m_Offset;
	}
}

void CRugnamePart::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CJustPlay::Update(DeltaTime);
}

void CRugnamePart::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CJustPlay::PostUpdate(DeltaTime);
}


void CRugnamePart::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CJustPlay::Render(hDC, DeltaTime);
}

bool CRugnamePart::LoadResource()
{
	if (!LoadCSVPreset(TEXT("Enemy/Rugname.csv")))
		return false;


	return true;
}

void CRugnamePart::SetEssential(ERugnameLayer Layer)
{
	m_RugnameLayer = Layer;

	switch (m_RugnameLayer)
	{
	case ERugnameLayer::Lower:
	{
		SetAnimation("RugnameLower");
		SetRenderLayer(ERenderLayer::RugnameLower);
		break;
	}
		
	case ERugnameLayer::Upper:
	{
		SetAnimation("RugnameUpper");
		SetRenderLayer(ERenderLayer::RugnameUpper);
		break;
	}
		
	default:
		break;
	}

}
