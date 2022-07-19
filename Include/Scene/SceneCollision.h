#pragma once

#include "../GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

	//m_vecCollider�� �浹 ä�� ���� �°� ����.
	std::vector<class CCollider*>	m_vecCollider[(int)ECollisionChannel::Max];
	//���� ���콺 �����Ϳ� �浹 ���� �浹ü �ּҸ� �����س��� �뵵.
	class CCollider* m_MouseColliding;

	//���� �浹 ó���� ��⿭. ���� �������� ������ ���� ���� ���̹Ƿ� capacity���⺻������ ���д�.
	std::vector<class CWidgetWindow*> m_vecWidgetWindow;
	class CWidget* m_MouseCollidingWidget;

	//�� ������ CollManager�� �ּҸ� �����ؾ� �ϱ� ������ �ƿ� �ּҸ� �̸� �޾Ƴ��´�.
	class CCollisionManager* m_CollManager;


public:
	void AddCollider(CCollider* Col);
	void CollisionUpdate();
	void CollisionUpdateMouse();
	bool DeleteCollider(CCollider* Col);	//vecCollider�� ���Ե� �ڽ��� �浹ü�� �����Ѵ�.(�Ҹ��ڿ��� ȣ��)

};

