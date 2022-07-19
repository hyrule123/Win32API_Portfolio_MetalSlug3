#include "WindowEnemyHP.h"

//���α׷��� ��
#include "WidgetProgressBar.h"

//����
#include "WidgetNumber.h"

//�̹��� �ε��
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"

//���� ��ġ ������
#include "../Scene/Scene.h"
#include "../GameObject/Enemy.h"

//CGameObject�� ��ġ�� �����ؾ� �ϹǷ� Camera �ʿ�
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
	//�켱 �ؽ�ó �ε��� ���ְ�
	m_Scene->GetSceneResource()->LoadTexture("BarBack", TEXT("Bar/BarBack.bmp"));
	m_Bar->SetTexture(EProgressBar_TextureType::Back, "BarBack");

	m_Scene->GetSceneResource()->LoadTexture("Bar", TEXT("Bar/BarDefault.bmp"));
	m_Bar->SetTexture(EProgressBar_TextureType::Bar, "Bar");

	m_Bar->SetBarFrameSize(200.f, 50.f);
	m_Bar->SetPadding(10.f);

	

	//�������� �߰� ����//////////////////////////////////////////
	m_Number = CreateWidget<CWidgetNumber>("EnemyHPNum");
	//�ִϸ��̼� �ε�
	m_Scene->GetSceneResource()->CreateAnimationSequence("Number", "Number", TEXT("UI/Number.bmp"));
	//�÷�Ű�� ����
	m_Scene->GetSceneResource()->SetColorKey("Number",163,73,164);

	//���� ��������Ʈ ��ǥ �߰�
	for (int i = 0; i < 10; ++i)
	{
		m_Scene->GetSceneResource()->AddAnimationSpriteFrame("Number",
			i * 17.f, 0.f, 16.f, 16.f);
	}

	//WidgetNumber Ŭ������ �ѹ� �̹����� ���.
	m_Number->SetNumberImage("Number");
	
	//4�� ũ�� �׷���.
	m_Number->SetScale(4.f);

	//������ ��ġ + 100 ��ġ�� �׷���
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


