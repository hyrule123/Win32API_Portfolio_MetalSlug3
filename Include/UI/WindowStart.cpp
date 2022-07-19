#include "WindowStart.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//새로운 씬 전환을 위해 포함
#include "../Input.h"
#include "../Scene/SceneManager.h"
#include "../Scene/MainScene.h"

//배경화면
#include "WidgetImage.h"

//버튼
#include "WidgetButton.h"

//텍스트, 텍스트이미지
#include "WidgetText.h"
#include "WidgetTextImage.h"

//화면 해상도 얻어오기 위함
#include "../GameManager.h"

//에딧씬 
#include "../Scene/SceneEdit.h"

//페이드인, 페이드아웃
#include "WidgetFadeInOut.h"

//테스트용. 확인후 지울것
#include "MovingTextImage.h"


CWindowStart::CWindowStart():
    m_PlayGameDelayOn(),
    m_Delay(2.f)
{
}

CWindowStart::~CWindowStart()
{
    CInput::GetInst()->DeleteBindClass<CWindowStart>(this);
}

bool CWindowStart::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT);

    //배경화면 생성
    CWidgetImage* Image = CreateWidget<CWidgetImage>("StartSceneBack");
    m_Scene->GetSceneResource()->LoadTexture(
        "StartSceneBack", TEXT("Background/StartSceneBack.bmp"));
    Image->SetTexture("StartSceneBack");


    {
        //텍스트이미지 출력
        CWidgetTextImage* Text = CreateWidget< CWidgetTextImage>("GamePlayText");
        Text->SetTextImage("BasicFontSilver");
        Text->SetPos(ORIGINAL_GAME_RES_WIDTH / 2, 170);
        Text->SetText("Press 1 Key to Insert Coin");
        Text->SetFlicker(10000.f, 0.8f);
        Text->SetAlignMode(EAlignMode::Mid);
    }

#ifdef _DEBUG
    {
        CWidgetTextImage* Text = CreateWidget< CWidgetTextImage>("EditModeText");
        Text->SetTextImage("BasicFontSilver");
        Text->SetPos(100, 214);
        Text->SetText("Ctrl + E to Enter Edit Mode");
        Text->SetFlicker(10000.f, 0.8f);
    }

    CInput::GetInst()->AddBindFunction<CWindowStart>("EditMode", EInput_Type::Down, this, &CWindowStart::OpenEditScene);
#endif




    CInput::GetInst()->AddBindFunction<CWindowStart>("PlayGame", EInput_Type::Down, this, &CWindowStart::PlayGame);

    //사용할 사운드 등록
    m_Scene->GetSceneResource()->LoadSound("UI", "OK", false, "OK.mp3");
    //WidgetButton->SetSound(EButtonSoundState::Click, "OK");











    //{
    //    //버튼 생성, 사운드, 클릭 등록 과정
    //    //1. 버튼 생성
    //    CWidgetButton* WidgetButton = CreateWidget<CWidgetButton>("WidgetButton");
    //    //2. 출력 위치와 사이즈 설정
    //    WidgetButton->SetPos(150.f, 110.f);
    //    WidgetButton->SetSize(30.f, 30.f);
    //    //3. 텍스처 로드
    //    m_Scene->GetSceneResource()->LoadTexture("NormalButton", TEXT("UI/Button_Normal.bmp"));
    //    WidgetButton->SetTexture("NormalButton");
    //    //4. 사운드 로드 및 등록
    //    m_Scene->GetSceneResource()->LoadSound("UI", "OK", false, "OK.mp3");
    //    WidgetButton->SetSound(EButtonSoundState::Click, "OK");
    //    //5. 버튼 상태별 출력 이미지 스프라이트 좌표 설정
    //    for (int i = 0; i < 4; ++i)
    //    {
    //        WidgetButton->SetButtonStateData((EButtonState)i, Vector2(200.f * i, 0.f), Vector2(200.f * (i + 1), 100.f));
    //    }
    //    //6. 버튼 사용 여부 설정
    //    WidgetButton->EnableButton(true);
    //    //7. 클릭시 호출할 콜백 함수 주소 설정
    //    WidgetButton->SetCallback<CWindowStart>(EButtonSoundState::Click, this, &CWindowStart::StartButtonClickCallback);
    //    // 완료 //
    //}

    //{
    //    //Edit버튼 생성, 사운드, 클릭 등록 과정
    //    //1. 버튼 생성
    //    CWidgetButton* WidgetButton = CreateWidget<CWidgetButton>("EditButton");
    //    //2. 출력 위치와 사이즈 설정
    //    WidgetButton->SetPos(150.f, 150.f);
    //    WidgetButton->SetSize(30.f, 30.f);
    //    //3. 텍스처 로드
    //    m_Scene->GetSceneResource()->LoadTexture("EditButton", TEXT("UI/Button_Edit.bmp"));
    //    WidgetButton->SetTexture("EditButton");
    //    //4. 사운드 로드 및 등록
    //  WidgetButton->SetSound(EButtonSoundState::Click, "OK");
    //    //5. 버튼 상태별 출력 이미지 스프라이트 좌표 설정
    //    for (int i = 0; i < 4; ++i)
    //    {
    //        WidgetButton->SetButtonStateData((EButtonState)i, Vector2(200.f * i, 0.f), Vector2(200.f * (i + 1), 100.f));
    //    }
    //    //6. 버튼 사용 여부 설정
    //    WidgetButton->EnableButton(true);
    //    //7. 클릭시 호출할 콜백 함수 주소 설정
    //    WidgetButton->SetCallback<CWindowStart>(EButtonSoundState::Click, this, &CWindowStart::EditButtonClickCallback);
    //    // 완료 //
    //}


    ////텍스트 출력
    //CWidgetText* TestText = CreateWidget<CWidgetText>("TestText");
    //TestText->SetPos(60.f, 115.f);
    //TestText->SetOneLetterAtTime(); //한글자씩 출력 모드 사용 설정
    //TestText->SetInterval(0.5f);

    ////일단 폰트를 큰걸로 하나 로딩해놓고 해당 폰트 사용 설정
    //TestText->SetFont("PixelFont");
    //TestText->SetColor(255, 0, 0);
    //TestText->EnableShadow();
    //TestText->SetText(TEXT("게임 시작!"));

    return true;
}

void CWindowStart::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);

    if (m_PlayGameDelayOn)
    {
        m_Delay -= DeltaTime;
        if (m_Delay < 0.f)
        {
            CWidgetFadeInOut* FadeOut = CreateWidget<CWidgetFadeInOut>("WidgetFadeOut");
            FadeOut->SetEssential(false);
            FadeOut->AddEndFunction< CWindowStart>(this, &CWindowStart::PlayGameCallback);
        }
    }
}



void CWindowStart::PlayGameCallback()
{
    CSceneManager::GetInst()->CreateScene<CMainScene>();
}


void CWindowStart::OpenEditScene()
{
    CSceneManager::GetInst()->CreateScene<CSceneEdit>();
}

void CWindowStart::PlayGame()
{
    CInput::GetInst()->DeleteBindClass< CWindowStart>(this);

    CWidgetTextImage* Text = FindWidget<CWidgetTextImage>("GamePlayText");
    if (Text)
        Text->SetFlicker(1.f, 0.3f);


#ifdef _DEBUG
    Text = nullptr;
    Text = FindWidget<CWidgetTextImage>("EditModeText");
    if (Text)
        Text->SetActive(false);
#endif

    m_Scene->GetSceneResource()->SoundPlay("OK");

    m_PlayGameDelayOn = true;
}

