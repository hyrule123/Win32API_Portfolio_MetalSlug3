#include "JustMovingSFX.h"

CJustMovingSFX::CJustMovingSFX()
{
	SetTypeID<CJustMovingSFX>();
}

CJustMovingSFX::CJustMovingSFX(const CJustMovingSFX& Obj) :
	CJustPlay(Obj)
{
}

CJustMovingSFX::~CJustMovingSFX()
{
}

bool CJustMovingSFX::Init(CGameObject* Obj)
{
	CJustPlay::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	//트레일 이펙트의 렌더링 레이어는 default
	m_RenderLayer = ERenderLayer::EffectLow;

	return true;
}

void CJustMovingSFX::SetEssential(const Vector2& Dir, const Vector2& Offset, float MaxSpeed)
{
	m_isReady = true;
	m_Dir = Dir;
	m_Offset = Offset;
	m_MaxSpeed = MaxSpeed;
}

void CJustMovingSFX::Update(float DeltaTime)
{

	CJustPlay::Update(DeltaTime);

	MoveDir(m_Dir);
}


bool CJustMovingSFX::LoadResource()
{
	return true;
}