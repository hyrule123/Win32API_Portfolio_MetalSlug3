
#include "WidgetProgressBar.h"
#include "WidgetWindow.h"

//�ؽ�ó �ε�
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"


//HBRUSH �������� �뵵
#include "../GameManager.h"

CWidgetProgressBar::CWidgetProgressBar()	:
	m_Dir(EProgressBar_Dir::LeftToRight),
	m_Value(1.f),
	m_Padding(1.f),
	m_isSizeSet(),
	m_PrevValue(),
	m_HitFlickerOn(),
	m_ReactionTime(),
	m_FlickerFrequency(),
	m_Flicker(),
	m_Reaction()
{
}

CWidgetProgressBar::CWidgetProgressBar(const CWidgetProgressBar& widget) :
	CWidget(widget),
	m_Value(widget.m_Value),
	m_Padding(widget.m_Padding),
	m_isSizeSet(widget.m_isSizeSet),
	m_PrevValue(widget.m_PrevValue),
	m_HitFlickerOn(widget.m_HitFlickerOn),
	m_ReactionTime(widget.m_ReactionTime),
	m_FlickerFrequency(widget.m_FlickerFrequency),
	m_Flicker(widget.m_Flicker),
	m_Reaction(widget.m_Reaction)
{
}

CWidgetProgressBar::~CWidgetProgressBar()
{
}



bool CWidgetProgressBar::Init()
{
	return true;
}

void CWidgetProgressBar::Start()
{
	m_Start = true;

	if (!m_isSizeSet)
	{
		m_Size = m_Texture[(int)EProgressBar_TextureType::Back]->GetTextureSize();
	}
}



void CWidgetProgressBar::SetBarFrameSize(float SizeX, float SizeY)
{
	m_isSizeSet = true;

	m_Size.x = SizeX;
	m_Size.y = SizeY;

	//�е��� �⺻ 10���� ��������.
	SetPadding(1.f);
}

void CWidgetProgressBar::SetBarFrameSize(const Vector2& Size)
{
	m_isSizeSet = true;

	m_Size = Size;

	SetPadding(1.f);
}

bool CWidgetProgressBar::SetHitFlicker(bool On, float Duration)
{


	if (On)
	{
		if (m_Texture[(int)EProgressBar_TextureType::Bar]->GetMaxReactionChannel() <= 1)
		{
			m_HitFlickerOn = false;
			return false;
		}


		m_ReactionTime = Duration;
		m_Reaction = EReactionChannel::HitReaction;

	}

	m_HitFlickerOn = On;
	return true;
}

void CWidgetProgressBar::SetPadding(float Padding)
{
	//x�� y �� ª�� ���� ������ �����Ͱ� ��.
	float ShortArea = (m_Size.x > m_Size.y ? m_Size.y : m_Size.x) / 2.f;
	m_Padding = Padding;

	//���� �е��� ���̰� ª�� ���� �ݺ��� ��� ������ �е����� �ٿ���
	if (m_Padding > ShortArea)
	{
		m_Padding = ShortArea - 1.f;
	}
}

bool CWidgetProgressBar::SetTexture(EProgressBar_TextureType Type, const std::string& Name)
{
	CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (!Texture)
		return false;

	m_Texture[(int)Type] = Texture;
	return true;
}

void CWidgetProgressBar::SetBarDir(EProgressBar_Dir Dir)
{
	m_Dir = Dir;
}

void CWidgetProgressBar::SetValue(float Value)
{
	m_Value = Value;

	if (m_Value > 1.f)
		m_Value = 1.f;

	else if (m_Value < 0.f)
		m_Value = 0.f;
}

void CWidgetProgressBar::AddValue(float Value)
{
	m_Value += Value;

	if (m_Value > 1.f)
		m_Value = 1.f;

	else if (m_Value < 0.f)
		m_Value = 0.f;
}




