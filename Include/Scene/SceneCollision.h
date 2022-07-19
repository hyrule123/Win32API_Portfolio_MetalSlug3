#pragma once

#include "../GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

	//m_vecCollider을 충돌 채널 수에 맞게 생성.
	std::vector<class CCollider*>	m_vecCollider[(int)ECollisionChannel::Max];
	//현재 마우스 포인터와 충돌 중인 충돌체 주소를 저장해놓는 용도.
	class CCollider* m_MouseColliding;

	//위젯 충돌 처리용 대기열. 위젯 윈도우의 갯수는 많지 않을 것이므로 capacity를기본값으로 냅둔다.
	std::vector<class CWidgetWindow*> m_vecWidgetWindow;
	class CWidget* m_MouseCollidingWidget;

	//매 프레임 CollManager의 주소를 참조해야 하기 때문에 아예 주소를 미리 받아놓는다.
	class CCollisionManager* m_CollManager;


public:
	void AddCollider(CCollider* Col);
	void CollisionUpdate();
	void CollisionUpdateMouse();
	bool DeleteCollider(CCollider* Col);	//vecCollider에 삽입된 자신의 충돌체를 삭제한다.(소멸자에서 호출)

};

