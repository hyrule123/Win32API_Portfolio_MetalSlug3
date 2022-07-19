
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
	//���ӿ�����Ʈ�� �پ ��µǴ� UI�� ���� ǥ�� UI���� ���� �ö󰡸� �ȵǹǷ� ZOrder = 0
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

//�浹ó�� �� ��
bool CWidgetComponent::CollisionMouse(CWidget** WidgetAddr, const Vector2& Mousepos)
{
    return false;
}


void CWidgetComponent::Update(float DeltaTime)
{
    //���� ���� �Ұ�: ������ ������ ��ġ�� UI���� ��ġ�� ��ȯ�ϱ�.
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
        //�޼ҵ带 ���� ����⺸�ٴ� ������������ ������ �� �������� ��ȸ�ؼ� ������������ ������
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


