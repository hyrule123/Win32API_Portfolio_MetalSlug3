
#include "WidgetComponent.h"
#include "Widget.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "WidgetWindow.h"
#include "../GameObject/GameObject.h"

CWidgetComponent::CWidgetComponent():
    m_OwnerGameObj(nullptr)
{
	//게임오브젝트에 붙어서 출력되는 UI가 정보 표시 UI보다 위에 올라가면 안되므로 ZOrder = 0
	m_ZOrder = 0;
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& widget):
	CWidgetWindow(widget),
    m_OwnerGameObj(nullptr)
{
}

CWidgetComponent::~CWidgetComponent()
{
}



bool CWidgetComponent::Init()
{
	return true;
}

//충돌처리 안 함
bool CWidgetComponent::CollisionMouse(CWidget** WidgetAddr, const Vector2& Mousepos)
{
    return false;
}


void CWidgetComponent::Update(float DeltaTime)
{
    //가장 먼저 할것: 주인의 렌더링 위치를 UI상의 위치로 변환하기.
    m_Pos = m_OwnerGameObj->GetPos() - m_Scene->GetCamera()->GetPos();


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

void CWidgetComponent::PostUpdate(float DeltaTime)
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

        (*iter)->PostUpdate(DeltaTime);
        ++iter;
    }


    if (m_vecWidget.size() >= 2)
    {
        //메소드를 새로 만들기보다는 내림차순으로 정렬한 뒤 역순으로 순회해서 오름차순으로 렌더링
        std::sort(m_vecWidget.begin(), m_vecWidget.end(), CWidgetWindow::SortWidget);
    }

}

void CWidgetComponent::Render(HDC hDC, float DeltaTime)
{
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


