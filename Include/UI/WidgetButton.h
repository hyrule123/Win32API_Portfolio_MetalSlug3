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



protected://텍스처
    CSharedPtr<class CTexture>  m_Texture;
public:
    //텍스처 설정 - 텍스처 로딩 먼저 한뒤 사용할것.
    bool SetTexture(const std::string& Name);
//=======================================================================




protected://버튼 상태 관련
    EButtonState           m_ButtonState;
    Sprite_FrameData      m_ButtonStateData[(int)EButtonState::Max];
public:
    void SetButtonStateData(EButtonState State, const Vector2& Start, const Vector2& End);
    void EnableButton(bool Enable);
//====================================================================




protected://사운드 관련
    CSharedPtr<class CSound>    m_SoundState[(int)EButtonSoundState::Max];
    //소리 설정 - 마찬가지로 사운드 로딩 먼저 한뒤 사용할것
public:
    bool SetSound(EButtonSoundState State, const std::string& Name);
//====================================================================

    


protected://마우스 콜백 관련

    //EButtonState별로 콜백함수를 등록하고 저장해놓기 위한 용도의 배열
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

