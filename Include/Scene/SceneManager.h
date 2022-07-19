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

	//�� ���� �޼ҵ�
	template <typename T>
	bool CreateScene()
	{
		T* Scene = new T;
		
		if(!Scene->Init())
		{ 
			SAFE_DELETE(Scene);
			return false;
		}
			
		//�� ������ ���������� ������� m_PresentScene�� ��ĳ������ ���� ���
		if (!m_PresentScene)
			m_PresentScene = (CScene*)Scene;

		//Ȥ�ó� ������ ���� �ִٸ� ���� NextScene�� ���.
		else
			m_NextScene = (CScene*)Scene;

		return true;
	}

	bool Init();
	
	//boolŸ���� ��ȯ�ϴ� ������ ���� ����Ǿ��� ��� true�� ��ȯ�� ���� ó�������� �ߴ��ϰ�
	//�ٷ� ���� �� �ε� �������� �����ϱ� ���ؼ��̴�.
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

