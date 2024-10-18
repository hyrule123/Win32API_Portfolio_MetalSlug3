#pragma once
#include "JustMovingSFX.h"


class CLaserDebris :
	public CJustMovingSFX
{

	friend class CScene;

protected:

	UINT8 m_ColorNum[4];    //4가지 색상 컬러
	UINT8 m_Index;
	bool  m_ChangeIndex;

	Vector2 m_Offset;
	float m_Force;



protected:
	CLaserDebris();
	CLaserDebris(const CLaserDebris& Obj);
	virtual ~CLaserDebris();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual bool LoadResource();


	virtual void SetEssential(const Vector2& Dir, const Vector2& Pos, const Vector2& Offset, float Force);
	virtual void SetEssential(float DirX, float DirY, float PosX, float PosY, float OffsetX, float OffsetY, float Force);
	void Trajectory(float Force);
};

