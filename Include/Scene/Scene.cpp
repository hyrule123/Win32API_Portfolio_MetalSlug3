#include "Scene.h"

#include "../Ref.h"
#include "../GameObject/GameObject.h"

//씬 리소스 관리자
#include "SceneResource.h"
#include "SceneManager.h"

//카메라 클래스
#include "Camera.h"

//씬 충돌체 관리자
#include "SceneCollision.h"

//위젯 윈도우 정렬용
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

	//게임오브젝트 먼저 싹 지워주고
	for (int i = 0; i < (int)ERenderLayer::Max; ++i)
	{
		m_ObjList[i].clear();
	}
	m_mapOriginalObj.clear();

	//해당 게임오브젝트들이 사용중이던 리소스 제거
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
	//카메라의 위치를 얻어놓는다
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

	//UI 업데이트
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



	//카메라 업데이트
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
				//주인 오브젝트의 종속 오브젝트 목록에서 자신을 삭제
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

				//종속 오브젝트를 순회하면서 자신과의 연결을 끊고 SetActive를 false로 전환
				auto SlaveIter = (*iter)->m_SlaveObject.begin();
				auto SlaveIterEnd = (*iter)->m_SlaveObject.end();

				while (SlaveIter != SlaveIterEnd)
				{
					(*SlaveIter)->SetActive(false);
					(*SlaveIter)->m_MasterObject = nullptr;
					++SlaveIter;
				}
				//삭제 대기를 시켜놓았으므로 리스트를 비워준다.
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

	//UI 업데이트
	{
		auto iter = m_vecWidgetWindow.begin();
		auto iterEnd = m_vecWidgetWindow.end();

		//우선 원소가 2개 이상일 경우 Zorder 순으로 정렬부터 해준다.
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

				//PostUpdate가 끝난 위젯윈도우들은 충돌 검사 대기열에 추가
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
			//제거는 모두 Update()에서 관리함. 여긴 그냥 냅둔다.
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


	//UI 렌더. PostUpdate에서 ZOrder 내림차순으로 정렬했으므로 역순으로 출력한다.
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
			Temp = *iter;	//주소 받아서
			m_ObjList[(int)DestLayer].push_back(Temp);//새 레이어에 삽입
			m_ObjList[(int)SrcLayer].erase(iter);	//해당 리스트는 제거 후

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



