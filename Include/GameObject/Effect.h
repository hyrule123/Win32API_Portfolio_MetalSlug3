#pragma once
#include "GameObject.h"

class CEffect: public CGameObject
{
	friend class CScene;

protected:
	CEffect();
	CEffect(const CEffect& Obj);
	virtual ~CEffect();

protected:
	EEffectType m_EffectType;
	float m_Duration;
	float m_Time;
	Vector2 m_Offset;
	bool m_isReady; //���� �غ� �Ϸ�Ǿ����� Ȯ��. �غ� �ȵ�ä�� Update���� �ٷ� ����

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual bool LoadResource();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual void SetEssential(const Vector2& Pos);	//�ݵ�� �����ؾ� �ϴ� ����

protected:
	void EffectEnd();

public:
	void SetOffset(const Vector2& Offset);
	void SetOffset(float OffsetX, float OffsetY);
	void SetOffsetX(float OffsetX);
	void SetOffsetY(float OffsetY);

	EEffectType GetEffect() const;

	void SetEffect(EEffectType Type);

	void SetDuration(float Duration);

};

