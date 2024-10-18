#include "SceneCollision.h"

#include "../Collision/Collider.h"

#include "../Collision/CollisionManager.h"

//위젯 충돌 검사용
#include "../UI/WidgetWindow.h"
#include "../UI/Widget.h"

//마우스 입력지점 좌표
#include "../Input.h"

CSceneCollision::CSceneCollision()
{
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		//각 충돌체 채널 배열별로 공간을 100개씩 할당해놓는다.
		m_vecCollider[i].reserve(100);
	}

	m_CollManager = CCollisionManager::GetInst();

}

CSceneCollision::~CSceneCollision()
{
}

void CSceneCollision::AddCollider(CCollider* Col)
{
	//자신이 소속된 Collision Channel 배열 에 pushback 해준다.
	m_vecCollider[(int)Col->GetCollisionProfile()].push_back(Col);
}

void CSceneCollision::CollisionUpdate()
{

	//일단 i만큼 순회를 돌아야 한다.
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		//비교할 채널의 충돌체가 비어있으면 스킵한다.
		if (m_vecCollider[i].empty())
			continue;


		for (int j = i + 1; j < (int)ECollisionChannel::Max; ++j)
		{
			//비교 대상 채널의 충돌체가 비어있으면 스킵한다.
			if (m_vecCollider[j].empty())
				continue;

			size_t size_i = m_vecCollider[i].size();
			for (size_t k = 0; k < size_i; ++k)
			{

				//다음 반복문도 빠져나오기 위한 변수
				//첫번쨰 원소끼리 전혀 관계없는 충돌체이면 
				//다음 원소들도 각각 같은 충돌 그룹이기 때문에
				//전혀 관련없으므로 반복문 전체 중단
				bool noCollision = false;


				size_t size_j = m_vecCollider[j].size();
				for (size_t l = 0; l < size_j; ++l)
				{
					ECollisionChannel SrcProfile = m_vecCollider[i][k]->GetCollisionProfile();
					ECollisionChannel DestProfile = m_vecCollider[j][l]->GetCollisionProfile();

					//둘중 한 채널이라도 상대방 채널과 충돌 판정이 없다면 
					//두 채널은 관계없는 충돌 판정 라인이므로 break를 통해 스킵
					if (!m_CollManager->GetCollisionProfile(SrcProfile)->Interaction[(int)DestProfile] ||
						!m_CollManager->GetCollisionProfile(DestProfile)->Interaction[(int)SrcProfile] )
					{
						
						noCollision = true;
						break;
					}

					//통과했으면 충돌 처리를 시작한다.
					CCollider* Src = m_vecCollider[i][k];
					CCollider* Dest = m_vecCollider[j][l];

					//충돌 여부를 확인해서 충돌이면
					if (Src->Collision(Dest))
					{
						//근데 만약 첫 충돌이면
						//->Src의 충돌중인 충돌체 리스트에 Dest가 없으면
						if (!Src->CheckCollisionList(Dest))
						{
							//서로의 리스트에 서로를 삽입한다.
							Src->AddCollisionList(Dest);
							Dest->AddCollisionList(Src);

							//그리고 각자의 충돌 호출 함수에 상대를 넣어 호출한다.
							Src->CallCollisionBegin(Dest);
							Dest->CallCollisionBegin(Src);

						}		
					}
					//만약 충돌 상태가 아닌데 자신의 충돌중 리스트 안에
					//상대 오브젝트가 있을 경우 
					//-> 충돌중이었는데 다시 떨어졌다는 의미이다!
					else if (Src->CheckCollisionList(Dest))
					{
						//서로에게 자신을 리스트에서 지우도록 요청
						Src->DeleteCollisionList(Dest);
						Dest->DeleteCollisionList(Src);

						//그리고 충돌 종료 함수를 호출
						Src->CallCollisionEnd(Dest);
						Dest->CallCollisionEnd(Src);
					}
				}

				//마찬가지로 상관없는 라인이므로 여기도 빠져나와준다.
				if (noCollision)
					break;
			}
		}
	}

	//마우스와도 충돌확인을 해 봐야 하므로 여기서 배열을 지우지 않는다.
}

