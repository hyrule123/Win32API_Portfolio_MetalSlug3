#include "WidgetButton.h"


//�ڽ��� �� ���������� �����
#include "WidgetWindow.h"


//���ҽ� �ε���
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//���ҽ�
#include "../Resource/Sound/Sound.h"
#include "../Resource/Texture/Texture.h"

//�ؽ�ó ���� ��� ���� �׸����(hPen, hBrush)
#include "../GameManager.h"

//���콺 �Է� �޾ƿ���
#include "../Input.h"


CWidgetButton::CWidgetButton() :
    m_Texture(nullptr),
    m_SoundState{},
    m_ButtonStateData{},
    m_ButtonState(EButtonState::Normal)
{
}

CWidgetButton::CWidgetButton(const CWidgetButton& widget):
    CWidget(widget)
{
}

CWidgetButton::~CWidgetButton()
{
}

bool CWidgetButton::Init()
{
    return true;
}


//�ؽ�ó ����, �ؽ�ó�� �ε��� �� ����Ұ�.
bool CWidgetButton::SetTexture(const std::string& Name)
{
    CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (!Texture)
        return false;

    m_Texture = Texture;
    return true;
}
void CWidgetButton::SetButtonStateData(EButtonState State, const Vector2& Start, const Vector2& End)
{
    m_ButtonStateData[(int)State].Start = Start;
    m_ButtonStateData[(int)State].End = End;
}
void CWidgetButton::EnableButton(bool Enable)
{
    m_ButtonState = Enable ? EButtonState::Normal : EButtonState::Disabled;
}
//=================================================================================


//���� ����. ���带 �ε��ѵ� ����Ұ�
bool CWidgetButton::SetSound(EButtonSoundState State, const std::string& Name)
{
    CSound* Sound = m_Scene->GetSceneResource()->FindSound(Name);

    if (!Sound)
        return false;

    m_SoundState[(int)State] = Sound;
    return true;
}
//========================================================================



void CWidgetButton::Update(float DeltaTime)
{
    //��ư�� ������ = ���� ��ư�� ���¿� �ش��ϴ� �̹����� ũ��.
    if(m_Texture)
    { 
        //m_Size = m_ButtonStateData[(int)m_ButtonState].End - m_ButtonStateData[(int)m_ButtonState].Start;
    }

    //��ư ���°� Disabled�� �ƴ� ��쿡�� ó���Ѵ�.
    if (m_ButtonState != EButtonState::Disabled)
    {
        //��ǲ Ŭ���� �ּҸ� �̸� �޾Ƴ���
        CInput* Input = CInput::GetInst();

        //���콺�� �ش� ��ư ���� �ö������ ��
        if (m_MouseHovered)
        {
            //���� ��ư Ŭ���� �־��ٸ�
            if (Input->GetMouseLDown())
            {
                m_ButtonState = EButtonState::Click;
            }
            //���� ��� ������ �ִ� ���̶�� Ŭ������ ����
            else if (
                m_ButtonState == EButtonState::Click &&
                Input->GetMouseLPush()
                )
            {
                m_ButtonState = EButtonState::Click;
            }
            //Ŭ�� ���¿��� ��ư�� �ö� �� �����Ǿ��ٸ� �ݹ��Լ��� ȿ���� ȣ��
            else if (
                m_ButtonState == EButtonState::Click &&
                Input->GetMouseLUp()
                )
            {
                if (m_Callback[(int)EButtonSoundState::Click])
                    m_Callback[(int)EButtonSoundState::Click]();
                if (m_SoundState[(int)EButtonSoundState::Click])
                    m_SoundState[(int)EButtonSoundState::Click]->Play();

                //�׸��� ����� ȣ�������Ƿ� �ٽ� ���󺹱�(���콺 �ö�»���)
                m_ButtonState = EButtonState::MouseHovered;
            }

            //��ư Ŭ���� ������ �׳� �״�� ����
            else
            {
                m_ButtonState = EButtonState::MouseHovered;
            }
            

        }
        //���� �ƴϸ� �Ϲ� ���·� ����
        else
        {
            m_ButtonState = EButtonState::Normal;
        }
    }

    
}

void CWidgetButton::PostUpdate(float DeltaTime)
{
    
}

void CWidgetButton::Render(HDC hDC, float DeltaTime)
{

    if (m_Texture)
    {
        
        Vector2 RenderLT = m_Pos + m_Owner->GetPos();

        int index = (int)m_ButtonState;

        Vector2 TextureSize = m_ButtonStateData[index].End - m_ButtonStateData[index].Start;

        if (m_Texture->GetColorKeyEnable())
        {
            TransparentBlt(hDC,
                (int)RenderLT.x, (int)RenderLT.y,
                (int)m_Size.x, (int)m_Size.y,
                m_Texture->GetDC(), 
                (int)m_ButtonStateData[index].Start.x, (int)m_ButtonStateData[index].Start.y,
                (int)TextureSize.x, (int)TextureSize.y,
                m_Texture->GetColorKey());

        }
        else
        {
            StretchBlt(hDC,
                (int)RenderLT.x, (int)RenderLT.y,
                (int)m_Size.x, (int)m_Size.y,
                m_Texture->GetDC(), 
                (int)m_ButtonStateData[index].Start.x, (int)m_ButtonStateData[index].Start.y,
                (int)TextureSize.x, (int)TextureSize.y,
                SRCCOPY);
        }
    }
    else //�ؽ�ó�� ������
    {
        HBRUSH Brush = CGameManager::GetInst()->GetBrush(EBrushType::Green);

        //���콺�� �ö�������� ���������� ��ȯ
        if (m_ButtonState == EButtonState::MouseHovered)
        {
            Brush = CGameManager::GetInst()->GetBrush(EBrushType::Red);
        }
        //Ŭ���ϸ� blue
        else if (m_ButtonState == EButtonState::Click)
        {
            Brush = CGameManager::GetInst()->GetBrush(EBrushType::Blue);
        }
        //��Ȱ��ȭ �ÿ��� ����
        else if (m_ButtonState == EButtonState::Disabled)
        {
            Brush = CGameManager::GetInst()->GetBrush(EBrushType::Black);
        }

        HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, Brush);


        //WidgetWindow�� ��ġ�� ���� ǥ�õ� ��� ��ġ�� ���Ѵ�.
        Vector2 PosLT = m_Pos + m_Owner->GetPos();

        Rectangle(hDC,
            (int)PosLT.x,
            (int)PosLT.y,
            (int)(PosLT.x + m_Size.x),
            (int)(PosLT.y + m_Size.y));

        SelectObject(hDC, PrevBrush);
    }
}

void CWidgetButton::Render(HDC hDC, const Vector2& Pos, float DeltaTime)
{
}

void CWidgetButton::CollisionMouseHoveredCallback(const Vector2& Pos)
{
    CWidget::CollisionMouseHoveredCallback(Pos);

    if (m_SoundState[(int)EButtonSoundState::MouseHovered])
    {
        m_SoundState[(int)EButtonSoundState::MouseHovered]->Play();
    }

}

void CWidgetButton::CollisionMouseReleaseCallback()
{
    CWidget::CollisionMouseReleaseCallback();
}
