#include "Scene.h"

#include "../Ref.h"
#include "../GameObject/GameObject.h"

//�� ���ҽ� ������
#include "SceneResource.h"
#include "SceneManager.h"

//ī�޶� Ŭ����
#include "Camera.h"

//�� �浹ü ������
#include "SceneCollision.h"

//���� ������ ���Ŀ�
#include "../UI/WidgetWindow.h"


CScene::CScene() :
	m_SceneResource(nullptr),
	m_Player()
{
	m_SceneResource = new CSceneResource;
	m_Camera = new CCamera;
	m_Collision = new CSceneCollision;

}

CScene::~CScene()
{

	//���ӿ�����Ʈ ���� �� �����ְ�
	for (int i = 0; i < (int)ERenderLayer::Max; ++i)
	{
		m_ObjList[i].clear();
	}
	m_mapOriginalObj.clear();

	//�ش� ���ӿ�����Ʈ���� ������̴� ���ҽ� ����
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Camera);
	SAFE_DELETE(m_SceneResource);
}

bool CScene::Init()
{
	

	return true;
}




void CScene::Update(float DeltaTime)
{
	//ī�޶��� ��ġ�� �����´�
	m_CamPos = m_Camera->GetPos();
	m_CamRes = m_Camera->GetRes();


	for (int i = 0; i < (int)ERenderLayer::Max; ++i)
	{
		auto iter = m_ObjList[i].begin();
		auto iterEnd = m_ObjList[i].end();

		while (iter != iterEnd)
		{

			if (!(*iter)->GetActive())
			{
				++iter;
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}

	//UI ������Ʈ
	{
		auto iter = m_vecWidgetWindow.begin();
		auto iterEnd = m_vecWidgetWindow.end();

		while (iter != iterEnd)
		{
			if (!(*iter)->GetActive())
			{
				iter = m_vecWidgetWindow.erase(iter);
				iterEnd = m_vecWidgetWindow.end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}



	//ī�޶� ������Ʈ
	m_Camera->Update(DeltaTime);
	
}

void CScene::PostUpdate(float DeltaTime)
{
	for (int i = 0; i < (int)ERenderLayer::Max; ++i)
	{
		auto iter = m_ObjList[i].begin();
		auto iterEnd = m_ObjList[i].end();

		while (iter != iterEnd)

		{
			if (!(*iter)->GetActive())
			{
				//���� ������Ʈ�� ���� ������Ʈ ��Ͽ��� �ڽ��� ����
				if ((*iter)->m_MasterObject)
				{
					CGameObject* master = (*iter)->m_MasterObject;

					auto Masteriter = master->m_SlaveObject.begin();
					auto MasteriterEnd = master->m_SlaveObject.end();

					while (Masteriter != MasteriterEnd)
					{
						if ((*iter) == (*Masteriter))
						{
							master->m_SlaveObject.erase(Masteriter);
							break;
						}

						++Masteriter;
					}
				}

				//���� ������Ʈ�� ��ȸ�ϸ鼭 �ڽŰ��� ������ ���� SetActive�� false�� ��ȯ
				auto SlaveIter = (*iter)->m_SlaveObject.begin();
				auto SlaveIterEnd = (*iter)->m_SlaveObject.end();

				while (SlaveIter != SlaveIterEnd)
				{
					(*SlaveIter)->SetActive(false);
					(*SlaveIter)->m_MasterObject = nullptr;
					++SlaveIter;
				}
				//���� ��⸦ ���ѳ������Ƿ� ����Ʈ�� ����ش�.
				(*iter)->m_SlaveObject.clear();

				iter = m_ObjList[i].erase(iter);
				iterEnd = m_ObjList[i].end();
				continue;
			}
			
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			else
			{
				(*iter)->PostUpdate(DeltaTime);
			}

			++iter;
		}
	}

	//UI ������Ʈ
	{
		auto iter = m_vecWidgetWindow.begin();
		auto iterEnd = m_vecWidgetWindow.end();

		//�켱 ���Ұ� 2�� �̻��� ��� Zorder ������ ���ĺ��� ���ش�.
		if (m_vecWidgetWindow.size() > 1)
		{
			std::sort(iter, iterEnd, CScene::SortWidgetWindow);
		}
		

		while (iter != iterEnd)
		{
			if (!(*iter)->GetActive())
			{
				iter = m_vecWidgetWindow.erase(iter);
				iterEnd = m_vecWidgetWindow.end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			else
			{
				(*iter)->PostUpdate(DeltaTime);

				//PostUpdate�� ���� ������������� �浹 �˻� ��⿭�� �߰�
				if ((*iter)->GetCheckCollide())
					m_Collision->m_vecWidgetWindow.push_back(*iter);
			}


			++iter;
		}
	}



	m_Collision->CollisionUpdate();
	m_Collision->CollisionUpdateMouse();
}

void CScene::Render(HDC hDC, float DeltaTime)
{
	for (int i = 0; i < (int)ERenderLayer::Max; ++i)
	{
		m_ObjList[i].sort(SortY);

		auto iter = m_ObjList[i].rbegin();
		auto iterEnd = m_ObjList[i].rend();

		while (iter != iterEnd)
		{
			//���Ŵ� ��� Update()���� ������. ���� �׳� ���д�.
			if (!(*iter)->GetActive())
			{
				++iter;
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			else
			{
				(*iter)->Render(hDC, DeltaTime);
			}
			
			++iter;
		}
	}


	//UI ����. PostUpdate���� ZOrder ������������ ���������Ƿ� �������� ����Ѵ�.
	{
		auto iter = m_vecWidgetWindow.rbegin();
		auto iterEnd = m_vecWidgetWindow.rend();

		while (iter!=iterEnd)
		{
			if (!(*iter)->GetActive())
			{
				++iter;
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			else
			{
				(*iter)->Render(hDC, DeltaTime);
			}

			
			++iter;
		}
	}



}




bool CScene::SortY(const CSharedPtr<class CGameObject>& Src, const CSharedPtr<class CGameObject>& Dest)
{
	float SrcY = Src->GetPos().y + (1.f - Src->GetPivot().y) * Src->GetSize().y;
	float DestY = Dest->GetPos().y + (1.f - Dest->GetPivot().y) * Dest->GetSize().y;

	return SrcY < DestY;
}

void CScene::SetCamTarget(CGameObject* Obj)
{
	m_Camera->SetTargetObj(Obj);
}


const Vector2& CScene::GetCamPos() const
{
	return m_CamPos;
}

const Vector2& CScene::GetCamRes() const
{
	return m_CamRes;
}

CSceneCollision* CScene::GetSceneCollision() const
{
	return m_Collision;
}

bool CScene::SortWidgetWindow(const CSharedPtr<CWidgetWindow>& Src, const CSharedPtr<CWidgetWindow>& Dest)
{
	return (Src->GetZOrder() > Dest->GetZOrder());
}

CWidgetWindow* CScene::FindWidgetWindow(const std::string& Name)
{
	size_t size = m_vecWidgetWindow.size();
	
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecWidgetWindow[i]->GetName() == Name)
		{
			return m_vecWidgetWindow[i];
		}
	}

	return nullptr;
}

CCamera* CScene::GetCamera() const
{
	return m_Camera;
}

CSceneResource* CScene::GetSceneResource() const
{
	return m_SceneResource;
}

CPlayer* CScene::GetPlayer() const
{
	return m_Player;
}

void CScene::SetPlayer(CPlayer* Player)
{
	m_Player = Player;
}

void CScene::DeletePlayer(CPlayer* Player)
{
	if (m_Player == Player)
		m_Player = nullptr;
}

void CScene::SetEssential()
{
}

bool CScene::ChangeRenderLayer(CGameObject* Obj, ERenderLayer SrcLayer, ERenderLayer DestLayer)
{
	auto iter = m_ObjList[(int)SrcLayer].begin();
	auto iterEnd = m_ObjList[(int)SrcLayer].end();
	CGameObject* Temp = nullptr;

	while (iter != iterEnd)
	{
		if ((*iter) == Obj)
		{
			Temp = *iter;	//�ּ� �޾Ƽ�
			m_ObjList[(int)DestLayer].push_back(Temp);//�� ���̾ ����
			m_ObjList[(int)SrcLayer].erase(iter);	//�ش� ����Ʈ�� ���� ��

			return true;
		}
		++iter;
	}


		
	return false;
}

CGameObject* CScene::FindObject(const std::string& Name)
{
	for (int i = 0; i < (int)ERenderLayer::Max; ++i)
	{
		std::list<CSharedPtr<class CGameObject>>::iterator iter = m_ObjList[i].begin();
		std::list<CSharedPtr<class CGameObject>>::iterator iterEnd = m_ObjList[i].end();

		while (iter != iterEnd)
		{
			if ((*iter)->m_Name == Name)
				return *iter;

			++iter;
		}

	}
	return nullptr;
}