void CSceneCollision::CollisionUpdateMouse()
{
	//마우스의 충돌은, 게임오브젝트던 UI던 간에 단 하나와만 충돌할 수 있다.
	//그러므로 둘 중 어떤 게 충돌하던 
	//기존 충돌중이던 게임오브젝트 주소와 UI 주소는 해제해주어야 한다.
	//또한 UI에서 충돌이 일어났음을 확인하면,
	//이후 과정은 스킵하고 빠져나와도 된다.(단 하나만 충돌 가능하므로)

	//마우스 충돌검사 안함.
	m_vecWidgetWindow.clear();
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		m_vecCollider[i].clear();
	}
	return;


	//UI 충돌검사 전, 미리 마우스의 좌표값을 받아놓는다.
	Vector2 MousePos = CInput::GetInst()->GetMousePos();


	//만약 UI 충돌검사에서 충돌된 객체가 있었을 경우, 다음 단계를 스킵하기 위한 변수
	bool Skip = false;


	size_t size = m_vecWidgetWindow.size();
	for (size_t i = 0; i < size; ++i)
	{
		CWidget* CollidingWidget = nullptr;

		//만약 충돌임이 확인 되었을 경우
		if (m_vecWidgetWindow[i]->CollisionMouse(&CollidingWidget, MousePos))
		{
			if (!m_vecWidgetWindow[i]->GetCheckCollide())
				continue;

			//만약 충돌중인 위젯이 있고 && 이 충돌중인 위젯 주소가 CollidingWidget과 다르면,
			if (m_MouseCollidingWidget && (m_MouseCollidingWidget != CollidingWidget))
			{
				//기존에 충돌중이던 위젯의 충돌이 끝났다는 얘기이므로 충돌 종료 함수를 호출한다.
				//그리고 나중에 새로운 주소를 등록한다.
				m_MouseCollidingWidget->CollisionMouseReleaseCallback();
				
			}
			
			//만약 충돌중이던 '게임오브젝트'가 있다면,해당 주소의 충돌 종료 함수를 호출하고
			//주소를 제거한다.
			if (m_MouseColliding)
			{
				m_MouseColliding->SetMouseColliding(false);
				m_MouseColliding->CallCollisionEndMouse(CInput::GetInst()->GetMouseWorldPos());
				m_MouseColliding = nullptr;
			}

			m_MouseCollidingWidget = CollidingWidget;

			Skip = true;

			break;
			//return은 안됨 -> 마지막에 배열을 제거하는 과정이 있어서 해당 과정을 반드시 거쳐야 함.
		}


	}








	if (!Skip)
	{

		//여기에 들어왔다는 것은,
		//어떤 UI와도 충돌하지 않았다는 뜻이다.
		//만약 충돌중이던 UI가 있을 경우,
		//충돌 종료 함수를 호출한 뒤 주소를 제거한다.
		if (m_MouseCollidingWidget)
		{
			m_MouseCollidingWidget->CollisionMouseReleaseCallback();
			m_MouseCollidingWidget = nullptr;
		}



		//Src = 무조건 마우스위치
	//미리 좌표값을 받아놓는다.
		MousePos = CInput::GetInst()->GetMouseWorldPos();


		//y소팅을 위해 순회를 돌 때는 인덱스만 기록해두고
		//순회가 끝나면 좌표를 사용해 충돌 처리를 완료한다.
		int iIndex = 0;
		int kIndex = 0;

		//만에하나 배열이 전체 다 비어있을 수도 있으므로
		//에러 방지용
		bool NotEmpty = false;

		//일단 i만큼 순회를 돌아야 한다.
		for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
		{
			//비교할 채널의 충돌체가 비어있으면 스킵한다.
			if (m_vecCollider[i].empty())
				continue;

			

			size_t size_i = m_vecCollider[i].size();
			for (size_t k = 0; k < size_i; ++k)
			{
				if (!m_vecCollider[i][k]->GetCheckMouseCollision())
					continue;


				ECollisionChannel SrcProfile = ECollisionChannel::Mouse;
				ECollisionChannel DestProfile = m_vecCollider[i][k]->GetCollisionProfile();

				//둘중 한 채널이라도 상대방 채널과 충돌 판정이 없다면 
				//두 채널은 관계없는 충돌 판정 라인이므로 break를 통해 스킵
				if (!m_CollManager->GetCollisionProfile(SrcProfile)->Interaction[(int)DestProfile] ||
					!m_CollManager->GetCollisionProfile(DestProfile)->Interaction[(int)SrcProfile])
				{
					break;
				}


				CCollider* Dest = m_vecCollider[i][k];
				//충돌 여부를 확인해서 충돌이면
				if (Dest->CollisionPoint(MousePos))
				{

					//처음 여기에 진입했으면 에러 방지를 위해 인덱스를 바꿔주고
					//에러 방지용 변수를 false로 바꿔준다.
					if (iIndex + kIndex == 0)
					{
						iIndex = (int)i;
						kIndex = (int)k;
						NotEmpty = true;
					}


					//통과했으면, Y-Sorting을 위해 index를 비교한다.
					//새로 들어온 i,k값의 Bottom값이 기존의 Bottom값보다 클 경우
					//저장한 인덱스 번호를 변경한다.
					if (m_vecCollider[i][k]->GetBottom() >= m_vecCollider[iIndex][kIndex]->GetBottom())
					{
						iIndex = (int)i;
						kIndex = (int)k;
					}
				}
				//만약 충돌 상태가 아닌데(else) 
				// 자신의 충돌중 리스트 안에 상대 오브젝트가 있을 경우(+ if)
				//-> 충돌중이었는데 다시 떨어졌다는 의미이다!
				else if (Dest->CheckMouseColliding())
				{
					Dest->SetMouseColliding(false);
					Dest->CallCollisionEndMouse(MousePos);

					m_MouseColliding = nullptr;
				}

			}
		}

		//만약 충돌된 충돌체가 하나라도 있었을 경우
		if (NotEmpty)
		{
			//저장된 인덱스 = 최종 충돌체
			CCollider* Dest = m_vecCollider[iIndex][kIndex];

			//근데 만약 첫 충돌이면
			//->Src의 충돌중인 충돌체 리스트에 Dest가 없으면
			if (!Dest->CheckMouseColliding())
			{
				Dest->SetMouseColliding(true);

				//마우스가 충돌한 대상의 충돌 호출 함수에 좌표를 넣어 호출한다.
				Dest->CallCollisionBeginMouse(MousePos);

				//해당 충돌체를 현재 충돌중인 충돌체 주소에 저장한다.
				//만약 충돌중이던 충돌체가 있었을 경우 해당 충돌체와는 충돌이 아니라는 의미이므로
				//해당 주소로 가서 충돌 종료 함수를 호출한 뒤에 자신으로 주소를 바꿔 준다.
				if (m_MouseColliding)
				{
					m_MouseColliding->SetMouseColliding(false);
					m_MouseColliding->CallCollisionEndMouse(MousePos);
				}
				m_MouseColliding = Dest;

			}


		}
	}


	//계산이 끝났으면 위젯과 게임오브젝트 모두 배열을 비워준다.
	m_vecWidgetWindow.clear();
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		m_vecCollider[i].clear();
	}


}

bool CSceneCollision::DeleteCollider(CCollider* Col)
{
	ECollisionChannel Profile = Col->GetCollisionProfile();

	auto iter = m_vecCollider[(int)Profile].begin();
	auto iterEnd = m_vecCollider[(int)Profile].end();

	while (iter != iterEnd)
	{
		if (*iter == Col)
		{
			m_vecCollider[(int)Profile].erase(iter);

			return true;
		}

		++iter;
	}


	return false;
}
