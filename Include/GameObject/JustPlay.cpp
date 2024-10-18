#include "JustPlay.h"

CJustPlay::CJustPlay()
{
	SetTypeID<CJustPlay>();
}

CJustPlay::CJustPlay(const CJustPlay& Obj) :
	CEffect(Obj)
{
}

CJustPlay::~CJustPlay()
{
}

bool CJustPlay::Init(CGameObject* Obj)
{
	CEffect::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	SetCullingDelete(ECullingDelete::All, false);

	//트레일 이펙트의 렌더링 레이어는 default
	m_RenderLayer = ERenderLayer::Default;

	return true;
}

void CJustPlay::Start()
{
	m_Start = true;

	if (m_MasterObject)
	{
		m_Pos = m_MasterObject->GetPos() + m_Offset;
	}
}

void CJustPlay::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CEffect::Update(DeltaTime);
}

void CJustPlay::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CEffect::PostUpdate(DeltaTime);
}


void CJustPlay::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CEffect::Render(hDC, DeltaTime);
}

bool CJustPlay::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/RocketLauncherTrail.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("SFX/AstroTailSFX.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("UI/Interface.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("SFX/GetAmmo.csv")))
		return false;

	return true;
}