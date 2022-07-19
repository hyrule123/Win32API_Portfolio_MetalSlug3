#pragma once
#include "../Ref.h"

#include "../GameInfo.h"

#include "Widget.h"

class CWidgetWindow :
    public CRef
{
    friend class CScene;

protected:
    CWidgetWindow();

    CWidgetWindow(const CWidgetWindow& Obj);
    virtual ~CWidgetWindow();
public:
    virtual bool Init();
    class CWidgetFadeInOut* FadeInOut(bool isFadeIn);



protected:
    class CScene* m_Scene;

    bool m_CheckCollide;
public:
    void SetCheckCollide(bool CheckCollide);
    bool GetCheckCollide() const;


protected://위젯 윈도우의 출력 순서 정렬을 위한 변수
    int         m_ZOrder;
public:
    void SetZOrder(int ZOrder);
    int GetZOrder() const;

protected://위젯 목록 
    std::vector<CSharedPtr<class CWidget>>  m_vecWidget;
public:
    class CWidget* GetWidget(int Index);
    int GetWidgetCount()    const;




protected://위치
    Vector2     m_Pos;
public:
    void SetPos(float x, float y);
    void SetPos(const Vector2& Pos);
    const Vector2& GetPos()	const;



protected://사이즈
    Vector2     m_Size;
public:
    void SetSize(float x, float y);
    void SetSize(const Vector2& Size);
    const Vector2& GetSize()	const;


public:
    virtual bool CollisionMouse(CWidget** WidgetAddr, const Vector2& Mousepos);



protected:
    static bool SortWidget(
        const CSharedPtr<CWidget>& Src,
        const CSharedPtr<CWidget>& Dest
    );


public://업데이트 및 출력
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);



public://위젯 탐색/생성/삭제

    void DeleteWidget(const std::string& Name);

    CWidget* FindWidgetCommonType(const std::string& Name);

    template <typename T>
    T* FindWidget(const std::string& Name)
    {
        size_t  Size = m_vecWidget.size();

        for (size_t i = 0; i < Size; ++i)
        {
            if (m_vecWidget[i]->GetName() == Name)
                return (T*)(*m_vecWidget[i]);
        }

        return nullptr;
    }


    template <typename T>
    T* CreateWidget(const std::string& Name)
    {
        T* Widget = new T;

        Widget->SetName(Name);
        Widget->m_Scene = m_Scene;
        Widget->m_Owner = this;

        if (!Widget->Init())
        {
            SAFE_DELETE(Widget);
            return nullptr;
        }

        m_vecWidget.push_back(Widget);

        return (T*)Widget;
    }

};

