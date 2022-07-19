//Class CGameManager
//GameManager.cpp

#include "GameManager.h"
#include <time.h> //���⼭�� ����� �ð� ���

//������
#include "resource.h"

//������ ����
#include "Timer.h"

//�Է� ������
#include "Input.h"

//���Ŵ���
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

//��� ������
#include "PathManager.h"

//���ҽ� ������
#include "Resource/ResourceManager.h"

//�浹 ������
#include "Collision/CollisionManager.h"

//�÷��̾� �ּ� �����
#include "GameObject/Player.h"

#include "ScoreManager.h"




DEFINITION_SINGLETON(CGameManager)
bool CGameManager::m_Loop = true;

CGameManager::CGameManager():
    m_EditMode(false),
    m_DebugRender(),
    m_RandNum()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(62977);
}
CGameManager::~CGameManager()
{
    //GDIPLUS SHUTDOWN
    Gdiplus::GdiplusShutdown(m_gpToken);



    SAFE_DELETE(m_Timer);

    //HBRUSH ����
    for (int i = 0; i < (int)EBrushType::Max; ++i)
    {
        DeleteObject(m_Brush[i]);
    }


    //HPEN ����
    for (int i = 0; i < (int)EBrushType::Max; ++i)
    {
        DeleteObject(m_Pen[i]);
    }


    //����� ����
    SelectObject(m_BackhDC, m_BackhBmpPrev);
    DeleteObject(m_BackhBmp);
    DeleteDC(m_BackhDC);

    //�ֹ��� ����
    ReleaseDC(m_hWnd, m_hDC);


    

    //�̱��� ���ϵ� �Ҵ� ����: ������ ��������
    CScoreManager::DestroyInst();
    CCollisionManager::DestroyInst();
    CSceneManager::DestroyInst();
    CInput::DestroyInst();
    CResourceManager::DestroyInst();
    CPathManager::DestroyInst();

    {
        auto iter = m_listDebugText.begin();
        auto iterEnd = m_listDebugText.end();

        while (iter != iterEnd)
        {
            SAFE_DELETE(*iter);
            ++iter;
        }
    }

}



void CGameManager::DebugTextWipe()
{
    Rectangle(m_hDC, 
        ORIGINAL_GAME_RES_WIDTH , 
        7 * (int)m_listDebugText.size(),
        m_WindowSize.Width + 1,
        m_WindowSize.Height + 1);
}

void CGameManager::SetEditMode(bool Enable)
{
    m_EditMode = Enable;
}

bool CGameManager::GetEditMode() const
{
    return m_EditMode;
}

void CGameManager::SetWhiteOut(int Frame)
{
    m_WhiteOut = Frame;
}

