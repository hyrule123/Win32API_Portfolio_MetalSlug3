#pragma once
#include "Meteor.h"
class CMeteorMid :
	public CMeteor
{
	friend class CScene;
protected:
	CMeteorMid();
	CMeteorMid(const CMeteorMid& Obj);
	virtual ~CMeteorMid();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);
	virtual void SetItemDrop(EItemList ItemType);

private:
	virtual void CollisionBegin(class CCollider* Src, class CCollider* Dest);

};

