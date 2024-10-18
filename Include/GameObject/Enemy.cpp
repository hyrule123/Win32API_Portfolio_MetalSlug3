#include "Enemy.h"

#include "Item.h"

#include "../Scene/MainScene.h"

CEnemy::CEnemy():
	m_ItemDropType(EItemList::End),
	m_isKey(),
	m_Routine(),
	m_FirstEnter(true)
{
	SetTypeID<CEnemy>();
}

CEnemy::CEnemy(const CEnemy& Obj):
	CCharacter(Obj),
	m_ItemDropType(Obj.m_ItemDropType),
	m_isKey(Obj.m_isKey),
	m_Routine(Obj.m_Routine),
	m_FirstEnter(Obj.m_FirstEnter)

{


}

CEnemy::~CEnemy()
{
	if (m_isKey)
	{
		CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
		if (Scene)
		{
			Scene->KeyMonsterDead();
		}
	}
}

bool CEnemy::Init(CGameObject* Obj)
{
	CCharacter::Init(Obj);

	SetRenderLayer(ERenderLayer::Enemy);

	return true;
}

void CEnemy::Start()
{
	m_Start = true;

	
}


void CEnemy::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);
}

void CEnemy::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	////체력 확인
	//if (m_HP <= 0.f)
	//	SetActive(false);

}

void CEnemy::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);
}


int CEnemy::InflictDamage(int Damage)
{
	int DMG = CCharacter::InflictDamage(Damage);

	return DMG;
}

void CEnemy::SetItemDrop(EItemList DropItemType)
{
	m_ItemDropType = DropItemType;
}





void CEnemy::CollBeginMouse(const Vector2& Point, CCollider* Col)
{

}

void CEnemy::ItemDrop()
{
	if (m_ItemDropType < EItemList::End)
	{
		CItem* Item = m_Scene->CreateObject<CItem>("Item");
		if (Item)
		{
			Vector2 Dir;
			Dir.x = (float)(rand() % 6) - 11.f;
			Dir.y = (float)(rand() % 6) - 11.f;
			Dir = Dir.Normalize();

			Item->SetEssential(m_Pos, Dir, m_ItemDropType);
		}
	}
}

void CEnemy::Routine(float DeltaTime)
{
}

void CEnemy::GoNextRoutine()
{
	++m_Routine;
	m_FirstEnter = true;
}

bool CEnemy::CheckFirstEnter()
{
	if (m_FirstEnter)
	{
		m_FirstEnter = false;
		return true;
	}


	return false;
}


void CEnemy::SetKeyMonster()
{
	m_isKey = true;
}
