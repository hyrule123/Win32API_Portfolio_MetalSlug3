//SceneManager.h

#pragma once

#include "../Singleton.h"
#include "../GameInfo.h"
#include "Scene.h"



class CSceneManager
{
private:
	CScene* m_PresentScene;
	CScene* m_NextScene;

public:
	bool ChangeScene();
	CScene* GetNextScene();

	//씬 생성 메소드
	template <typename T>
	bool CreateScene()
	{
		T* Scene = new T;
		
		if(!Scene->Init())
		{ 
			SAFE_DELETE(Scene);
			return false;
		}
			
		//씬 생성에 성공했으면 멤버변수 m_PresentScene에 업캐스팅을 통해 등록
		if (!m_PresentScene)
			m_PresentScene = (CScene*)Scene;

		//혹시나 기존에 씬이 있다면 현재 NextScene에 등록.
		else
			m_NextScene = (CScene*)Scene;

		return true;
	}

	bool Init();
	
	//bool타입을 반환하는 이유는 씬이 변경되었을 경우 true를 반환해 이후 처리과정을 중단하고
	//바로 다음 씬 로딩 과정으로 진입하기 위해서이다.
	bool Update(float DeltaTime);

	bool PostUpdate(float DeltaTime);

	void Render(HDC hDC, float DeltaTime);

	void ChangeGameOver();

	CScene* GetScene()	const
	{
		return m_PresentScene;
	}

	DECLARE_SINGLETON(CSceneManager)
};

