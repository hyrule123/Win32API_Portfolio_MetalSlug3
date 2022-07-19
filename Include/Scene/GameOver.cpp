#include "GameOver.h"

#include "StartScene.h"
#include "../Input.h"

#include "SceneManager.h"
#include "SceneResource.h"

//로드할 씬
#include "SceneEdit.h"
#include "MainScene.h"

//초기화 때 해상도 받아오는 용도
#include "../GameManager.h"

//배경화면
#include "../GameObject\Background.h"

//위젯
#include "../UI/WindowGameOver.h"
#include "../UI/WidgetTextImage.h"

//점수
#include "../ScoreManager.h"


//카메라

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

	//리소스 로드
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontSilver.csv"));
	m_SceneResource->LoadTexture("GameOver0", TEXT("UI/GameOver0.bmp"));
	m_SceneResource->LoadTexture("GameOver1", TEXT("UI/GameOver1.bmp"));

	m_SceneResource->LoadSound("BGM", "MissionFailed", false, "MissionFailed.mp3");
	m_SceneResource->LoadSound("BGM", "MissionAllOver", false, "MissionAllOver.mp3");



	//윈도우 호출
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

	//배경화면
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



