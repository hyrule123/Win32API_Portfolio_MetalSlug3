#pragma once
#include "../Ref.h"

#include "../GameInfo.h"


class CWidget :
    public CRef
{
    friend class CWidgetWindow;

protected:
    CWidget();
    CWidget(const CWidget& widget);
    virtual ~CWidget();
public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, const Vector2& Pos, float DeltaTime);

protected:
    bool m_Start;
    class CScene* m_Scene;
    class CWidgetWindow* m_Owner;

protected:
    int     m_ZOrder;
public:
    int GetZOrder() const;
    void SetZOrder(int ZOrder);


protected://��ġ =( + ������)���� ��� ��ġ�� ������ �� �ִ�.
    Vector2 m_Pos;
    Vector2 m_Offset;
public:
    const Vector2& GetPos()	const;
    void SetPos(float _x, float _y);
    void SetPos(const Vector2& Pos);
    void SetOffset(float _x, float _y);





protected:
    Vector2 m_Size;
public:
    //WidgetImage���� �����Լ��� �����.
    virtual void SetSize(float _x, float _y);
    virtual void SetSize(const Vector2& Size);
    const Vector2& GetSize()	const;




protected://�浹 ���� �޼ҵ�
    bool    m_MouseHovered;
    bool    m_SkipCollisionCheck;
public:
    void SetSkipCollisionCheck(bool SkipCollisionCheck);
    bool CollisionMouse(const Vector2& Pos);


public:
    virtual void CollisionMouseHoveredCallback(const Vector2& Pos);
    virtual void CollisionMouseReleaseCallback();






};

