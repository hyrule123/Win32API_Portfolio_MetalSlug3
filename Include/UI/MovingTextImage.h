#pragma once
#include "WidgetTextImage.h"
class CMovingTextImage :
	public CWidgetTextImage
{
	friend class CWidgetWindow;

protected:
	CMovingTextImage();
	CMovingTextImage(const CMovingTextImage& widget);
	virtual ~CMovingTextImage();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	bool m_MoveToOn;
	Vector2 m_MoveToDest;
	Vector2 m_Dir;
	float m_MaxSpeed;
	void MoveDir(float DeltaTime);
public:
	void SetEssential(float MaxSpeed, const Vector2& Dest);
	bool GetMoveDone()  const;

};

