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

#include "../UI/WindowStart.h"

#include "../UI/WidgetTextImage.h"


//카메라

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
	
}

bool CStartScene::Init()
{
	CScene::Init();

	//음량 지정
	GetSceneResource()->SetMasterVolume(25);


	//리소스 로드
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontSilver.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontGold.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/ScoreNumber.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/MissionFont.csv"));
	
	//배경화면
	//CreateObject<CBackground>("BackGround");



	//윈도우 호출
	CreateWidgetWindow<CWindowStart>("WindowStart");






	return true;
}



