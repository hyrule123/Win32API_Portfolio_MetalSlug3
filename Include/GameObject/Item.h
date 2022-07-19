#pragma once
#include "Enemy.h"



class CItem :
    public CGameObject
{
	friend class CScene;
protected:
	CItem();
	CItem(const CItem& Obj);
	virtual ~CItem();

public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void SetEssential(const Vector2& Pos, const Vector2& Dir, EItemList ItemType);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	void Reflect(const Vector2& NormalVector);


protected:
	EItemList m_ItemType;
	std::string m_vecAnimName[(int)EItemList::End];
	BoxInfo m_SideCollCheck;
	float m_LeftTime;

private:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
	void RenderScoreNum(int Score);
};

