#pragma once
#include "WidgetWindow.h"

#include "../GameObject/Enemy.h"


class CWindowEnemyHP :
	public CWidgetWindow
{
	friend class CScene;

protected:
	CWindowEnemyHP();
	CWindowEnemyHP(const CWindowEnemyHP& widget);
	virtual ~CWindowEnemyHP();
public:
	virtual bool Init();

private:
	class CWidgetProgressBar* m_Bar;
	class CWidgetNumber* m_Number;


	class CEnemy* m_Enemy;
	float m_EnemyHPMax; //초기화때 바로 몬스터의 HP를 받아와서 그걸 만피로 설정

	//몬스터의 위치에서 기본적으로 얼마나 떨어뜨려서 출력할지
	//(체력바이므로)
	Vector2 m_Offset;
public:
	void SetOffset(const Vector2& Offset);

public:
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

};

