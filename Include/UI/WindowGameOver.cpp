#include "WindowGameOver.h"


#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//���ο� �� ��ȯ�� ���� ����
#include "../Input.h"
#include "../Scene/SceneManager.h"
#include "../Scene/StartScene.h"


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



CWindowGameOver::CWindowGameOver() :
    m_PlayGameDelayOn(),
    m_Delay(2.f)
{
}

CWindowGameOver::~CWindowGameOver()
{
    CInput::GetInst()->DeleteBindClass<CWindowGameOver>(this);
}

bool CWindowGameOver::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT);

    //���ȭ�� ����
    CWidgetImage* Image = CreateWidget<CWidgetImage>("StartSceneBack");
    m_Scene->GetSceneResource()->LoadTexture(
        "GameOver0", TEXT("UI/GameOver0.bmp"));
    Image->SetTexture("GameOver0");


    //���̵���
    CWidgetFadeInOut* FadeIn = CreateWidget<CWidgetFadeInOut>("FadeIn");
    FadeIn->SetEssential(true);


    //10�� �� ���θ޴��� ���ư�.
    m_PlayGameDelayOn = true;
    m_Delay = 10.f;

    //{
    //    //�ؽ�Ʈ�̹��� ���
    //    CWidgetTextImage* Text = CreateWidget< CWidgetTextImage>("GamePlayText");
    //    Text->SetTextImage("BasicFontSilver");
    //    Text->SetPos(ORIGINAL_GAME_RES_WIDTH / 2, 170);
    //    Text->SetText("Press 1 Key to Insert Coin");
    //    Text->SetFlicker(10000.f, 0.8f);
    //    Text->SetAlignMode(EAlignMode::Mid);
    //}

    //{
    //    CWidgetTextImage* Text = CreateWidget< CWidgetTextImage>("EditModeText");
    //    Text->SetTextImage("BasicFontSilver");
    //    Text->SetPos(100, 214);
    //    Text->SetText("Ctrl + E to Enter Edit Mode");
    //    Text->SetFlicker(10000.f, 0.8f);
    //}



    return true;
}

void CWindowGameOver::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);

    if (m_PlayGameDelayOn)
    {
        m_Delay -= DeltaTime;
        if (m_Delay < 0.f)
        {
            CWidgetFadeInOut* FadeOut = CreateWidget<CWidgetFadeInOut>("WidgetFadeOut");
            FadeOut->SetEssential(false);
            FadeOut->AddEndFunction<CWindowGameOver>(this, &CWindowGameOver::ReturnMainMenuCallBack);
        }
    }

}

void CWindowGameOver::ReturnMainMenuCallBack()
{
    CSceneManager::GetInst()->CreateScene<CStartScene>();
}


