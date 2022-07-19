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

#include "../UI/WindowStart.h"

#include "../UI/WidgetTextImage.h"


//ī�޶�

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
	
}

bool CStartScene::Init()
{
	CScene::Init();

	//���� ����
	GetSceneResource()->SetMasterVolume(25);


	//���ҽ� �ε�
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontSilver.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/BasicFontGold.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/ScoreNumber.csv"));
	m_SceneResource->LoadCSVPreset(TEXT("UI/MissionFont.csv"));
	
	//���ȭ��
	//CreateObject<CBackground>("BackGround");



	//������ ȣ��
	CreateWidgetWindow<CWindowStart>("WindowStart");






	return true;
}



