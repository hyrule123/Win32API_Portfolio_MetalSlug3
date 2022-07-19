#include "WidgetNumber.h"

//텍스처 로드용
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence.h"


#include "WidgetWindow.h"



CWidgetNumber::CWidgetNumber():
	m_Number(0),
	m_NumberImage(nullptr),
	m_Scale(1.f)
{
}

CWidgetNumber::CWidgetNumber(const CWidgetNumber& widget):
	CWidget(widget),
	m_NumberImage(widget.m_NumberImage),
	m_Scale(widget.m_Scale)
{
}

CWidgetNumber::~CWidgetNumber()
{
}

bool CWidgetNumber::Init()
{
	return true;
}


bool CWidgetNumber::SetNumberImage(const std::string& Name)
{
	m_NumberImage = CResourceManager::GetInst()->FindAnimationSequence(Name);

	if (!m_NumberImage)
		return false;

	m_Size = m_NumberImage->GetFrameData(0).End - m_NumberImage
		->GetFrameData(0).Start;

	return true;
}

void CWidgetNumber::SetNumber(int Num)
{
	m_Number = Num;
}

void CWidgetNumber::AddNumber(int Num)
{
	m_Number += Num;
}

void CWidgetNumber::SetScale(float Scale)
{
	m_Scale = Scale;
}


void CWidgetNumber::Update(float DeltaTime)
{
	int Num = m_Number;

	//10단위로 끝까지 나눠가면서 나머지를 한자리씩 스택에 삽입
	while (true)
	{
		m_NumberStack.push(Num % 10);
		Num /= 10;
		if (Num == 0)
			break;
	}


	m_NumberVec.clear();
	//그대로 다시 빼내면 올바른 순서로 벡터에 들어갈 것임.
	while (!m_NumberStack.empty())
	{
		m_NumberVec.push_back(m_NumberStack.top());
		m_NumberStack.pop();
	}


}

void CWidgetNumber::PostUpdate(float DeltaTime)
{
}

void CWidgetNumber::Render(HDC hDC, float DeltaTime)
{

	if (m_NumberImage)
	{
		size_t size = m_NumberVec.size();
		for (size_t i = 0; i < size; ++i)
		{
			//숫자가 출력될 좌표
			Vector2 RenderLT = m_Pos + m_Offset + m_Owner->GetPos();
			RenderLT.x += m_Size.x * m_Scale * (float)i;
			//숫자가 출력될 사이즈
			Vector2 RenderSize = m_Size * m_Scale;

			//출력해야 할 숫자
			int num = m_NumberVec[i];

			if (m_NumberImage->GetTextureType() == ETextureType::Sprite)
			{

				if (m_NumberImage->GetTexture()->GetColorKeyEnable())
				{
					TransparentBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)RenderSize.x, (int)RenderSize.y,
						m_NumberImage->GetTexture()->GetDC(),
						(int)m_NumberImage->GetFrameData(num).Start.x, (int)m_NumberImage->GetFrameData(num).Start.y,
						(int)m_Size.x, (int)m_Size.y,
						m_NumberImage->GetTexture()->GetColorKey());
				}
				else
				{
					StretchBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)RenderSize.x, (int)RenderSize.y,
						m_NumberImage->GetTexture()->GetDC(),
						(int)m_NumberImage->GetFrameData(num).Start.x, (int)m_NumberImage->GetFrameData(num).Start.y,
						(int)m_Size.x, (int)m_Size.y, 
						SRCCOPY);
				}
			}
			else//ETextureType::Frame
			{
				if (m_NumberImage->GetTexture()->GetColorKeyEnable(num))
				{
					TransparentBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)RenderSize.x, (int)RenderSize.y,
						m_NumberImage->GetTexture()->GetDC(num),
						(int)m_NumberImage->GetFrameData(num).Start.x, (int)m_NumberImage->GetFrameData(num).Start.y,
						(int)m_Size.x, (int)m_Size.y,
						m_NumberImage->GetTexture()->GetColorKey());
				}
				else
				{
					StretchBlt(hDC,
						(int)RenderLT.x, (int)RenderLT.y,
						(int)RenderSize.x, (int)RenderSize.y,
						m_NumberImage->GetTexture()->GetDC(num),
						(int)m_NumberImage->GetFrameData(num).Start.x, (int)m_NumberImage->GetFrameData(num).Start.y,
						(int)m_Size.x, (int)m_Size.y,
						SRCCOPY);
				}
			}
		}
	}
	else
	{
		Vector2 Pos = m_Pos + m_Offset + m_Owner->GetPos();
		char TimeText[64] = {};
		sprintf_s(TimeText, "%d(No NumberImage)", m_Number);
		TextOutA(hDC, (int)Pos.x, (int)Pos.y, TimeText, (int)strlen(TimeText));
	}
	
}
