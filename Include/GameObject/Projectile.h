#pragma once
#include "GameObject.h"

class CProjectile :
	public CGameObject
{
	friend class CScene;
	
protected:
	int m_Damage;
	bool m_isReady;
	

protected:
	CProjectile();
	CProjectile(const CProjectile& Obj);
	virtual ~CProjectile();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	void SetDamage(int Damage);


	virtual void SetEssential(Vector2 Dir, Vector2 Pos);
	virtual void SetEssential(float DirX, float DirY, float PosX, float PosY);

	//SetOffset: 반드시 위치 설정 후에 사용할것.
	void SetOffset(const Vector2& Offset);
	void SetOffset(float x, float y);
	void SetOffsetX(float x);
	void SetOffsetY(float y);

private:
	virtual void CollisionBegin(class CCollider* Src, class CCollider* Dest);
};

