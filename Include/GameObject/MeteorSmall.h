#pragma once
#include "Meteor.h"
class CMeteorSmall :
	public CMeteor
{
	friend class CScene;
protected:
	CMeteorSmall();
	CMeteorSmall(const CMeteorSmall& Obj);
	virtual ~CMeteorSmall();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);


private:
	virtual void CollisionBegin(class CCollider* Src, class CCollider* Dest);

};