void CWidgetProgressBar::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);


	//���� ����� �ʱ�ȭ�ϰ�
	m_BarPos.LT = m_Pos + m_Padding;
	m_BarPos.RB = m_Pos + m_Size - m_Padding;
	Vector2 m_BarSize = (m_BarPos.RB - m_BarPos.LT) * m_Value;

	//�׷��� ���� �����Ѵ�.
	switch (m_Dir)
	{
	case EProgressBar_Dir::LeftToRight:
		m_BarPos.RB.x = m_BarPos.LT.x + m_BarSize.x;
		break;
	case EProgressBar_Dir::RightToLeft:
		m_BarPos.LT.x = m_BarPos.RB.x - m_BarSize.x;
		break;
	case EProgressBar_Dir::BottomToTop:
		m_BarPos.LT.y = m_BarPos.RB.y - m_BarSize.y;
		break;
	case EProgressBar_Dir::TopToBottom:
		m_BarPos.RB.y = m_BarPos.LT.y + m_BarSize.y;
		break;
	}
}

void CWidgetProgressBar::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);


	m_PrevValue = m_Value;
}

void CWidgetProgressBar::Render(HDC hDC, float DeltaTime)
{
	CWidget::Render(hDC, DeltaTime);

	//���� ���� ���
	if (m_ReactionTime > 0.f)
	{
		m_ReactionTime -= DeltaTime;
		if (m_HitFlickerOn)
		{
			m_FlickerFrequency -= DeltaTime;

			if (m_FlickerFrequency < 0.f)
			{
				m_Flicker = !m_Flicker;
				m_FlickerFrequency = 0.05f;
			}
		}
		else
			m_Flicker = false;
	}
	else
	{
		m_Flicker = false;
		m_Reaction = EReactionChannel::Normal;
	}

	for (int i = 0; i < (int)EProgressBar_TextureType::End; ++i)
	{
		Vector2	RenderPos;
		Vector2 RenderSize;

		//���� ���� Ȯ��. �� ���׼� ����
		int ReactionNum = 0;
		if (!m_Flicker)
		{
			ReactionNum = (int)m_Reaction;
		}

		switch ((EProgressBar_TextureType)i)
		{
		case EProgressBar_TextureType::Back:
			RenderPos = m_Pos;
			RenderSize = m_Size;
			ReactionNum = 0;
			break;
		case EProgressBar_TextureType::Bar:
			RenderPos = m_BarPos.LT;
			RenderSize = m_BarPos.RB - m_BarPos.LT;
			break;

		}

		RenderPos += m_Owner->GetPos() + m_Offset;


		if (m_Texture[i])
		{
			Vector2 TexSize = m_Texture[i]->GetTextureSize();

			if (m_Texture[i]->GetTextureType() == ETextureType::Sprite)
			{


				if (m_Texture[i]->GetColorKeyEnable())
				{
					TransparentBlt(hDC, 
						(int)RenderPos.x, (int)RenderPos.y,
						(int)RenderSize.x, (int)RenderSize.y,
						m_Texture[i]->GetDC(ReactionNum),
						0, 0, 
						(int)TexSize.x, (int)TexSize.y,
						m_Texture[i]->GetColorKey(ReactionNum));
				}

				else
				{
					StretchBlt(hDC,
						(int)RenderPos.x, (int)RenderPos.y,
						(int)RenderSize.x, (int)RenderSize.y,
						m_Texture[i]->GetDC(ReactionNum),
						0, 0,
						(int)TexSize.x, (int)TexSize.y,
						SRCCOPY);
				}
			}

			else//�������� ���� ó������ ����
			{
			}
		}

		else//�ؽ�ó �������
		{
			//Ʋ�� �����
			HBRUSH Brush = CGameManager::GetInst()->GetBrush(EBrushType::Yellow);

			//�ٴ� ������
			if (i == (int)EProgressBar_TextureType::Bar)
			{
				Brush = CGameManager::GetInst()->GetBrush(EBrushType::Red);
			}

			HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, Brush);

			Rectangle(hDC,
				(int)RenderPos.x,
				(int)RenderPos.y,
				(int)(RenderPos.x + RenderSize.x),
				(int)(RenderPos.y + RenderSize.y));

			SelectObject(hDC, PrevBrush);

		}
	}
}

void CWidgetProgressBar::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
}
