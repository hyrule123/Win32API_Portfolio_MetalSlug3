#include "WidgetWindow.h"
#include "Widget.h"
#include "WidgetFadeInOut.h"

//일반 위젯 윈도우의 Z오더는 2
CWidgetWindow::CWidgetWindow() :
    m_Scene(nullptr),
    m_ZOrder(2),
    m_CheckCollide()
{
}

CWidgetWindow::CWidgetWindow(const CWidgetWindow& Obj):
    CRef(Obj),
    m_Scene(Obj.m_Scene),
    m_ZOrder(Obj.m_ZOrder),
    m_Size(Obj.m_Size),
    m_Pos(Obj.m_Pos),
    m_CheckCollide(Obj.m_CheckCollide)
{
    //size_t size = Obj.m_vecWidget.size();
    //for (size_t i = 0; i < size; ++i)
    //{
    //    Obj.m_vecWidget[i]()
    //}
}

CWidgetWindow::~CWidgetWindow()
{


}

bool CWidgetWindow::Init()
{
    m_Size = Vector2(ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT) * SCREEN_SCALE;


    return true;
}

CWidgetFadeInOut* CWidgetWindow::FadeInOut(bool isFadeIn)
{
    CWidgetFadeInOut* FadeIn = CreateWidget<CWidgetFadeInOut>("Fade");
    FadeIn->SetEssential(isFadeIn);

    return FadeIn;
}



CWidget* CWidgetWindow::GetWidget(int Index)
{
    return m_vecWidget[Index];
}

int CWidgetWindow::GetWidgetCount() const
{
    return (int)m_vecWidget.size();
}

const Vector2& CWidgetWindow::GetPos() const
{
    return m_Pos;
}

const Vector2& CWidgetWindow::GetSize() const
{
    return m_Size;
}

bool CWidgetWindow::CollisionMouse(CWidget** WidgetAddr, const Vector2& Mousepos)
{
    Vector2 WindowLT = m_Pos;


    if (Mousepos.x < WindowLT.x)
        return false;
    if(Mousepos.x > (WindowLT.x + m_Size.x))
        return false;
    if (Mousepos.y < WindowLT.y)
        return false;
    if(Mousepos.y > (WindowLT.y + m_Size.y))
        return false;


    //해당 위젯 윈도우가 충돌일 경우 정확히 어떤 위젯과 충돌 중인지를 확인.
    size_t size = m_vecWidget.size();
    for (size_t i = 0; i < size; ++i)
    {
        //버튼이 활성화 되어있을때에만 검사
        if (!m_vecWidget[i]->GetEnable())
            continue;

        if (m_vecWidget[i]->CollisionMouse(Mousepos))
        {
            //충돌인 위젯이 있을 경우 들어온 WidgetAddr 인자에 해당 위젯의 주소를 넣고 반환.
            *WidgetAddr = m_vecWidget[i];
            return true;
        }

    }


    return false;
}

bool CWidgetWindow::SortWidget(const CSharedPtr<CWidget>& Src, const CSharedPtr<CWidget>& Dest)
{
    return Src->GetZOrder() > Dest->GetZOrder();
}


int CWidgetWindow::GetZOrder() const
{
    return m_ZOrder;
}

void CWidgetWindow::SetPos(float x, float y)
{
    m_Pos.x = x;
    m_Pos.y = y;
}

void CWidgetWindow::SetPos(const Vector2& Pos)
{
    m_Pos = Pos;
}

void CWidgetWindow::SetSize(float x, float y)
{
    m_Size.x = x;
    m_Size.y = y;
}

void CWidgetWindow::SetSize(const Vector2& Size)
{
    m_Size = Size;
}

void CWidgetWindow::SetCheckCollide(bool CheckCollide)
{
    m_CheckCollide = CheckCollide;
}

bool CWidgetWindow::GetCheckCollide() const
{
    return m_CheckCollide;
}

void CWidgetWindow::SetZOrder(int ZOrder)
{
    m_ZOrder = ZOrder;
}


void CWidgetWindow::Update(float DeltaTime)
{
    auto iter = m_vecWidget.begin();
    auto iterEnd = m_vecWidget.end();

    while (iter != iterEnd)
    {
        if (!(*iter)->GetActive())
        {
            iter = m_vecWidget.erase(iter);
            iterEnd = m_vecWidget.end();
        }
        else if (!(*iter)->GetEnable()) 
        {
            ++iter;
            continue;
        }

        (*iter)->Update(DeltaTime);
        ++iter;
    }


}

void CWidgetWindow::PostUpdate(float DeltaTime)
{
    auto iter = m_vecWidget.begin();
    auto iterEnd = m_vecWidget.end();

    while (iter != iterEnd)
    {
        if (!(*iter)->GetActive())
        {
            iter = m_vecWidget.erase(iter);
            iterEnd = m_vecWidget.end();
            continue;
        }
        else if (!(*iter)->GetEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->PostUpdate(DeltaTime);
        ++iter;
    }

    //사이즈가 2 이상이면 내림차순으로 sort 해준다.(충돌검사 = 내림차순)
    //이 PostUpdate 과정이 끝나고
    //전체 충돌검사를 하므로 지금 정렬하면 된다.
    if(m_vecWidget.size() >= 2)
    { 
        std::sort(m_vecWidget.begin(), m_vecWidget.end(), CWidgetWindow::SortWidget);
    }


}

void CWidgetWindow::Render(HDC hDC, float DeltaTime)
{
    //위에서 내림차순으로 정렬했으므로 
    //역순으로 순회돌면 오름차순으로 렌더링될 것이다.
    auto iter = m_vecWidget.rbegin();
    auto iterEnd = m_vecWidget.rend();

    while (iter != iterEnd)
    {
        if (!(*iter)->GetActive())
        {
            ++iter;
            continue;
        }
        else if (!(*iter)->GetEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->Render(hDC, DeltaTime);
        ++iter;
    }
}

void CWidgetWindow::DeleteWidget(const std::string& Name)
{
    CWidget* Widget = FindWidgetCommonType(Name);
    if (Widget)
    {
        Widget->SetActive(false);
    }
}

CWidget* CWidgetWindow::FindWidgetCommonType(const std::string& Name)
{
    auto iter = m_vecWidget.begin();
    auto iterEnd = m_vecWidget.end();

    while (iter != iterEnd)
    {
        if ((*iter)->GetName() == Name)
        {
            return (*iter);
        }

        ++iter;
    }

    return nullptr;
}
