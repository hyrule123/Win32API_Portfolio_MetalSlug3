#pragma once

#include "JustPlay.h"

enum class ERugnameLayer
{
	Lower,
	Upper
};

class CRugnamePart : public CJustPlay
{
	friend class CScene;

protected:
	CRugnamePart();
	CRugnamePart(const CRugnamePart& Obj);
	virtual ~CRugnamePart();
public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual bool LoadResource();
	virtual void SetEssential(ERugnameLayer Layer);

private:
	ERugnameLayer m_RugnameLayer;
};