bool CGameManager::Init(HINSTANCE hInstance)
{
    srand((unsigned int)time(0));
    rand(); //ù �� ����


	m_hInst = hInstance;

    m_WindowSize.Width = (int)(ORIGINAL_GAME_RES_WIDTH * SCREEN_SCALE * STATUS_SPACE);
    m_WindowSize.Height = (int)(ORIGINAL_GAME_RES_HEIGHT * SCREEN_SCALE);




	// ������Ŭ���� ����ü�� ������ְ� ����Ѵ�.
	Register();

	// ������ â�� �����ϰ� �����ش�.
	Create();

    //�ֹ��� �޾Ƴ���
    m_hDC = GetDC(m_hWnd);


    //����� �ʱ�ȭ - ���� �ػ� ������� �ʱ�ȭ���ش�.
    m_BackhDC = CreateCompatibleDC(m_hDC);
    m_BackhBmp = CreateCompatibleBitmap(m_hDC, 
        ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT);
    m_BackhBmpPrev = (HBITMAP)SelectObject(m_BackhDC, m_BackhBmp);

    SetBkMode(m_BackhDC, TRANSPARENT);
    SetTextColor(m_BackhDC, RGB(255, 127, 0));

    //�ֹ��� ���� Ȯ��
    //���� ���� ���� ���� ����
    SetMapMode(m_hDC, MM_ANISOTROPIC);
    //���� �ػ󵵸�
    SetWindowExtEx(m_hDC, 
        (int)(ORIGINAL_GAME_RES_WIDTH * STATUS_SPACE), 
        (int)(ORIGINAL_GAME_RES_HEIGHT),
        NULL);
    //Ȯ��� �ػ󵵷� ����(����� ����� ��� ���� �� ������ ����� �ش� ��ġ�� ����� ���� ǥ��
    SetViewportExtEx(m_hDC, 
        m_WindowSize.Width, m_WindowSize.Height, NULL);


    //GDI PLUS

    if (Gdiplus::GdiplusStartup(&m_gpToken, &m_gpsi, NULL) != Gdiplus::Ok)
        return false;



    //BRUSH �ʱ�ȭ
    m_Brush[(int)EBrushType::Black] = CreateSolidBrush(RGB(0, 0, 0));
    m_Brush[(int)EBrushType::Red] = CreateSolidBrush(RGB(255, 0, 0));
    m_Brush[(int)EBrushType::Green] = CreateSolidBrush(RGB(0, 255, 0));
    m_Brush[(int)EBrushType::Blue] = CreateSolidBrush(RGB(0, 0, 255));
    m_Brush[(int)EBrushType::Yellow] = CreateSolidBrush(RGB(255, 255, 0));

    //BRUSH �ʱ�ȭ
    m_Pen[(int)EBrushType::Black] = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    m_Pen[(int)EBrushType::Red] = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    m_Pen[(int)EBrushType::Green] = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    m_Pen[(int)EBrushType::Blue] = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    m_Pen[(int)EBrushType::Yellow] = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));




    //Ÿ�̸� �ʱ�ȭ
    m_Timer = new CTimer;
    m_Timer->Init();
    m_DeltaTime = 0.f;
    m_TimeScale = 1.f;
 


    //��ΰ����� ����
    if (!CPathManager::GetInst()->Init())
        return false;
    //���ҽ� �Ŵ��� ���� - ��� ������ ���� �Ŀ� �����ؾ� ��.
    if (!CResourceManager::GetInst()->Init())
        return false;



    //��ǲ�Ŵ��� ����
    if (!CInput::GetInst()->Init(m_hWnd))
        return false;

    //�浹 �Ŵ��� ���� - ���ӿ�����Ʈ���� ����ؾ� �ϹǷ� ��������
    if (!CCollisionManager::GetInst()->Init())
        return false;


    //���Ŵ��� ���� - ������ ��ǲ�Ŵ��� ���� �Ŀ� �����ؾ� ��.
    if (!CSceneManager::GetInst()->Init())
        return false;



    //���� ����
    m_Blank = "                                           ";


    //����׸�� ����
#ifdef _DEBUG
    m_DebugRender = true;
#else
    m_DebugRender = false;
#endif

    //����׸�� ���� Ű ���
    CInput::GetInst()->AddBindFunction<CGameManager>("DebugRenderToggle", EInput_Type::Down, this, &CGameManager::ToggleDebugRender);

    //�ΰ��� Ÿ�ӽ����� ����/���� ���
    CInput::GetInst()->AddBindFunction<CGameManager>("TimeScaleUp", EInput_Type::Down, this, &CGameManager::TimeScaleUp);
    CInput::GetInst()->AddBindFunction<CGameManager>("TimeScaleDown", EInput_Type::Down, this, &CGameManager::TimeScaleDown);


	return true;
}

