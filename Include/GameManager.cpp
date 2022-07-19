//Class CGameManager
//GameManager.cpp

#include "GameManager.h"
#include <time.h> //여기서만 사용할 시간 헤더

//아이콘
#include "resource.h"

//프레임 관리
#include "Timer.h"

//입력 관리자
#include "Input.h"

//씬매니저
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

//경로 관리자
#include "PathManager.h"

//리소스 관리자
#include "Resource/ResourceManager.h"

//충돌 관리자
#include "Collision/CollisionManager.h"

//플레이어 주소 저장용
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

    //HBRUSH 제거
    for (int i = 0; i < (int)EBrushType::Max; ++i)
    {
        DeleteObject(m_Brush[i]);
    }


    //HPEN 제거
    for (int i = 0; i < (int)EBrushType::Max; ++i)
    {
        DeleteObject(m_Pen[i]);
    }


    //백버퍼 제거
    SelectObject(m_BackhDC, m_BackhBmpPrev);
    DeleteObject(m_BackhBmp);
    DeleteDC(m_BackhDC);

    //주버퍼 제거
    ReleaseDC(m_hWnd, m_hDC);


    

    //싱글턴 패턴들 할당 해제: 생성의 역순으로
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
    rand(); //첫 값 제외


	m_hInst = hInstance;

    m_WindowSize.Width = (int)(ORIGINAL_GAME_RES_WIDTH * SCREEN_SCALE * STATUS_SPACE);
    m_WindowSize.Height = (int)(ORIGINAL_GAME_RES_HEIGHT * SCREEN_SCALE);




	// 윈도우클래스 구조체를 만들어주고 등록한다.
	Register();

	// 윈도우 창을 생성하고 보여준다.
	Create();

    //주버퍼 받아놓기
    m_hDC = GetDC(m_hWnd);


    //백버퍼 초기화 - 원래 해상도 사이즈로 초기화해준다.
    m_BackhDC = CreateCompatibleDC(m_hDC);
    m_BackhBmp = CreateCompatibleBitmap(m_hDC, 
        ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT);
    m_BackhBmpPrev = (HBITMAP)SelectObject(m_BackhDC, m_BackhBmp);

    SetBkMode(m_BackhDC, TRANSPARENT);
    SetTextColor(m_BackhDC, RGB(255, 127, 0));

    //주버퍼 배율 확대
    //배율 자율 조정 모드로 변경
    SetMapMode(m_hDC, MM_ANISOTROPIC);
    //원래 해상도를
    SetWindowExtEx(m_hDC, 
        (int)(ORIGINAL_GAME_RES_WIDTH * STATUS_SPACE), 
        (int)(ORIGINAL_GAME_RES_HEIGHT),
        NULL);
    //확대된 해상도로 적용(디버그 모드일 경우 옆에 빈 공간을 만들어 해당 위치에 디버그 정보 표시
    SetViewportExtEx(m_hDC, 
        m_WindowSize.Width, m_WindowSize.Height, NULL);


    //GDI PLUS

    if (Gdiplus::GdiplusStartup(&m_gpToken, &m_gpsi, NULL) != Gdiplus::Ok)
        return false;



    //BRUSH 초기화
    m_Brush[(int)EBrushType::Black] = CreateSolidBrush(RGB(0, 0, 0));
    m_Brush[(int)EBrushType::Red] = CreateSolidBrush(RGB(255, 0, 0));
    m_Brush[(int)EBrushType::Green] = CreateSolidBrush(RGB(0, 255, 0));
    m_Brush[(int)EBrushType::Blue] = CreateSolidBrush(RGB(0, 0, 255));
    m_Brush[(int)EBrushType::Yellow] = CreateSolidBrush(RGB(255, 255, 0));

    //BRUSH 초기화
    m_Pen[(int)EBrushType::Black] = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    m_Pen[(int)EBrushType::Red] = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    m_Pen[(int)EBrushType::Green] = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    m_Pen[(int)EBrushType::Blue] = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    m_Pen[(int)EBrushType::Yellow] = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));




    //타이머 초기화
    m_Timer = new CTimer;
    m_Timer->Init();
    m_DeltaTime = 0.f;
    m_TimeScale = 1.f;
 


    //경로관리자 생성
    if (!CPathManager::GetInst()->Init())
        return false;
    //리소스 매니저 생성 - 경로 관리자 생성 후에 생성해야 함.
    if (!CResourceManager::GetInst()->Init())
        return false;



    //인풋매니저 생성
    if (!CInput::GetInst()->Init(m_hWnd))
        return false;

    //충돌 매니저 생성 - 게임오브젝트에서 사용해야 하므로 마지막에
    if (!CCollisionManager::GetInst()->Init())
        return false;


    //씬매니저 생성 - 무조건 인풋매니저 생성 후에 생성해야 함.
    if (!CSceneManager::GetInst()->Init())
        return false;



    //공란 생성
    m_Blank = "                                           ";


    //디버그모드 설정
