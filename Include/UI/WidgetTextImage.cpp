#include "WidgetTextImage.h"

//텍스처 로드용
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence.h"


#include "WidgetWindow.h"



CWidgetTextImage::CWidgetTextImage() :
	m_Text{},
	m_TextImage(nullptr),
	m_Scale(1.f),
	m_Kerning(1),
	m_vecCharWidth{},
	m_RenderPosX(),
	m_Flicker(),
	m_FlickerDuration(),
	m_FlickerSwitch(),
	m_AlignMode(EAlignMode::Left),
	m_ColorFlicker(EReactionChannel::Normal)
{
}

CWidgetTextImage::CWidgetTextImage(const CWidgetTextImage& widget) :
	CWidget(widget),
	m_Text(widget.m_Text),
	m_TextImage(widget.m_TextImage),
	m_Scale(widget.m_Scale),
	m_Kerning(widget.m_Kerning),
	m_vecCharWidth{},
	m_RenderPosX(widget.m_RenderPosX),
	m_Flicker(widget.m_Flicker),
	m_FlickerDuration(widget.m_FlickerDuration),
	m_FlickerSwitch(widget.m_FlickerSwitch),
	m_AlignMode(widget.m_AlignMode),
	m_ColorFlicker(widget.m_ColorFlicker)
{
}

CWidgetTextImage::~CWidgetTextImage()
{
}

bool CWidgetTextImage::Init()
{
	return true;
}


bool CWidgetTextImage::SetTextImage(const std::string& Name)
{
	m_TextImage = CResourceManager::GetInst()->FindAnimationSequence(Name);

	if (!m_TextImage)
		return false;

	std::vector<Sprite_FrameData> FrameData = m_TextImage->GetFrameDataAll();

	size_t Size = FrameData.size();
	if (Size < 'Z')
		return false;

	for (int i = 0; i < Size; ++i)
	{
		m_vecCharWidth.resize(Size);
		m_vecCharWidth[i] = (INT16)FrameData[i].End.x - (INT16)FrameData[i].Start.x;
	}

	m_Size.y = FrameData['A'].End.y - FrameData['A'].Start.y;

	return true;
}

void CWidgetTextImage::SetText(const std::string& Txt)
{
	m_Text = Txt;

	m_RenderPosX = 0;
	CalcTextLen();
}

void CWidgetTextImage::AddText(const std::string& Txt)
{
	m_Text += Txt;


	CalcTextLen();
}

void CWidgetTextImage::SetText(int Num)
{
	m_Text = std::to_string(Num);


	m_RenderPosX = 0;
	CalcTextLen();
}

void CWidgetTextImage::AddText(int Num)
{
	int CurrentNum = std::stoi(m_Text);
	CurrentNum += Num;

	SetText(CurrentNum);
}

void CWidgetTextImage::SetBulletLeft(int Num)
{
	if (Num <= 0)
		m_Text = "#";
	else
	{
		m_Text = std::to_string(Num);
		SetColor(0.1f, EReactionChannel::InvincibleReaction);
	}
		

	m_RenderPosX = 0;
	
	CalcTextLen();
}

void CWidgetTextImage::SetScore(int Score)
{
	m_Text = std::to_string(Score);

	size_t n_zero = 6;

	m_Text = std::string(n_zero - min(n_zero, m_Text.length()), '0') + m_Text;

	m_RenderPosX = 0;
	CalcTextLen();
}

void CWidgetTextImage::SetScale(float Scale)
{
	m_Scale = Scale;
}

void CWidgetTextImage::CalcTextLen()
{
	//우선 가장 끝부분을 계산해서 끝부분부터 글자를 그려나가야한다.(왼쪽정렬)
	int size = (int)m_Text.size();
	for (int i = size - 1; i >= 0; --i)
	{
		m_RenderPosX += (int)roundf((m_vecCharWidth[m_Text[i]]) * m_Scale);
	}

	m_RenderPosX += m_Kerning * (size - 2);
}

void CWidgetTextImage::SetFlicker(float Duration, float Frequency)
{
	m_FlickerDuration.InitCooltime(Duration);
	m_FlickerDuration.EnterCooltime();

	m_Flicker.InitCooltime(Frequency);

}

void CWidgetTextImage::SetColorFlicker(float Duration, float Frequency, EReactionChannel ColorFlicker)
{
	m_FlickerDuration.InitCooltime(Duration);
	m_FlickerDuration.EnterCooltime();

	m_Flicker.InitCooltime(Frequency);

	m_ColorFlicker = ColorFlicker;
}

