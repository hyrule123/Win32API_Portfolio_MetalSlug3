#pragma once
#include "Widget.h"


class CWidgetImage :
	public CWidget
{

	friend class CWidgetWindow;

protected:
	CWidgetImage();
	CWidgetImage(const CWidgetImage& widget);
	virtual ~CWidgetImage();
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);



protected://텍스처
	CSharedPtr<class CTexture>  m_Texture;


protected:
	bool m_isSizeSet;
public:
	void SetSize(float _x, float _y);
	void SetSize(const Vector2& Size);



	//원본 이미지의 어디부터 어디까지 출력할것인지 설정
	BoxInfo m_ImageRenderRange;
	//위의값을 설정하지 않았다면 그냥 이미지 size만큼 출력
	bool m_isRangeSet;
public:
	void SetRange(const Vector2& Start, const Vector2& End);

	//텍스처 설정 - 텍스처 로딩 먼저 한뒤 사용할것.
	bool SetTexture(const std::string& Name);




};

