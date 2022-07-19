#include "WindowStart.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//���ο� �� ��ȯ�� ���� ����
#include "../Input.h"
#include "../Scene/SceneManager.h"
#include "../Scene/MainScene.h"

//���ȭ��
#include "WidgetImage.h"

//��ư
#include "WidgetButton.h"

//�ؽ�Ʈ, �ؽ�Ʈ�̹���
#include "WidgetText.h"
#include "WidgetTextImage.h"

//ȭ�� �ػ� ������ ����
#include "../GameManager.h"

//������ 
#include "../Scene/SceneEdit.h"

//���̵���, ���̵�ƿ�
#include "WidgetFadeInOut.h"

//�׽�Ʈ��. Ȯ���� �����
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

    //���ȭ�� ����
    CWidgetImage* Image = CreateWidget<CWidgetImage>("StartSceneBack");
    m_Scene->GetSceneResource()->LoadTexture(
        "StartSceneBack", TEXT("Background/StartSceneBack.bmp"));
    Image->SetTexture("StartSceneBack");


    {
        //�ؽ�Ʈ�̹��� ���
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

    //����� ���� ���
    m_Scene->GetSceneResource()->LoadSound("UI", "OK", false, "OK.mp3");
    //WidgetButton->SetSound(EButtonSoundState::Click, "OK");











    //{
    //    //��ư ����, ����, Ŭ�� ��� ����
    //    //1. ��ư ����
    //    CWidgetButton* WidgetButton = CreateWidget<CWidgetButton>("WidgetButton");
    //    //2. ��� ��ġ�� ������ ����
    //    WidgetButton->SetPos(150.f, 110.f);
    //    WidgetButton->SetSize(30.f, 30.f);
    //    //3. �ؽ�ó �ε�
    //    m_Scene->GetSceneResource()->LoadTexture("NormalButton", TEXT("UI/Button_Normal.bmp"));
    //    WidgetButton->SetTexture("NormalButton");
    //    //4. ���� �ε� �� ���
    //    m_Scene->GetSceneResource()->LoadSound("UI", "OK", false, "OK.mp3");
    //    WidgetButton->SetSound(EButtonSoundState::Click, "OK");
    //    //5. ��ư ���º� ��� �̹��� ��������Ʈ ��ǥ ����
    //    for (int i = 0; i < 4; ++i)
    //    {
    //        WidgetButton->SetButtonStateData((EButtonState)i, Vector2(200.f * i, 0.f), Vector2(200.f * (i + 1), 100.f));
    //    }
    //    //6. ��ư ��� ���� ����
    //    WidgetButton->EnableButton(true);
    //    //7. Ŭ���� ȣ���� �ݹ� �Լ� �ּ� ����
    //    WidgetButton->SetCallback<CWindowStart>(EButtonSoundState::Click, this, &CWindowStart::StartButtonClickCallback);
    //    // �Ϸ� //
    //}

    //{
    //    //Edit��ư ����, ����, Ŭ�� ��� ����
    //    //1. ��ư ����
    //    CWidgetButton* WidgetButton = CreateWidget<CWidgetButton>("EditButton");
    //    //2. ��� ��ġ�� ������ ����
    //    WidgetButton->SetPos(150.f, 150.f);
    //    WidgetButton->SetSize(30.f, 30.f);
    //    //3. �ؽ�ó �ε�
    //    m_Scene->GetSceneResource()->LoadTexture("EditButton", TEXT("UI/Button_Edit.bmp"));
    //    WidgetButton->SetTexture("EditButton");
    //    //4. ���� �ε� �� ���
    //  WidgetButton->SetSound(EButtonSoundState::Click, "OK");
    //    //5. ��ư ���º� ��� �̹��� ��������Ʈ ��ǥ ����
    //    for (int i = 0; i < 4; ++i)
    //    {
    //        WidgetButton->SetButtonStateData((EButtonState)i, Vector2(200.f * i, 0.f), Vector2(200.f * (i + 1), 100.f));
    //    }
    //    //6. ��ư ��� ���� ����
    //    WidgetButton->EnableButton(true);
    //    //7. Ŭ���� ȣ���� �ݹ� �Լ� �ּ� ����
    //    WidgetButton->SetCallback<CWindowStart>(EButtonSoundState::Click, this, &CWindowStart::EditButtonClickCallback);
    //    // �Ϸ� //
    //}


    ////�ؽ�Ʈ ���
    //CWidgetText* TestText = CreateWidget<CWidgetText>("TestText");
    //TestText->SetPos(60.f, 115.f);
    //TestText->SetOneLetterAtTime(); //�ѱ��ھ� ��� ��� ��� ����
    //TestText->SetInterval(0.5f);

    ////�ϴ� ��Ʈ�� ū�ɷ� �ϳ� �ε��س��� �ش� ��Ʈ ��� ����
    //TestText->SetFont("PixelFont");
    //TestText->SetColor(255, 0, 0);
    //TestText->EnableShadow();
    //TestText->SetText(TEXT("���� ����!"));

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