void CWidgetTextImage::SetColor(float Duration, EReactionChannel ColorFlicker)
{
	m_FlickerDuration.InitCooltime(Duration);
	m_FlickerDuration.EnterCooltime();

	m_Flicker.InitCooltime(100.f);
	m_Flicker.EnterCooltime();

	m_FlickerSwitch = true;
	m_ColorFlicker = ColorFlicker;
}

bool CWidgetTextImage::GetFlickerDone() const
{
	return !m_FlickerDuration.isCooltime;
}



void CWidgetTextImage::SetKerning(INT8 Kerning)
{
	m_Kerning = Kerning;
}

void CWidgetTextImage::SetAlignMode(EAlignMode AlignMode)
{
	m_AlignMode = AlignMode;
}

void CWidgetTextImage::Update(float DeltaTime)
{
	
	m_FlickerDuration.UpdateCooltime(DeltaTime);
	

	if (m_FlickerDuration.isCooltime)
	{
		m_Flicker.UpdateCooltime(DeltaTime);

		if (m_Flicker.EnterCooltime())
		{
			m_FlickerSwitch = !m_FlickerSwitch;
		}
	}
	else
	{
		m_ColorFlicker = EReactionChannel::Normal;
		m_FlickerSwitch = false;
	}
}

void CWidgetTextImage::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CWidgetTextImage::Render(HDC hDC, float DeltaTime)
{
	if (m_FlickerSwitch && m_ColorFlicker == EReactionChannel::Normal)
		return;

	//숫자가 출력될 좌표
	Vector2 RenderLT = m_Pos + m_Offset + m_Owner->GetPos();
	if (m_TextImage)
	{
		int size = (int)m_Text.size();

		switch (m_AlignMode)
		{
		case EAlignMode::Left:
			RenderLT.x += m_RenderPosX;
			break;
		case EAlignMode::Mid:
			RenderLT.x += m_RenderPosX / 2;
			break;
		case EAlignMode::Right:
			break;
		}
		

		for (int i = size -1 ; i >= 0; --i)
		{
			//해당 숫자의 아스키 코드로 텍스처의 부분을 불러온다.
			char AsciiCode = m_Text[i];

			//해당 글자의 폭과 자간만큼 숫자를 뺴준다.
			RenderLT.x -= (m_vecCharWidth[AsciiCode] + m_Kerning);


			if (m_TextImage->GetTextureType() == ETextureType::Sprite)
			{


				int NowColor = 0;	//(int)EReactionChannel::Normal

				//변경된 색깔이 로딩되어있지 않으면 변경 불가능
				if (m_FlickerSwitch && m_TextImage->GetTexture()->GetTextureNumbers() > (int)m_ColorFlicker)
				{
					NowColor = (int)m_ColorFlicker;
				}



				if (m_TextImage->GetTexture()->GetColorKeyEnable())
				{
					TransparentBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						m_TextImage->GetTexture()->GetDC(NowColor),
						(int)m_TextImage->GetFrameData(AsciiCode).Start.x, (int)m_TextImage->GetFrameData(AsciiCode).Start.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						m_TextImage->GetTexture()->GetColorKey(NowColor));
				}
				else
				{
					StretchBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						m_TextImage->GetTexture()->GetDC(NowColor),
						(int)m_TextImage->GetFrameData(AsciiCode).Start.x, (int)m_TextImage->GetFrameData(AsciiCode).Start.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						SRCCOPY);
				}
			}
			else//ETextureType::Frame
			{
				if (m_TextImage->GetTexture()->GetColorKeyEnable(AsciiCode))
				{
					TransparentBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						m_TextImage->GetTexture()->GetDC(),
						(int)m_TextImage->GetFrameData(AsciiCode).Start.x, (int)m_TextImage->GetFrameData(AsciiCode).Start.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						m_TextImage->GetTexture()->GetColorKey());



				}
				else
				{
					StretchBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						m_TextImage->GetTexture()->GetDC(),
						(int)m_TextImage->GetFrameData(AsciiCode).Start.x, (int)m_TextImage->GetFrameData(AsciiCode).Start.y,
						(int)m_vecCharWidth[AsciiCode], (int)m_Size.y,
						SRCCOPY);
				}
			}


		}
	}


}
