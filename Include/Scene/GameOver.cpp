#include "GameOver.h"

#include "StartScene.h"
#include "../Input.h"

#include "SceneManager.h"
#include "SceneResource.h"

//�ε��� ��
#include "SceneEdit.h"
#include "MainScene.h"

//�ʱ�ȭ �� �ػ� �޾ƿ��� �뵵
#include "../GameManager.h"

//���ȭ��
#include "../GameObject\Background.h"

//����
#include "../UI/WindowGameOver.h"
#include "../UI/WidgetTextImage.h"

//����
#include "../ScoreManager.h"


//ī�޶�

CGameOver::CGameOver():
	m_Window()
{
}

CGameOver::~CGameOver()
{

}

bool CGameOver::Init()
{
	CScene::Init();

	//���ҽ� �ε�
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontSilver.csv"));
	m_SceneResource->LoadTexture("GameOver0", TEXT("UI/GameOver0.bmp"));
	m_SceneResource->LoadTexture("GameOver1", TEXT("UI/GameOver1.bmp"));

	m_SceneResource->LoadSound("BGM", "MissionFailed", false, "MissionFailed.mp3");
	m_SceneResource->LoadSound("BGM", "MissionAllOver", false, "MissionAllOver.mp3");



	//������ ȣ��
	m_Window = CreateWidgetWindow<CWindowGameOver>("WindowGameOver");
	

	return true;
}

void CGameOver::SetEssential(bool DeathGameOver)
{

	if (!m_Window)
		return;

	CWidgetTextImage* YourScore = m_Window->CreateWidget<CWidgetTextImage>("YourScore");
	if (YourScore)
	{
		YourScore->SetTextImage("BasicFontSilver");
		YourScore->SetPos(220.f, 90.f);
		YourScore->SetAlignMode(EAlignMode::Left);
		YourScore->SetText("Score :");
	}


	CWidgetTextImage* Score = m_Window->CreateWidget<CWidgetTextImage>("Score");
	if (Score)
	{
		Score->SetTextImage("BasicFontSilver");
		Score->SetPos(220.f, 100.f);
		Score->SetAlignMode(EAlignMode::Left);
	}

	//���ȭ��
	CBackground* Back = CreateObject<CBackground>("BackGround");
	if (DeathGameOver)
	{
		Back->SetTexture("GameOver1");
		m_SceneResource->SoundPlay("MissionFailed");
		Score->SetScore(0);
	}
	else
	{
		Back->SetTexture("GameOver1");
		m_SceneResource->SoundPlay("MissionAllOver");
		Score->SetScore(CScoreManager::GetInst()->GetScore());
	}
}



