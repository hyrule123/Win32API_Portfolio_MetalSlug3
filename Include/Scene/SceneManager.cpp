//SceneManager.cpp

#include "SceneManager.h"
#include "MainScene.h"
#include "StartScene.h"

#include "GameOver.h"

DEFINITION_SINGLETON(CSceneManager)

CSceneManager::CSceneManager():
	m_PresentScene(nullptr)
{
}



CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_PresentScene);
}

bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		SAFE_DELETE(m_PresentScene);
		m_PresentScene = m_NextScene;
		m_NextScene = nullptr;

		return true;
	}


	return false;
}

CScene* CSceneManager::GetNextScene()
{
	return m_NextScene;
}

bool CSceneManager::Init()
{
	CreateScene<CStartScene>();

	return true;
}


bool CSceneManager::Update(float DeltaTime)
{
	m_PresentScene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_PresentScene->PostUpdate(DeltaTime);

	return ChangeScene();
}

void CSceneManager::Render(HDC hDC, float DeltaTime)
{
	m_PresentScene->Render(hDC, DeltaTime);
}

void CSceneManager::ChangeGameOver()
{
	CreateScene<CGameOver>();
}
