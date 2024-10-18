#include "WidgetFadeInOut.h"

//이미지 로드용
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"

CWidgetFadeInOut::CWidgetFadeInOut():
	m_isFadeIn(),
	m_StepMovePos(22.f),
	m_FrameTickLeft(2),
	m_FrameTickSet(2),
	m_isSet(),
	m_End()
{
	m_ZOrder = 10;
	m_SkipCollisionCheck = true;
}

CWidgetFadeInOut::CWidgetFadeInOut(const CWidgetFadeInOut& widget) :
	CWidgetImage(widget),
	m_isFadeIn(widget.m_isFadeIn),
	m_StepMovePos(widget.m_StepMovePos),
	m_FrameTickLeft(widget.m_FrameTickLeft),
	m_FrameTickSet(widget.m_FrameTickSet),
	m_isSet(widget.m_isSet),
	m_End(widget.m_End)
{
}

CWidgetFadeInOut::~CWidgetFadeInOut()
{
}

bool CWidgetFadeInOut::Init()
{
	m_Scene->GetSceneResource()->LoadTexture("FadeInOut", TEXT("UI/FadeInOut.bmp"));

	CTexture* Texture = m_Scene->GetSceneResource()->FindTexture("FadeInOut");
	if (!Texture)
		return false;

	Texture->SetColorKey(255, 0, 255);

	

	SetRange(Vector2(0, 0), Texture->GetTextureSize());
	SetSize(Texture->GetTextureSize());

	m_Texture = Texture;

	return true;
}


void CWidgetFadeInOut::Update(float DeltaTime)
{
	CWidgetImage::Update(DeltaTime);

	if(!m_End)
		PosCalc();
}

void CWidgetFadeInOut::PostUpdate(float DeltaTime)
{
	CWidgetImage::PostUpdate(DeltaTime);
}

void CWidgetFadeInOut::Render(HDC hDC, float DeltaTime)
{
	if (!m_isSet)
		return;


	CWidgetImage::Render(hDC, DeltaTime);
}

void CWidgetFadeInOut::SetEssential(bool isFadeIn)
{
	m_isFadeIn = isFadeIn;

	if (isFadeIn)
	{
		m_Pos.x = 0.f;
	}
	else
	{
		m_Pos.x = -526.f;
	}
	m_Pos.y = 0.f;


	m_isSet = true;
}

void CWidgetFadeInOut::PosCalc()
{
	--m_FrameTickLeft;
	if (m_FrameTickLeft <= 0)
	{
		m_FrameTickLeft = m_FrameTickSet;

		if (m_isFadeIn)
		{
			if (m_Pos.x + m_Size.x < 0.f)
			{
				m_End = true;
				if (m_EndFunc)
					m_EndFunc();
				SetActive(false);   //페이드인일 경우에는 화면 열리면 삭제
				return;
			}

				m_Pos.x -= m_StepMovePos;
		}
		else
		{   
			if (m_Pos.x > (m_StepMovePos * -2.f))
			{
				m_End = true;
				if (m_EndFunc)
					m_EndFunc();
				return;
			}

			m_Pos.x += m_StepMovePos;   
		}
	}
}


