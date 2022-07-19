#include "SceneCollision.h"

#include "../Collision/Collider.h"

#include "../Collision/CollisionManager.h"

//���� �浹 �˻��
#include "../UI/WidgetWindow.h"
#include "../UI/Widget.h"

//���콺 �Է����� ��ǥ
#include "../Input.h"

CSceneCollision::CSceneCollision()
{
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		//�� �浹ü ä�� �迭���� ������ 100���� �Ҵ��س��´�.
		m_vecCollider[i].reserve(100);
	}

	m_CollManager = CCollisionManager::GetInst();

}

CSceneCollision::~CSceneCollision()
{
}

void CSceneCollision::AddCollider(CCollider* Col)
{
	//�ڽ��� �Ҽӵ� Collision Channel �迭 �� pushback ���ش�.
	m_vecCollider[(int)Col->GetCollisionProfile()].push_back(Col);
}

void CSceneCollision::CollisionUpdate()
{

	//�ϴ� i��ŭ ��ȸ�� ���ƾ� �Ѵ�.
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		//���� ä���� �浹ü�� ��������� ��ŵ�Ѵ�.
		if (m_vecCollider[i].empty())
			continue;


		for (int j = i + 1; j < (int)ECollisionChannel::Max; ++j)
		{
			//�� ��� ä���� �浹ü�� ��������� ��ŵ�Ѵ�.
			if (m_vecCollider[j].empty())
				continue;

			size_t size_i = m_vecCollider[i].size();
			for (size_t k = 0; k < size_i; ++k)
			{

				//���� �ݺ����� ���������� ���� ����
				//ù���� ���ҳ��� ���� ������� �浹ü�̸� 
				//���� ���ҵ鵵 ���� ���� �浹 �׷��̱� ������
				//���� ���þ����Ƿ� �ݺ��� ��ü �ߴ�
				bool noCollision = false;


				size_t size_j = m_vecCollider[j].size();
				for (size_t l = 0; l < size_j; ++l)
				{
					ECollisionChannel SrcProfile = m_vecCollider[i][k]->GetCollisionProfile();
					ECollisionChannel DestProfile = m_vecCollider[j][l]->GetCollisionProfile();

					//���� �� ä���̶� ���� ä�ΰ� �浹 ������ ���ٸ� 
					//�� ä���� ������� �浹 ���� �����̹Ƿ� break�� ���� ��ŵ
					if (!m_CollManager->GetCollisionProfile(SrcProfile)->Interaction[(int)DestProfile] ||
						!m_CollManager->GetCollisionProfile(DestProfile)->Interaction[(int)SrcProfile] )
					{
						
						noCollision = true;
						break;
					}

					//��������� �浹 ó���� �����Ѵ�.
					CCollider* Src = m_vecCollider[i][k];
					CCollider* Dest = m_vecCollider[j][l];

					//�浹 ���θ� Ȯ���ؼ� �浹�̸�
					if (Src->Collision(Dest))
					{
						//�ٵ� ���� ù �浹�̸�
						//->Src�� �浹���� �浹ü ����Ʈ�� Dest�� ������
						if (!Src->CheckCollisionList(Dest))
						{
							//������ ����Ʈ�� ���θ� �����Ѵ�.
							Src->AddCollisionList(Dest);
							Dest->AddCollisionList(Src);

							//�׸��� ������ �浹 ȣ�� �Լ��� ��븦 �־� ȣ���Ѵ�.
							Src->CallCollisionBegin(Dest);
							Dest->CallCollisionBegin(Src);

						}		
					}
					//���� �浹 ���°� �ƴѵ� �ڽ��� �浹�� ����Ʈ �ȿ�
					//��� ������Ʈ�� ���� ��� 
					//-> �浹���̾��µ� �ٽ� �������ٴ� �ǹ��̴�!
					else if (Src->CheckCollisionList(Dest))
					{
						//���ο��� �ڽ��� ����Ʈ���� ���쵵�� ��û
						Src->DeleteCollisionList(Dest);
						Dest->DeleteCollisionList(Src);

						//�׸��� �浹 ���� �Լ��� ȣ��
						Src->CallCollisionEnd(Dest);
						Dest->CallCollisionEnd(Src);
					}
				}

				//���������� ������� �����̹Ƿ� ���⵵ ���������ش�.
				if (noCollision)
					break;
			}
		}
	}

	//���콺�͵� �浹Ȯ���� �� ���� �ϹǷ� ���⼭ �迭�� ������ �ʴ´�.
}

