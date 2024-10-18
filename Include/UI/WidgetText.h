#pragma once
#include "Widget.h"
class CWidgetText :
	public CWidget
{
	friend class CWidgetComponent;
	friend class CWidgetWindow;

protected:
	CWidgetText();
	CWidgetText(const CWidgetText& widget);
	virtual ~CWidgetText();
public:
	virtual bool Init();


protected://출력할 텍스트 저장
	TCHAR* m_Text;
	int    m_Count;
	int    m_Capacity;
public:
	void SetText(const std::tstring& Text);
	void AddText(const TCHAR Text);
	void AddText(const std::tstring& Text);
	void pop_back();
	void clear();


protected://텍스트 색 지정
	COLORREF    m_TextColor;
public:
	void SetColor(unsigned char r, unsigned char g, unsigned char b);


protected://폰트 지정
	CSharedPtr<class CFont> m_Font;
public:
	void SetFont(const std::string& Name);


protected://한 글자씩 출력할것인지
	bool m_OneLetterAtTime;
	float m_Interval; //출력 시간 간격
	float m_TimeCount;
public:
	void SetOneLetterAtTime(bool Enable = true);
	void SetInterval(float Interval);


protected://그림자 사용 여부 / 색상, 떨어져 있을 거리지정
	bool        m_Shadow;
	COLORREF    m_ShadowColor;
	Vector2     m_ShadowOffset;
public:
	void SetTextShadowColor(unsigned char r, unsigned char g, unsigned char b);
	void EnableShadow(bool Shadow = true);
	void SetShadowOffset(float x, float y);



public:
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);
};

