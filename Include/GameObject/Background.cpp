#include "Background.h"

#include "../GameInfo.h"
#include "../GameManager.h"

#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CBackground::CBackground()
{
	m_RenderLayer = ERenderLayer::Back;
}

CBackground::~CBackground()
{
}

bool CBackground::Init(CGameObject* Obj)
{
	CGameObject::Init();

	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);

	return true;
}

void CBackground::Start()
{
	CGameObject::Start();
}

void CBackground::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBackground::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}
