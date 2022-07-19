#include "Explosion.h"

#include "../Scene/Scene.h"

CExplosion::CExplosion():
	m_vecSizeName{},
	m_CurrentSize()
{
	SetTypeID<CExplosion>();
}

CExplosion::CExplosion(const CExplosion& Obj) :
	CEffect(Obj),
	m_CurrentSize(Obj.m_CurrentSize)
{
	for (int i = 0; i < (int)EExplosionSize::Max; ++i)
	{
		m_vecSizeName[i] = Obj.m_vecSizeName[i];
	}
	//이후 단계에서 설정되는 변수
	//bool m_isReady;
}

CExplosion::~CExplosion()
{
}

bool CExplosion::LoadResource()
{
	CEffect::LoadResource();

	if (!LoadCSVPreset(TEXT("SFX/Explosion.csv")))
		return false;
	if (!LoadCSVPreset(TEXT("SFX/BackExplosion.csv")))
		return false;

	m_vecSizeName[(int)EExplosionSize::Back] = "BackExplosion";
	m_vecSizeName[(int)EExplosionSize::Small] = "SmallExplosion";
	m_vecSizeName[(int)EExplosionSize::Midium] = "MediumExplosion";
	m_vecSizeName[(int)EExplosionSize::Big] = "BigExplosion";

	return true;
}

bool CExplosion::Init(CGameObject* Obj)
{
	CEffect::Init(Obj);
	return true;
}

bool CExplosion::SetEssential(EExplosionSize ExplosionSize, const Vector2& Pos)
{
	m_CurrentSize = ExplosionSize;
	m_Pos = Pos;

	if (!m_Animation->FindAnimInfo(m_vecSizeName[(int)m_CurrentSize]))
		return false;

	SetAnimation(m_vecSizeName[(int)m_CurrentSize]);

	if (m_CurrentSize == EExplosionSize::Big)
		SetRenderLayer(ERenderLayer::EffectHigh);
	else if (m_CurrentSize == EExplosionSize::Back)
		SetRenderLayer(ERenderLayer::Back);

	m_isReady = true;

	return true;
}



void CExplosion::Update(float DeltaTime)
{
	CEffect::Update(DeltaTime);

	if (!m_isReady)
		SetActive(false);
}

void CExplosion::PostUpdate(float DeltaTime)
{
	CEffect::PostUpdate(DeltaTime);
}

void CExplosion::Render(HDC hDC, float DeltaTime)
{
	CEffect::Render(hDC, DeltaTime);
}
