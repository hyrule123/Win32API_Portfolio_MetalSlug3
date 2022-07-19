#include "Cursor.h"
#include "../Input.h"
#include "../Collision/ColliderPoint.h"

CCursor::CCursor()
{
}

CCursor::CCursor(const CCursor& Obj):
	CGameObject(Obj)
{
}

CCursor::~CCursor()
{
}

bool CCursor::Init(CGameObject* Obj)
{
	CGameObject::Init();


	//충돌체 설정
	m_MouseCollider = AddCollider<CColliderPoint>("MouseCursor");
	m_MouseCollider->SetCollisionProfile(ECollisionChannel::Mouse);
	m_MouseCollider->SetFixedPos(CInput::GetInst()->GetMouseWorldPos());


	return true;
}

void CCursor::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_MouseCollider->SetFixedPos(CInput::GetInst()->GetMouseWorldPos());
}

void CCursor::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CCursor::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}