void CSceneCollision::CollisionUpdateMouse()
{
	//���콺�� �浹��, ���ӿ�����Ʈ�� UI�� ���� �� �ϳ��͸� �浹�� �� �ִ�.
	//�׷��Ƿ� �� �� � �� �浹�ϴ� 
	//���� �浹���̴� ���ӿ�����Ʈ �ּҿ� UI �ּҴ� �������־�� �Ѵ�.
	//���� UI���� �浹�� �Ͼ���� Ȯ���ϸ�,
	//���� ������ ��ŵ�ϰ� �������͵� �ȴ�.(�� �ϳ��� �浹 �����ϹǷ�)

	//���콺 �浹�˻� ����.
	m_vecWidgetWindow.clear();
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		m_vecCollider[i].clear();
	}
	return;


	//UI �浹�˻� ��, �̸� ���콺�� ��ǥ���� �޾Ƴ��´�.
	Vector2 MousePos = CInput::GetInst()->GetMousePos();


	//���� UI �浹�˻翡�� �浹�� ��ü�� �־��� ���, ���� �ܰ踦 ��ŵ�ϱ� ���� ����
	bool Skip = false;


	size_t size = m_vecWidgetWindow.size();
	for (size_t i = 0; i < size; ++i)
	{
		CWidget* CollidingWidget = nullptr;

		//���� �浹���� Ȯ�� �Ǿ��� ���
		if (m_vecWidgetWindow[i]->CollisionMouse(&CollidingWidget, MousePos))
		{
			if (!m_vecWidgetWindow[i]->GetCheckCollide())
				continue;

			//���� �浹���� ������ �ְ� && �� �浹���� ���� �ּҰ� CollidingWidget�� �ٸ���,
			if (m_MouseCollidingWidget && (m_MouseCollidingWidget != CollidingWidget))
			{
				//������ �浹���̴� ������ �浹�� �����ٴ� ����̹Ƿ� �浹 ���� �Լ��� ȣ���Ѵ�.
				//�׸��� ���߿� ���ο� �ּҸ� ����Ѵ�.
				m_MouseCollidingWidget->CollisionMouseReleaseCallback();
				
			}
			
			//���� �浹���̴� '���ӿ�����Ʈ'�� �ִٸ�,�ش� �ּ��� �浹 ���� �Լ��� ȣ���ϰ�
			//�ּҸ� �����Ѵ�.
			if (m_MouseColliding)
			{
				m_MouseColliding->SetMouseColliding(false);
				m_MouseColliding->CallCollisionEndMouse(CInput::GetInst()->GetMouseWorldPos());
				m_MouseColliding = nullptr;
			}

			m_MouseCollidingWidget = CollidingWidget;

			Skip = true;

			break;
			//return�� �ȵ� -> �������� �迭�� �����ϴ� ������ �־ �ش� ������ �ݵ�� ���ľ� ��.
		}


	}








	if (!Skip)
	{

		//���⿡ ���Դٴ� ����,
		//� UI�͵� �浹���� �ʾҴٴ� ���̴�.
		//���� �浹���̴� UI�� ���� ���,
		//�浹 ���� �Լ��� ȣ���� �� �ּҸ� �����Ѵ�.
		if (m_MouseCollidingWidget)
		{
			m_MouseCollidingWidget->CollisionMouseReleaseCallback();
			m_MouseCollidingWidget = nullptr;
		}



		//Src = ������ ���콺��ġ
	//�̸� ��ǥ���� �޾Ƴ��´�.
		MousePos = CInput::GetInst()->GetMouseWorldPos();


		//y������ ���� ��ȸ�� �� ���� �ε����� ����صΰ�
		//��ȸ�� ������ ��ǥ�� ����� �浹 ó���� �Ϸ��Ѵ�.
		int iIndex = 0;
		int kIndex = 0;

		//�����ϳ� �迭�� ��ü �� ������� ���� �����Ƿ�
		//���� ������
		bool NotEmpty = false;

		//�ϴ� i��ŭ ��ȸ�� ���ƾ� �Ѵ�.
		for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
		{
			//���� ä���� �浹ü�� ��������� ��ŵ�Ѵ�.
			if (m_vecCollider[i].empty())
				continue;

			

			size_t size_i = m_vecCollider[i].size();
			for (size_t k = 0; k < size_i; ++k)
			{
				if (!m_vecCollider[i][k]->GetCheckMouseCollision())
					continue;


				ECollisionChannel SrcProfile = ECollisionChannel::Mouse;
				ECollisionChannel DestProfile = m_vecCollider[i][k]->GetCollisionProfile();

				//���� �� ä���̶� ���� ä�ΰ� �浹 ������ ���ٸ� 
				//�� ä���� ������� �浹 ���� �����̹Ƿ� break�� ���� ��ŵ
				if (!m_CollManager->GetCollisionProfile(SrcProfile)->Interaction[(int)DestProfile] ||
					!m_CollManager->GetCollisionProfile(DestProfile)->Interaction[(int)SrcProfile])
				{
					break;
				}


				CCollider* Dest = m_vecCollider[i][k];
				//�浹 ���θ� Ȯ���ؼ� �浹�̸�
				if (Dest->CollisionPoint(MousePos))
				{

					//ó�� ���⿡ ���������� ���� ������ ���� �ε����� �ٲ��ְ�
					//���� ������ ������ false�� �ٲ��ش�.
					if (iIndex + kIndex == 0)
					{
						iIndex = (int)i;
						kIndex = (int)k;
						NotEmpty = true;
					}


					//���������, Y-Sorting�� ���� index�� ���Ѵ�.
					//���� ���� i,k���� Bottom���� ������ Bottom������ Ŭ ���
					//������ �ε��� ��ȣ�� �����Ѵ�.
					if (m_vecCollider[i][k]->GetBottom() >= m_vecCollider[iIndex][kIndex]->GetBottom())
					{
						iIndex = (int)i;
						kIndex = (int)k;
					}
				}
				//���� �浹 ���°� �ƴѵ�(else) 
				// �ڽ��� �浹�� ����Ʈ �ȿ� ��� ������Ʈ�� ���� ���(+ if)
				//-> �浹���̾��µ� �ٽ� �������ٴ� �ǹ��̴�!
				else if (Dest->CheckMouseColliding())
				{
					Dest->SetMouseColliding(false);
					Dest->CallCollisionEndMouse(MousePos);

					m_MouseColliding = nullptr;
				}

			}
		}

		//���� �浹�� �浹ü�� �ϳ��� �־��� ���
		if (NotEmpty)
		{
			//����� �ε��� = ���� �浹ü
			CCollider* Dest = m_vecCollider[iIndex][kIndex];

			//�ٵ� ���� ù �浹�̸�
			//->Src�� �浹���� �浹ü ����Ʈ�� Dest�� ������
			if (!Dest->CheckMouseColliding())
			{
				Dest->SetMouseColliding(true);

				//���콺�� �浹�� ����� �浹 ȣ�� �Լ��� ��ǥ�� �־� ȣ���Ѵ�.
				Dest->CallCollisionBeginMouse(MousePos);

				//�ش� �浹ü�� ���� �浹���� �浹ü �ּҿ� �����Ѵ�.
				//���� �浹���̴� �浹ü�� �־��� ��� �ش� �浹ü�ʹ� �浹�� �ƴ϶�� �ǹ��̹Ƿ�
				//�ش� �ּҷ� ���� �浹 ���� �Լ��� ȣ���� �ڿ� �ڽ����� �ּҸ� �ٲ� �ش�.
				if (m_MouseColliding)
				{
					m_MouseColliding->SetMouseColliding(false);
					m_MouseColliding->CallCollisionEndMouse(MousePos);
				}
				m_MouseColliding = Dest;

			}


		}
	}


	//����� �������� ������ ���ӿ�����Ʈ ��� �迭�� ����ش�.
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
