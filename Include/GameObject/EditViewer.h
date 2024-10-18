#pragma once
#include "GameObject.h"


class CEditViewer :
	public CGameObject
{
	friend class CSceneEdit;
	friend class CScene;

protected:
	CEditViewer();
	CEditViewer(const CGameObject& Obj);
	virtual ~CEditViewer();
public:
	bool Init(CGameObject* Obj = nullptr);



public:
	virtual void Update(float DeltaTime);

	void ViewAnimation(const std::string& AnimName, float PlayTime, ELoopFlag LoopFlag, bool Reverse, float PlayScale, int Layer, float OffsetX, float OffsetY, float PivotX, float PivotY);

	void GetAnimInfoValue(const std::string& AnimName, float& PlayTime, int& LoopFlag, bool& Reverse, float& PlayScale, int& Layer, float& OffsetX, float& OffsetY,
		float& PivotX, float& PivotY);

	class CAnimation* GetAnimation();

	void Play();

	void Render(HDC hDC, float DeltaTime);

	//void ViewFrame();
};