#ifdef _DEBUG
    m_DebugRender = true;
#else
    m_DebugRender = false;
#endif

    //디버그모드 변경 키 등록
    CInput::GetInst()->AddBindFunction<CGameManager>("DebugRenderToggle", EInput_Type::Down, this, &CGameManager::ToggleDebugRender);

    //인게임 타임스케일 증가/감소 기능
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

    //매 프레임 rand 계산
    m_RandNum = rand();

    //현재 창에 포커스 또는 에딧 모드일경우에만 Input을 받는다.
    if (GetFocus() == m_hWnd || m_EditMode)
    {
        Input(m_DeltaTime);
    }
    

    //true가 반환되었다면 Scene 전환이 일어난 것이다.
    //그대로 return 해준다.
    if (Update(m_DeltaTime))
        return;
    if (PostUpdate(m_DeltaTime))
        return;
    Collision(m_DeltaTime);

    if (m_DebugRender)
    {
        DebugTextWipe();    //디버그 텍스트열을 깨끗이 지운다.     
        ShowBasicStatus();  //기본 스테이터스 출력정보 전달
        DebugTextRender();  //상태정보 전달
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
    
    //화이트아웃을 호출하지 않았을 경우에만
    if (m_WhiteOut <= 0)
    {
  

        //백버퍼에 모두 그려낸다.
        CSceneManager::GetInst()->Render(m_BackhDC, DeltaTime);

        //마지막으로, 가장 윗부분 마우스 애니메이션을 렌더링해준다.
        CInput::GetInst()->Render(m_BackhDC, DeltaTime);

    }
    else
        --m_WhiteOut;


        //이후 백버퍼를 주버퍼에 덮어씌운다.
        BitBlt(m_hDC, 0, 0,
            ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT,
            m_BackhDC, 0, 0, SRCCOPY);



    //디버그 텍스트란을 지운다
    DebugTextWipe();
}

void CGameManager::ShowBasicStatus()
{
    DebugTextOut("FPS", m_Timer->GetFPS());

    DebugTextOut("MousePos", CInput::GetInst()->GetMousePos());
}



void CGameManager::Register()
{
    // 레지스터에 등록할 윈도우 클래스 구조체를 만들어준다.
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;

    // 메세지큐에서 꺼내온 메세지를 인자로 전달하며 호출할 함수의 함수 주소를
    // 등록한다.
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // 윈도우 인스턴스를 등록한다.
    wcex.hInstance = m_hInst;

    // 실행파일에 사용할 아이콘을 등록한다.
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

    // 마우스 커서 모양을 결정한다.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // 메뉴를 사용할지 말지를 결정한다.
    wcex.lpszMenuName = nullptr;

    // 등록할 클래스의 이름을 유니코드 문자열로 만들어서 지정한다.
    // TEXT 매크로는 프로젝트 설정이 유니코드로 되어있을 경우 유니코드 문자열로 만들어지고
    // 멀티바이트로 되어있을 경우 멀티바이트 문자열로 만들어지게 된다.
    wcex.lpszClassName = TEXT("MetalSlug3");

    // 윈도우창 좌상단에 표시할 작은 아이콘을 등록한다.
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

    // 윈도우 창을 보여준다. 1번인자에 들어간 핸들의 윈도우 창을 보여줄지 말지를
    // 결정해준다.
    ShowWindow(m_hWnd, SW_SHOW);

    // 이 함수를 호출하여 클라이언트 영역이 제대로 갱신되었다면 0이 아닌 값을 반환하고
    // 갱신이 실패했을 경우 0을 반환한다.
    UpdateWindow(m_hWnd);

    
    return true;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // 윈도우가 종료될때 들어오는 메세지이다.
        m_Loop = false;
        PostQuitMessage(0);
        break;
        //ESC 키 누르면 종료
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
        if (!DT->AddMode)    //AddMode면 값 초기화
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
        if (!DT->AddMode)    //AddMode면 값 초기화
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

        if (!DT->AddMode)    //AddMode면 값 초기화
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

        if (!DT->AddMode)    //AddMode면 값 초기화
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
        //해당 키이름이 들어있는 텍스트가 있으면 true를 반환
        if ((*iter)->Text.find(KeyName) != std::string::npos)
        {
            return iter;
        }
        ++iter;
    }

    //찾지 못했으면 end를 반환
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
        //확대된 해상도로 적용(디버그 모드일 경우 옆에 빈 공간을 만들어 해당 위치에 디버그 정보 표시
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
        //확대된 해상도로 적용(디버그 모드일 경우 옆에 빈 공간을 만들어 해당 위치에 디버그 정보 표시
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