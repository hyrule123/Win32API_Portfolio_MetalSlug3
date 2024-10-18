#include "EditViewer.h"
#include "../Scene/SceneEdit.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneManager.h"
#include "../UI/WidgetComponent.h"
#include "../Resource/Animation/AnimationInfo.h"
#include "../Resource/Animation/Animation.h"

//마우스 위치 표시
#include "../Input.h"

//십자 안내눈금 색깔 변경
#include "../GameManager.h"

CEditViewer::CEditViewer()
{
	SetTypeID<CEditViewer>();
}

CEditViewer::CEditViewer(const CGameObject& Obj):
	CGameObject(Obj)
{
}

CEditViewer::~CEditViewer()
{
}

bool CEditViewer::Init(CGameObject* Obj)
{
	if (!CGameObject::Init())
		return false;

	SetPos(100.f, 100.f);
	SetPivot(0.5f, 1.f);
	SetSize(50.f, 50.f);

	CreateAnimation();

	return true;
}

void CEditViewer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}




void CEditViewer::ViewAnimation(const std::string& AnimName, float PlayTime, ELoopFlag LoopFlag, bool Reverse, float PlayScale, int Layer, float OffsetX, float OffsetY, float PivotX, float PivotY)
{
	if (m_Animation)
	{
		CAnimationInfo* Info = m_Animation->FindAnimInfo(AnimName);

		if (!Info)
		{
			if (!m_Animation->AddAnimationInfo(AnimName, PlayTime, LoopFlag, Reverse, PlayScale, Layer,
				OffsetX, OffsetY,
				PivotX, PivotY))
				return;

			Info = m_Animation->FindAnimInfo(AnimName);
		}


		SetPlayTime(AnimName, PlayTime);
		Info->m_LoopFlag = LoopFlag;
		Info->m_PlayScale = PlayScale;
		Info->m_Reverse = Reverse;
		SetLayer(AnimName, Layer);//레이어의 경우 추가 계산이 필요하므로 메소드로
		Info->m_Offset.x = OffsetX;
		Info->m_Offset.y = OffsetY;
		Info->m_Pivot.x = PivotX;
		Info->m_Pivot.y = PivotY;
	}
}



void CEditViewer::GetAnimInfoValue(const std::string& AnimName, float& PlayTime, int& LoopFlag, bool& Reverse, float& PlayScale, int& Layer, float& OffsetX, float& OffsetY,
	float& PivotX, float& PivotY)
{
	CAnimationInfo* Info = m_Animation->FindAnimInfo(AnimName);

	if (!Info)
		return;

	
	PlayTime = Info->m_PlayTime;
	LoopFlag = (int)Info->m_LoopFlag;
	PlayScale = Info->m_PlayScale;
	Reverse = Info->m_Reverse;
	Layer = Info->m_Layer;
	OffsetX = Info->m_Offset.x;
	OffsetY = Info->m_Offset.y;
	PivotX = Info->m_Pivot.x;
	PivotY = Info->m_Pivot.y;
}

CAnimation* CEditViewer::GetAnimation()
{
	if (!m_Animation)
		return nullptr;

	return m_Animation;
}

void CEditViewer::Play()
{
	for (int i = 0; i <= m_Animation->GetHighestLayer(); ++i)
	{
		m_Animation->PlayCurrentAnimAgain(i);
	}
}

void CEditViewer::Render(HDC hDC, float DeltaTime)
{
	int x = (int)m_Pos.x;
	int y = (int)m_Pos.y;
	MoveToEx(hDC, x - 50, y, nullptr);
	LineTo(hDC, x + 50, y);
	MoveToEx(hDC, x, y - 50, nullptr);
	LineTo(hDC, x, y + 50);

	Vector2 MouseRelativePos = CInput::GetInst()->GetMousePos() - m_Pos;
	CGameManager::GetInst()->DebugTextOut("MouseRelativePos", MouseRelativePos);
	

	CGameObject::Render(hDC, DeltaTime);
}



