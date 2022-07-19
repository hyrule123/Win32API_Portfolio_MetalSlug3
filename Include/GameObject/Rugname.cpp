#include "Rugname.h"
#include "RugnamePart.h"

#include "../Scene/Scene.h"

CRugname::CRugname():
	m_RugNameLower(),
	m_RugNameUpper()
{
	SetTypeID<CRugname>();
}

CRugname::CRugname(const CRugname& Obj) :
	CGameObject(Obj),
	m_RugNameLower(),
	m_RugNameUpper()
{
}

CRugname::~CRugname()
{
}

bool CRugname::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	SoundPlay("LowPitchThrust");
	SoundPlay("RugName");

	//라그네임 로드
	m_RugNameLower = m_Scene->CreateObject<CRugnamePart>("RugnameLower", this);
	m_RugNameLower->SetEssential(ERugnameLayer::Lower);
	m_RugNameUpper = m_Scene->CreateObject<CRugnamePart>("RugnameUpper", this);
	m_RugNameUpper->SetEssential(ERugnameLayer::Upper);


	SetPivot(0.f, 0.f);
	SetPos(0.f, -100.f);
	SetMaxSpeed(20.f);
	MoveToDest(true, EMoveToMethod::MoveDir, Vector2(0.f, 0.f));


	return true;
}


void CRugname::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
	CGameObject::Update(DeltaTime);
}

void CRugname::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CGameObject::PostUpdate(DeltaTime);
}


void CRugname::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CGameObject::Render(hDC, DeltaTime);
}

bool CRugname::LoadResource()
{
	return true;
}