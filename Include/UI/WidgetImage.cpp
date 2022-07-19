#include "WidgetImage.h"

//이미지 로드용
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"

CWidgetImage::CWidgetImage():
    m_Texture(nullptr),
    m_isRangeSet(false),
    m_isSizeSet(false)
{ 
    m_ZOrder = -1;
}

CWidgetImage::CWidgetImage(const CWidgetImage& widget) :
    CWidget(widget),
    m_Texture(widget.m_Texture),
    m_isRangeSet(widget.m_isRangeSet),
    m_ImageRenderRange(widget.m_ImageRenderRange),
    m_isSizeSet(widget.m_isSizeSet)
{
}

CWidgetImage::~CWidgetImage()
{
}

bool CWidgetImage::Init()
{
    return true;
}

void CWidgetImage::Start()
{
    m_Start = true;


    if (m_Texture)
    {
        Vector2 TSize = m_Texture->GetTextureSize();

         if (!m_isSizeSet)
         {
             m_Size = TSize;
         }

         if (!m_isRangeSet)
         {
             m_ImageRenderRange.LT.Set(0.f, 0.f);
             m_ImageRenderRange.RB = TSize;
         }
    }
    else
    {
        SetActive(false);
    }



}

void CWidgetImage::SetSize(float _x, float _y)
{
    m_isSizeSet = true;
    m_Size.x = _x;
    m_Size.y = _y;
}

void CWidgetImage::SetSize(const Vector2& Size)
{
    m_isSizeSet = true;
    m_Size = Size;
}





void CWidgetImage::SetRange(const Vector2& Start, const Vector2& End)
{
    m_ImageRenderRange.LT = Start;
    m_ImageRenderRange.RB = End;
    m_isRangeSet = true;
}

bool CWidgetImage::SetTexture(const std::string& Name)
{
    CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (!Texture)
        return false;

    m_Texture = Texture;
    return true;
}

void CWidgetImage::Update(float DeltaTime)
{
    CWidget::Update(DeltaTime);

    //범위를 지정하지 않으면 0,0부터 이미지 크기를 전체 출력한다.
    if (!m_isRangeSet)
    {
        m_ImageRenderRange.RB = m_Texture->GetTextureSize();
    }
    if (!m_isSizeSet)
    {
        m_Size = m_Texture->GetTextureSize();
    }
}

void CWidgetImage::PostUpdate(float DeltaTime)
{
    CWidget::PostUpdate(DeltaTime);
}

void CWidgetImage::Render(HDC hDC, float DeltaTime)
{
    CWidget::Render(hDC, DeltaTime);



    if (m_Texture)
    {

        Vector2 RenderLT = m_Pos + m_Owner->GetPos();
        Vector2 ImageRenderRangeSize = m_ImageRenderRange.RB - m_ImageRenderRange.LT;

        if (m_Texture->GetColorKeyEnable())
        {
            TransparentBlt(hDC,
                (int)RenderLT.x, (int)RenderLT.y,
                (int)m_Size.x, (int)m_Size.y,
                m_Texture->GetDC(),
                (int)m_ImageRenderRange.LT.x, (int)m_ImageRenderRange.LT.y,
                (int)ImageRenderRangeSize.x, (int)ImageRenderRangeSize.y,
                m_Texture->GetColorKey());

        }
        else
        {
            StretchBlt(hDC,
                (int)RenderLT.x, (int)RenderLT.y,
                (int)m_Size.x, (int)m_Size.y,
                m_Texture->GetDC(),
                (int)m_ImageRenderRange.LT.x, (int)m_ImageRenderRange.LT.y,
                (int)ImageRenderRangeSize.x, (int)ImageRenderRangeSize.y,
                SRCCOPY);
        }
    }
}


