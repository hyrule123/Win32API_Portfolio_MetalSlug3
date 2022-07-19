#pragma once

#include "../GameInfo.h"

class CCamera
{
	friend class CScene;

private:
	CCamera();
	~CCamera();
	
	Vector2 m_Pos;
	Vector2 m_Resolution;
	Vector2 m_WorldResolution;

	CSharedPtr<class CGameObject> m_Target;
	Vector2 m_TargetPivot;
	Vector2 m_TargetOffset;


public:
	void Update(float DeltaTime);

	void SetPos(const Vector2& Pos);
	void SetPos(float _x, float _y);
	void AddPos(const Vector2& Pos);
	void SetRes(const Vector2& Res);
	void SetRes(float _x, float _y);
	void SetWorldRes(const Vector2& WorldRes);
	void SetWorldRes(float _x, float _y);
	
	void SetTargetObj(class CGameObject* Target);
	void SetTargetPivot(const Vector2& Pivot);
	void SetTargetPivot(float _x, float _y);
	void SetTargetOffset(const Vector2& Offset);
	void SetTargetOffset(float _x, float _y);

	Vector2 GetPos() const;
	Vector2 GetRes() const;
	Vector2 GetWorldRes() const;
	
	class CGameObject* GetTargetObj() const;
	Vector2 GetTargetPivot() const;
	Vector2 GetTargetOffset() const;




};

