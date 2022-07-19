#include "WindowEnemyHP.h"

//프로그레스 바
#include "WidgetProgressBar.h"

//숫자
#include "WidgetNumber.h"

//이미지 로드용
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"

//몬스터 위치 추적용
#include "../Scene/Scene.h"
#include "../GameObject/Enemy.h"

//CGameObject의 위치를 추적해야 하므로 Camera 필요
#include "../Scene/Camera.h"

CWindowEnemyHP::CWindowEnemyHP():
	m_Enemy(nullptr),
	m_Offset(0.f, 100.f)
{
}

CWindowEnemyHP::CWindowEnemyHP(const CWindowEnemyHP& widget):
	CWidgetWindow(widget),
	m_Enemy(widget.m_Enemy)
{
}

CWindowEnemyHP::~CWindowEnemyHP()
{
}

bool CWindowEnemyHP::Init()
{
	if (!CWidgetWindow::Init())
		return false;


	SetSize(200.f, 100.f);
	m_Enemy = (CEnemy*)m_Scene->FindObject("Enemy");

	if (m_Enemy)
		SetPos(m_Enemy->GetPos());

	m_Bar = CreateWidget<CWidgetProgressBar>("EnemyHPBar"); 
	//우선 텍스처 로딩을 해주고
	m_Scene->GetSceneResource()->LoadTexture("BarBack", TEXT("Bar/BarBack.bmp"));
	m_Bar->SetTexture(EProgressBar_TextureType::Back, "BarBack");

	m_Scene->GetSceneResource()->LoadTexture("Bar", TEXT("Bar/BarDefault.bmp"));
	m_Bar->SetTexture(EProgressBar_TextureType::Bar, "Bar");

	m_Bar->SetBarFrameSize(200.f, 50.f);
	m_Bar->SetPadding(10.f);

	

	//숫자위젯 추가 과정//////////////////////////////////////////
	m_Number = CreateWidget<CWidgetNumber>("EnemyHPNum");
	//애니메이션 로드
	m_Scene->GetSceneResource()->CreateAnimationSequence("Number", "Number", TEXT("UI/Number.bmp"));
	//컬러키를 설정
	m_Scene->GetSceneResource()->SetColorKey("Number",163,73,164);

	//숫자 스프라이트 좌표 추가
	for (int i = 0; i < 10; ++i)
	{
		m_Scene->GetSceneResource()->AddAnimationSpriteFrame("Number",
			i * 17.f, 0.f, 16.f, 16.f);
	}

	//WidgetNumber 클래스에 넘버 이미지를 등록.
	m_Number->SetNumberImage("Number");
	
	//4배 크게 그려줌.
	m_Number->SetScale(4.f);

	//몬스터의 위치 + 100 위치에 그려줌
	m_Number->SetOffset(0.f, 100.f);
///////////////////////////////////////////////////////////////

	return true;
}



void CWindowEnemyHP::SetOffset(const Vector2& Offset)
{
	m_Offset = Offset;
}

void CWindowEnemyHP::Update(float DeltaTime)
{

	CWidgetWindow::Update(DeltaTime);

	if (!m_Enemy->GetActive())
	{
		m_Enemy = nullptr;
		SetActive(false);
	}



	if (m_Enemy)
	{
		SetPos(m_Enemy->GetPos() - m_Scene->GetCamera()->GetPos() + m_Offset);
	}
}

void CWindowEnemyHP::PostUpdate(float DeltaTime)
{

	CWidgetWindow::PostUpdate(DeltaTime);

	if (!m_Enemy->GetActive())
	{
		m_Enemy = nullptr;
		SetActive(false);
	}
}

void CWindowEnemyHP::Render(HDC hDC, float DeltaTime)
{
	if (!m_Enemy->GetActive())
	{
		m_Enemy = nullptr;
		SetActive(false);
	}

	CWidgetWindow::Render(hDC, DeltaTime);
}


