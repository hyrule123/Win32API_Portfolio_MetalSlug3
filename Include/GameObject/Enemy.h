#pragma once

#include "Character.h"

class CEnemy : public CCharacter
{
	friend class CScene;

protected:
	CEnemy();
	CEnemy(const CEnemy& Obj);
	virtual ~CEnemy();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

	virtual int InflictDamage(int Damage);

protected:
	EItemList m_ItemDropType;
	bool m_isKey;
	UINT8 m_Routine;
	bool m_FirstEnter;

protected:
	void CollBeginMouse(const Vector2& Point, class CCollider* Col);
	void ItemDrop();
	virtual void Routine(float DeltaTime);
	void GoNextRoutine();
	bool CheckFirstEnter();

public:
	virtual void SetItemDrop(EItemList DropItemType);
	void SetKeyMonster();
};