int CGameManager::Run()
{
    MSG msg = {};

    while (m_Loop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else 
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

void CGameManager::Logic()
{


    m_DeltaTime = m_Timer->Update() * m_TimeScale;

    //�� ������ rand ���
    m_RandNum = rand();

    //���� â�� ��Ŀ�� �Ǵ� ���� ����ϰ�쿡�� Input�� �޴´�.
    if (GetFocus() == m_hWnd || m_EditMode)
    {
        Input(m_DeltaTime);
    }
    

    //true�� ��ȯ�Ǿ��ٸ� Scene ��ȯ�� �Ͼ ���̴�.
    //�״�� return ���ش�.
    if (Update(m_DeltaTime))
        return;
    if (PostUpdate(m_DeltaTime))
        return;
    Collision(m_DeltaTime);

    if (m_DebugRender)
    {
        DebugTextWipe();    //����� �ؽ�Ʈ���� ������ �����.     
        ShowBasicStatus();  //�⺻ �������ͽ� ������� ����
        DebugTextRender();  //�������� ����
    }


    Render(m_DeltaTime);
}

void CGameManager::Input(float DeltaTime)
{
    CInput::GetInst()->Update(DeltaTime);
}

bool CGameManager::Update(float DeltaTime)
{


    CResourceManager::GetInst()->Update();


    return CSceneManager::GetInst()->Update(DeltaTime);
}

bool CGameManager::PostUpdate(float DeltaTime)
{
    CInput::GetInst()->PostUpdate(DeltaTime);

    return CSceneManager::GetInst()->PostUpdate(DeltaTime);
}

void CGameManager::Collision(float DeltaTime)
{
}

void CGameManager::Render(float DeltaTime)
{
    Rectangle(m_BackhDC, -1, -1, ORIGINAL_GAME_RES_WIDTH+1, ORIGINAL_GAME_RES_HEIGHT+1);
    
    //ȭ��Ʈ�ƿ��� ȣ������ �ʾ��� ��쿡��
    if (m_WhiteOut <= 0)
    {
  

        //����ۿ� ��� �׷�����.
        CSceneManager::GetInst()->Render(m_BackhDC, DeltaTime);

        //����������, ���� ���κ� ���콺 �ִϸ��̼��� ���������ش�.
        CInput::GetInst()->Render(m_BackhDC, DeltaTime);

    }
    else
        --m_WhiteOut;


        //���� ����۸� �ֹ��ۿ� ������.
        BitBlt(m_hDC, 0, 0,
            ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT,
            m_BackhDC, 0, 0, SRCCOPY);



    //����� �ؽ�Ʈ���� �����
    DebugTextWipe();
}

void CGameManager::ShowBasicStatus()
{
    DebugTextOut("FPS", m_Timer->GetFPS());

    DebugTextOut("MousePos", CInput::GetInst()->GetMousePos());
}



void CGameManager::Register()
{
    // �������Ϳ� ����� ������ Ŭ���� ����ü�� ������ش�.
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;

    // �޼���ť���� ������ �޼����� ���ڷ� �����ϸ� ȣ���� �Լ��� �Լ� �ּҸ�
    // ����Ѵ�.
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // ������ �ν��Ͻ��� ����Ѵ�.
    wcex.hInstance = m_hInst;

    // �������Ͽ� ����� �������� ����Ѵ�.
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

    // ���콺 Ŀ�� ����� �����Ѵ�.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // �޴��� ������� ������ �����Ѵ�.
    wcex.lpszMenuName = nullptr;

    // ����� Ŭ������ �̸��� �����ڵ� ���ڿ��� ���� �����Ѵ�.
    // TEXT ��ũ�δ� ������Ʈ ������ �����ڵ�� �Ǿ����� ��� �����ڵ� ���ڿ��� ���������
    // ��Ƽ����Ʈ�� �Ǿ����� ��� ��Ƽ����Ʈ ���ڿ��� ��������� �ȴ�.
    wcex.lpszClassName = TEXT("MetalSlug3");

    // ������â �»�ܿ� ǥ���� ���� �������� ����Ѵ�.
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClassEx(&wcex);
}

bool CGameManager::Create()
{
    m_hWnd = CreateWindow(TEXT("MetalSlug3"),
        TEXT("MetalSlug3"), WS_OVERLAPPED | WS_SYSMENU,
        200, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
    {
        return false;
    }

    RECT rc = { 0, 0, 
        m_WindowSize.Width, m_WindowSize.Height};
    

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    MoveWindow(m_hWnd, 200, 0, abs(rc.left) + abs(rc.right), abs(rc.top) + abs(rc.bottom), true);

    // ������ â�� �����ش�. 1�����ڿ� �� �ڵ��� ������ â�� �������� ������
    // �������ش�.
    ShowWindow(m_hWnd, SW_SHOW);

    // �� �Լ��� ȣ���Ͽ� Ŭ���̾�Ʈ ������ ����� ���ŵǾ��ٸ� 0�� �ƴ� ���� ��ȯ�ϰ�
    // ������ �������� ��� 0�� ��ȯ�Ѵ�.
    UpdateWindow(m_hWnd);

    
    return true;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // �����찡 ����ɶ� ������ �޼����̴�.
        m_Loop = false;
        PostQuitMessage(0);
        break;
        //ESC Ű ������ ����
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hWnd);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CGameManager::TimeScaleUp()
{
    m_TimeScale += 0.1f;
}

void CGameManager::TimeScaleDown()
{
    m_TimeScale -= 0.1f;
}

//void CGameManager::DebugTextOut(const char* txt)
//{
//    TextOutA(m_hDC, ORIGINAL_GAME_RES_WIDTH + 1, 7 *m_DebugTextOutCount, txt, (int)strlen(txt));
//    ++m_DebugTextOutCount;
//}

void CGameManager::DebugTextOut(const std::string& KeyName, const std::string& txt)
{
    std::list<DebugText*>::iterator iter = FindKeyName(KeyName);

    DebugText* DT = nullptr;
    if (iter == m_listDebugText.end())
    {
        DT = new DebugText;
        m_listDebugText.push_back(DT);
    }
    else
    {
        DT = *iter;
        DT->Duration = DT->DurationMax;
    }

    if (DT)
    {
        if (!DT->AddMode)    //AddMode�� �� �ʱ�ȭ
        {
            DT->Text = KeyName + ": " + txt;
        }
        else
        {
            DT->Text += ", " + txt;
        }
    }

}

void CGameManager::DebugTextOut(const std::string& KeyName, int Num)
{
    std::list<DebugText*>::iterator iter = FindKeyName(KeyName);

    DebugText* DT = nullptr;
    if (iter == m_listDebugText.end())
    {
        DT = new DebugText;
        m_listDebugText.push_back(DT);
    }
    else
    {
        DT = *iter;
        DT->Duration = DT->DurationMax;
    }

    if (DT)
    {
        if (!DT->AddMode)    //AddMode�� �� �ʱ�ȭ
        {
            DT->Text = KeyName + ": " + std::to_string(Num);
        }
        else
        {
            DT->Text += ", " + std::to_string(Num);
        }
    }
}

void CGameManager::DebugTextOut(const std::string& KeyName, float Flt)
{
    std::list<DebugText*>::iterator iter = FindKeyName(KeyName);

    DebugText* DT = nullptr;
    if (iter == m_listDebugText.end())
    {
        DT = new DebugText;
        m_listDebugText.push_back(DT);
    }
    else
    {
        DT = *iter;
        DT->Duration = DT->DurationMax;
    }

    if (DT)
    {
        char temp[64] = {};

        if (!DT->AddMode)    //AddMode�� �� �ʱ�ȭ
        {
            DT->Text = KeyName + ": ";

            sprintf_s(temp, "%.2f", Flt);
            DT->Text += temp;
        }
        else
        {
            DT->Text += " / "; 
            sprintf_s(temp, "%.2f", Flt);
            DT->Text += temp;
        }
    }

}

void CGameManager::DebugTextOut(const std::string& KeyName, Vector2 Vec)
{
    std::list<DebugText*>::iterator iter = FindKeyName(KeyName);

    DebugText* DT = nullptr;
    if (iter == m_listDebugText.end())
    {
        DT = new DebugText;
        m_listDebugText.push_back(DT);
    }
    else
    {
        DT = *iter;
        DT->Duration = DT->DurationMax;
    }

    if (DT)
    {
        char temp[64] = {};

        if (!DT->AddMode)    //AddMode�� �� �ʱ�ȭ
        {
            DT->Text = KeyName + ": ";

            sprintf_s(temp, "%.2f", Vec.x);
            DT->Text += temp;

            memset(temp, 0, sizeof(temp));
            sprintf_s(temp, ", %.2f", Vec.y);
            DT->Text += temp;
        }
        else
        {
            DT->Text += " / ";

            sprintf_s(temp, "%.2f", Vec.x);
            DT->Text += temp;

            memset(temp, 0, sizeof(temp));
            sprintf_s(temp, ", %.2f", Vec.y);
            DT->Text += temp;
        }
    }

}

std::list<DebugText*>::iterator CGameManager::FindKeyName(const std::string& KeyName)
{
    std::list<DebugText*>::iterator iter = m_listDebugText.begin();
    std::list<DebugText*>::iterator iterEnd = m_listDebugText.end();

    while (iter != iterEnd)
    {
        //�ش� Ű�̸��� ����ִ� �ؽ�Ʈ�� ������ true�� ��ȯ
        if ((*iter)->Text.find(KeyName) != std::string::npos)
        {
            return iter;
        }
        ++iter;
    }

    //ã�� �������� end�� ��ȯ
    return iterEnd;
}

void CGameManager::DebugTextRender()
{
    UINT8 count = 0;

    auto iter = m_listDebugText.begin();
    auto iterEnd = m_listDebugText.end();

    while (iter != iterEnd)
    {
        (*iter)->Duration -= m_DeltaTime;
        if ((*iter)->Duration < 0.f)
        {
            SAFE_DELETE(*iter);

            iter = m_listDebugText.erase(iter);

            continue;
        }


        std::string out = (*iter)->Text + m_Blank;

        TextOutA(m_hDC, ORIGINAL_GAME_RES_WIDTH + 1, 7 * count, (*iter)->Text.c_str(), (int)(*iter)->Text.size());

        ++count;
        ++iter;
    }

}


void CGameManager::ToggleDebugRender()
{
    m_DebugRender = !m_DebugRender;

    if (m_DebugRender)
    {
        m_WindowSize.Width = (int)(ORIGINAL_GAME_RES_WIDTH * SCREEN_SCALE * STATUS_SPACE);

        RECT rc = { 0, 0,
           m_WindowSize.Width, m_WindowSize.Height };

        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

        MoveWindow(m_hWnd, 200, 0, abs(rc.left) + abs(rc.right), abs(rc.top) + abs(rc.bottom), true);


        SetWindowExtEx(m_hDC,
            (int)(ORIGINAL_GAME_RES_WIDTH * STATUS_SPACE),
            (int)(ORIGINAL_GAME_RES_HEIGHT),
            NULL);
        //Ȯ��� �ػ󵵷� ����(����� ����� ��� ���� �� ������ ����� �ش� ��ġ�� ����� ���� ǥ��
        SetViewportExtEx(m_hDC,
            m_WindowSize.Width, m_WindowSize.Height, NULL);
    }
    else
    {
        m_WindowSize.Width = (int)(ORIGINAL_GAME_RES_WIDTH * SCREEN_SCALE);

        RECT rc = { 0, 0,
            m_WindowSize.Width, m_WindowSize.Height };

        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

        MoveWindow(m_hWnd, 200, 0, abs(rc.left) + abs(rc.right), abs(rc.top) + abs(rc.bottom), true);


        SetWindowExtEx(m_hDC,
            (int)(ORIGINAL_GAME_RES_WIDTH),
            (int)(ORIGINAL_GAME_RES_HEIGHT),
            NULL);
        //Ȯ��� �ػ󵵷� ����(����� ����� ��� ���� �� ������ ����� �ش� ��ġ�� ����� ���� ǥ��
        SetViewportExtEx(m_hDC,
            m_WindowSize.Width, m_WindowSize.Height, NULL);

    }


}

bool CGameManager::GetDebugRender() const
{
    return m_DebugRender;
}

HBRUSH CGameManager::GetBrush(EBrushType Type) const
{
    return m_Brush[(int)Type];
}

HPEN CGameManager::GetPen(EBrushType Type) const
{
    return m_Pen[(int)Type];
}

void CGameManager::SetPlayer(CPlayer* Player)
{
    m_Player = Player;
}

void CGameManager::DeletePlayer(CPlayer* Player)
{
    if (m_Player == Player)
        m_Player = nullptr;
}

CPlayer* CGameManager::GetPlayer() const
{
    return m_Player;
}


float CGameManager::GetTimeScale() const
{
    return m_TimeScale;
}

void CGameManager::SetTimeScale(float TimeScale)
{
    m_TimeScale = TimeScale;
}

int CGameManager::GetRandNum() const
{
    return m_RandNum;
}

HINSTANCE CGameManager::GetWindowInstance() const
{
    return m_hInst;
}

HDC CGameManager::GetWindowDC() const
{
    return m_hDC;
}

HDC CGameManager::GetWindowBackDC() const
{
    return m_BackhDC;
}

HWND CGameManager::GetWindowHandle() const
{
    return m_hWnd;
}

float CGameManager::GetDeltaTime() const
{
    return m_DeltaTime;
}

Resolution CGameManager::GetWindowSize() const
{
    return m_WindowSize;
}