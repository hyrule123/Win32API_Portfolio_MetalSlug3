#include "WindowSpace.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

#include "WidgetFadeInOut.h"

#include "WidgetImage.h"
#include "WidgetTextImage.h"

#include "WidgetProgressBar.h"

#include "../ScoreManager.h"

#include "MovingTextImage.h"

CWindowSpace::CWindowSpace():
	m_BulletLeft(),
	m_BombLeft(),
	m_HPBar(),
	m_Score(),
	m_TimeLeft(240.f),
	m_TimeNumber(),
	m_AdjustTime(4)
{
}

CWindowSpace::CWindowSpace(const CWindowSpace& widget) :
	CWidgetWindow(widget),
	m_BulletLeft(widget.m_BulletLeft),
	m_BombLeft(widget.m_BombLeft),
	m_HPBar(widget.m_HPBar),
	m_Score(widget.m_Score),
	m_TimeLeft(widget.m_TimeLeft),
	m_TimeNumber(widget.m_TimeNumber),
	m_AdjustTime(widget.m_AdjustTime)
{
}


CWindowSpace::~CWindowSpace()
{
}

bool CWindowSpace::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetPos(0.f, 0.f);
	SetSize(Vector2(ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT));

	CWidgetFadeInOut* FadeIn = CreateWidget<CWidgetFadeInOut>("FadeIn");
	FadeIn->SetEssential(true);



	return true;
}


void CWindowSpace::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	if (m_TimeNumber)
	{
		int TimeLeft = (int)m_TimeLeft;
		m_TimeLeft -= DeltaTime;
		if ((TimeLeft / m_AdjustTime != ((int)m_TimeLeft / m_AdjustTime)))
		{
			m_TimeNumber->SetText((int)m_TimeLeft / m_AdjustTime);
		}
	}

	if (m_Score)
		m_Score->SetScore(CScoreManager::GetInst()->GetScore());
}

void CWindowSpace::PostUpdate(float DeltaTime)
{

	CWidgetWindow::PostUpdate(DeltaTime);

}

void CWindowSpace::Render(HDC hDC, float DeltaTime)
{
	CWidgetWindow::Render(hDC, DeltaTime);
}

void CWindowSpace::ResetTimeLeft()
{
	m_TimeLeft = 240.f;
}

void CWindowSpace::SetBulletLeft(int Bullet)
{
	m_BulletLeft->SetBulletLeft(Bullet);
}

void CWindowSpace::SetBombLeft(int Bomb)
{
	m_BombLeft->SetText(Bomb);
    m_BombLeft->SetColor(0.1f, EReactionChannel::InvincibleReaction);
}

void CWindowSpace::SetHPBar(int HP)
{
	m_HPBar->SetValue((float)HP / 3.f);
	m_HPBar->SetHitFlicker(true, 2.f);
}


void CWindowSpace::DisplayHUD()
{
	CWidgetImage* AmmoHUD = CreateWidget<CWidgetImage>("AmmoHUD");
	AmmoHUD->SetTexture("AmmoHUD");
	AmmoHUD->SetPos(73.f, 8.f);

	m_BulletLeft = CreateWidget<CWidgetTextImage>("BulletHUD");
	if (m_BulletLeft)
	{
		m_BulletLeft->SetTextImage("BasicFontGold");
		m_BulletLeft->SetPos(94.f, 16.f);
		m_BulletLeft->SetAlignMode(EAlignMode::Mid);
		m_BulletLeft->SetBulletLeft(0);
	}
		
	m_BombLeft = CreateWidget<CWidgetTextImage>("BombHUD");
	if (m_BombLeft)
	{
		m_BombLeft->SetTextImage("BasicFontGold");
		m_BombLeft->SetPos(122.f, 16.f);
		m_BombLeft->SetAlignMode(EAlignMode::Mid);
		m_BombLeft->SetText(10);
	}



	m_Scene->GetSceneResource()->LoadTexture("ProgressBarOut", TEXT("UI/ProgressBarOut.bmp"));
	m_Scene->GetSceneResource()->LoadTexture("ProgressBarIn", TEXT("UI/ProgressBarIn.bmp"), TEXTURE_PATH, EReactionChannel::HitReaction);
	m_HPBar = CreateWidget< CWidgetProgressBar>("Bar");
	if (m_HPBar)
	{
		m_HPBar->SetTexture(EProgressBar_TextureType::Back, "ProgressBarOut");
		m_HPBar->SetTexture(EProgressBar_TextureType::Bar, "ProgressBarIn");
		m_HPBar->SetPos(14.f, 16.f);
		m_HPBar->SetSkipCollisionCheck(true);
	}


	m_Score = CreateWidget<CWidgetTextImage>("ScoreHUD");
	if (m_Score)
	{
		m_Score->SetTextImage("BasicFontSilver");
		m_Score->SetPos(67.f, 8.f);
		m_Score->SetAlignMode(EAlignMode::Right);
		m_Score->SetScore(0);
	}

	m_TimeNumber = CreateWidget<CWidgetTextImage>("TimeLeft");
	if(m_TimeNumber)
	{
		m_TimeNumber->SetTextImage("TimeNumber");
		m_TimeNumber->SetKerning(0);
		m_TimeNumber->SetPos(136.f, 8.f);
		m_TimeNumber->SetText((int)m_TimeLeft / 4);
	}

	CWidgetTextImage* LifeHUD = CreateWidget<CWidgetTextImage>("LifeHUD");
	if (LifeHUD)
	{
		LifeHUD->SetTextImage("BasicFontGold");
		LifeHUD->SetPos(18.f, 24.f);
		LifeHUD->SetAlignMode(EAlignMode::Left);
		LifeHUD->SetText("1UP=0");
	}


}


