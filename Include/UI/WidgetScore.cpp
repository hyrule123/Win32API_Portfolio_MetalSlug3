#include "WidgetScore.h"

//텍스처 로드용
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence.h"


#include "WidgetWindow.h"



CWidgetScore::CWidgetScore() :
	m_Timer(2.f)
{
}

CWidgetScore::CWidgetScore(const CWidgetScore& widget) :
	CWidgetTextImage(widget),
	m_Timer(widget.m_Timer)
{
}

CWidgetScore::~CWidgetScore()
{
}

bool CWidgetScore::Init()
{
	CWidgetTextImage::Init();

	SetTextImage("ScoreNumber");
	SetKerning(1);
	SetAlignMode(EAlignMode::Mid);
	


	return true;
}


void CWidgetScore::Update(float DeltaTime)
{
	CWidgetTextImage::Update(DeltaTime);

	m_Timer -= DeltaTime;
	if (m_Timer <= 1.5f)
	{
		if (m_Timer >= 1.45f)
		{
			SetFlicker(2.f, 0.1f);
		}
		
		m_Pos.y -= 30.f * DeltaTime;
	}
	
	if (m_Timer <= 0.f)
		SetActive(false);
	
}

void CWidgetScore::Render(HDC hDC, float DeltaTime)
{
	CWidgetTextImage::Render(hDC, DeltaTime);
}



