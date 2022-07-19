#pragma once
#include "Widget.h"


class CWidgetButton :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CWidgetButton();
    CWidgetButton(const CWidgetButton& widget);
    virtual ~CWidgetButton();
public:
    virtual bool Init();



protected://�ؽ�ó
    CSharedPtr<class CTexture>  m_Texture;
public:
    //�ؽ�ó ���� - �ؽ�ó �ε� ���� �ѵ� ����Ұ�.
    bool SetTexture(const std::string& Name);
//=======================================================================




protected://��ư ���� ����
    EButtonState           m_ButtonState;
    Sprite_FrameData      m_ButtonStateData[(int)EButtonState::Max];
public:
    void SetButtonStateData(EButtonState State, const Vector2& Start, const Vector2& End);
    void EnableButton(bool Enable);
//====================================================================




protected://���� ����
    CSharedPtr<class CSound>    m_SoundState[(int)EButtonSoundState::Max];
    //�Ҹ� ���� - ���������� ���� �ε� ���� �ѵ� ����Ұ�
public:
    bool SetSound(EButtonSoundState State, const std::string& Name);
//====================================================================

    


protected://���콺 �ݹ� ����

    //EButtonState���� �ݹ��Լ��� ����ϰ� �����س��� ���� �뵵�� �迭
    std::function<void()>   m_Callback[(int)EButtonSoundState::Max];
public:
    virtual void CollisionMouseHoveredCallback(const Vector2& Pos);
    virtual void CollisionMouseReleaseCallback();

    template <typename T>
    void SetCallback(EButtonSoundState State, T* Obj, void(T::* Func)())
    {
        m_Callback[(int)State] = std::bind(Func, Obj);
    }
//======================================================================


public:
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);




};

