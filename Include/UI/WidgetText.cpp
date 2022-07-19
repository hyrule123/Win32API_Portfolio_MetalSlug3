
#include "WidgetText.h"
#include "../Resource/Font/Font.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "WidgetWindow.h"

CWidgetText::CWidgetText():
	m_OneLetterAtTime(false),
	m_Interval(0.1f),
	m_TimeCount(0.f)
{
	m_Count = 0;
	m_Capacity = 32;

	m_Text = new TCHAR[m_Capacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_Capacity);

	m_TextColor = RGB(0, 0, 0);
	m_ShadowColor = RGB(30, 30, 30);
	m_Shadow = false;
	m_ShadowOffset = Vector2(1.f, 1.f);
}

CWidgetText::CWidgetText(const CWidgetText& widget) :
	CWidget(widget)
{
}

CWidgetText::~CWidgetText()
{
	SAFE_DELETE_ARRAY(m_Text);
}

void CWidgetText::SetFont(const std::string& Name)
{
	m_Font = m_Scene->GetSceneResource()->FindFont(Name);
}

void CWidgetText::SetOneLetterAtTime(bool Enable)
{
	m_OneLetterAtTime = Enable;
}

void CWidgetText::SetInterval(float Interval)
{
	m_Interval = Interval;
}

bool CWidgetText::Init()
{
	m_Font = m_Scene->GetSceneResource()->FindFont("DefaultFont");

	return true;
}

void CWidgetText::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_TextColor = RGB(r, g, b);
}

void CWidgetText::SetTextShadowColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_ShadowColor = RGB(r, g, b);
}

void CWidgetText::EnableShadow(bool Shadow)
{
	m_Shadow = Shadow;
}

void CWidgetText::SetShadowOffset(float x, float y)
{
	m_ShadowOffset = Vector2(x, y);
}
void CWidgetText::SetText(const std::tstring& Text)
{
	int Length = lstrlen(Text.c_str());

	if (Length + 1 > m_Capacity)
	{
		m_Capacity = Length + 1;

		SAFE_DELETE_ARRAY(m_Text);

		m_Text = new TCHAR[m_Capacity];
		memset(m_Text, 0, sizeof(TCHAR) * m_Capacity);
	}

	lstrcpy(m_Text, Text.c_str());

	m_Count = lstrlen(m_Text);
}

void CWidgetText::AddText(const TCHAR Text)
{
	int Length = m_Count + 1;

	if (Length + 1 > m_Capacity)
	{
		m_Capacity = Length + 1;

		TCHAR* NewText = new TCHAR[m_Capacity];
		memset(NewText, 0, sizeof(TCHAR) * m_Capacity);

		lstrcpy(NewText, m_Text);

		SAFE_DELETE_ARRAY(m_Text);

		m_Text = NewText;
	}

	m_Text[m_Count] = Text;

	++m_Count;
}
void CWidgetText::AddText(const std::tstring& Text)
{
	int Length = m_Count + lstrlen(Text.c_str());

	if (Length + 1 > m_Capacity)
	{
		m_Capacity = Length + 1;

		TCHAR* NewText = new TCHAR[m_Capacity];
		memset(NewText, 0, sizeof(TCHAR) * m_Capacity);

		lstrcpy(NewText, m_Text);

		SAFE_DELETE_ARRAY(m_Text);

		m_Text = NewText;
	}

	lstrcat(m_Text, Text.c_str());

	m_Count += lstrlen(Text.c_str());
}

void CWidgetText::pop_back()
{
	--m_Count;

	m_Text[m_Count] = 0;
}

void CWidgetText::clear()
{
	m_Count = 0;

	m_Text[m_Count] = 0;
}

void CWidgetText::Update(float DeltaTime)
{
	if (m_OneLetterAtTime)
	{
		m_TimeCount += DeltaTime;
	}
}

void CWidgetText::PostUpdate(float DeltaTime)
{
}

void CWidgetText::Render(HDC hDC, float DeltaTime)
{
	int CharCount = m_Count;
	if (m_OneLetterAtTime)
	{
		//�Ҽ��� �� ������ int�� ����ȯ
		CharCount = (int)(m_TimeCount / m_Interval);

		//���� �ִ� �ڸ������� ����� �Ϸ������
		if (CharCount > m_Count)
		{
			//CharCount�� ���� ������ ���� ���� �ѱ��ھ� ��� ��带 �����Ѵ�
			CharCount = m_Count;
			m_OneLetterAtTime = false;
		}
	}



	m_Font->SetFont(hDC);

	Vector2	RenderPos = m_Pos + m_Owner->GetPos();

	SetBkMode(hDC, TRANSPARENT);

	// �׸��ڸ� ����ؾ� �Ѵٸ� �׸��� ���� ����Ѵ�.
	if (m_Shadow)
	{
		Vector2	ShadowPos = RenderPos + m_ShadowOffset;

		::SetTextColor(hDC, m_ShadowColor);
		TextOut(hDC, (int)ShadowPos.x, (int)ShadowPos.y, m_Text, CharCount);
	}

	// ����Լ��� �ƴ� ���� �̸��� �����Լ��� ȣ���ϰ��� �Ѵٸ� �տ� :: �� �ٿ��� ȣ���Ѵ�.
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)RenderPos.x, (int)RenderPos.y, m_Text, CharCount);

	m_Font->ResetFont(hDC);
}

void CWidgetText::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
	int CharCount = m_Count;
	if (m_OneLetterAtTime)
	{
		//�Ҽ��� �� ������ int�� ����ȯ
		CharCount = (int)(m_TimeCount / m_Interval);

		//���� �ִ� �ڸ������� ����� �Ϸ������
		if (CharCount > m_Count)
		{
			//CharCount�� ���� ������ ���� ���� �ѱ��ھ� ��� ��带 �����Ѵ�
			CharCount = m_Count;
			m_OneLetterAtTime = false;
		}
	}



	m_Font->SetFont(hDC);

	Vector2	RenderPos = Pos;

	SetBkMode(hDC, TRANSPARENT);

	// �׸��ڸ� ����ؾ� �Ѵٸ� �׸��� ���� ����Ѵ�.
	if (m_Shadow)
	{
		Vector2	ShadowPos = RenderPos + m_ShadowOffset;

		::SetTextColor(hDC, m_ShadowColor);
		TextOut(hDC, (int)ShadowPos.x, (int)ShadowPos.y, m_Text, CharCount);
	}

	// ����Լ��� �ƴ� ���� �̸��� �����Լ��� ȣ���ϰ��� �Ѵٸ� �տ� :: �� �ٿ��� ȣ���Ѵ�.
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)RenderPos.x, (int)RenderPos.y, m_Text, CharCount);


	m_Font->ResetFont(hDC);
	
}

