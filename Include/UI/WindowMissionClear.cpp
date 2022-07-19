#include "WindowMissionClear.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

#include "WidgetFadeInOut.h"

#include "WidgetImage.h"
#include "WidgetTextImage.h"

#include "WidgetProgressBar.h"

#include "../ScoreManager.h"

#include "MovingTextImage.h"

CWindowMissionClear::CWindowMissionClear():
    m_vecText{},
    m_Timer(),
    m_BoolFlag(),
    m_MaxSpeed(300.f)
{
    m_ZOrder = 10;
}

CWindowMissionClear::CWindowMissionClear(const CWindowMissionClear& widget) :
    CWidgetWindow(widget),
    m_vecText{},
    m_Timer(widget.m_Timer),
    m_BoolFlag(widget.m_BoolFlag),
    m_MaxSpeed(widget.m_MaxSpeed)
{
}


CWindowMissionClear::~CWindowMissionClear()
{
}

bool CWindowMissionClear::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetPos(0.f, 0.f);
    SetSize(Vector2(ORIGINAL_GAME_RES_WIDTH, ORIGINAL_GAME_RES_HEIGHT));

    return true;
}


void CWindowMissionClear::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);

    //맨 마지막 글자가 이동이 끝났음을 확인하면 플리커 수회 반복후 다시 산개
    if (!m_vecText.empty())
    {
        if (m_vecText.back()->GetMoveDone() && !(m_BoolFlag & FlickerStart))
        {
            size_t size = m_vecText.size();
            for (size_t i = 0; i < size; ++i)
            {
                m_Timer.InitCooltime(1.5f);
                m_Timer.EnterCooltime();
                m_vecText[i]->SetFlicker(1.5f, 0.3f);
                m_BoolFlag |= FlickerStart;
            }
        }
    }

    if ((m_BoolFlag & FlickerStart) && m_Timer.UpdateCooltime(DeltaTime))
    {
        if (!(m_BoolFlag & ScatterStart))
        {
            m_BoolFlag |= ScatterStart;

            //0~6: 윗줄, 나머지 윗줄
            size_t size = m_vecText.size();
            float DestAngle = 0.f;
            int UpperLineNum = 7;
            float UpperDegree = 180.f / (float)UpperLineNum;
            float LowerDegree = 160.f / (float)(size - UpperLineNum);

            float DestYPos = 0.f;
            
            for (size_t i = 0; i < size; ++i)
            {
                if (i < UpperLineNum)
                {
                    DestAngle = 180.f + UpperDegree * (float)(i + 1);
                }
                else
                {
                    DestAngle = 360.f - LowerDegree * (float)(i + 4);
                }
                    

                DestAngle = DegreeToRadian(DestAngle);
                
                Vector2 Dest;
                Dest.Set(cosf(DestAngle), sinf(DestAngle));
                Dest *= 500.f;

                m_vecText[i]->SetEssential(m_MaxSpeed, m_Pos + Dest);
            }
        }
        else if ((m_BoolFlag & ScatterStart) && m_vecText[0]->GetMoveDone())
        {
            size_t size = m_vecText.size();
            for (size_t i = 0; i < size; ++i)
            {
                m_vecText[i]->SetActive(false);
            }
            SetActive(false);
        }
    }



}

void CWindowMissionClear::PostUpdate(float DeltaTime)
{

    CWidgetWindow::PostUpdate(DeltaTime);

}

void CWindowMissionClear::Render(HDC hDC, float DeltaTime)
{
    CWidgetWindow::Render(hDC, DeltaTime);
}



void CWindowMissionClear::RenderMissionComplete()
{
    CMovingTextImage* MissionText = CreateWidget<CMovingTextImage>("TimeLeft");

    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("M");
        MissionText->SetEssential(m_MaxSpeed, Vector2(78, 60));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("I");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("I");
        MissionText->SetEssential(m_MaxSpeed, Vector2(103, 60));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("S");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("S");
        MissionText->SetEssential(m_MaxSpeed, Vector2(117, 60));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("S");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("S");
        MissionText->SetEssential(m_MaxSpeed, Vector2(142, 60));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("I");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("I");
        MissionText->SetEssential(m_MaxSpeed, Vector2(167, 60));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("O");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("O");
        MissionText->SetEssential(m_MaxSpeed, Vector2(181, 60));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("N");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-20.f, -20.f);
        MissionText->SetText("N");
        MissionText->SetEssential(m_MaxSpeed, Vector2(206, 60));
        m_vecText.push_back(MissionText);
    }



    /// <summary>
    /// //////////////////////////////
    /// </summary>
    /// <returns></returns>
    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("C");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("C");
        MissionText->SetEssential(m_MaxSpeed, Vector2(46, 108));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("O");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("O");
        MissionText->SetEssential(m_MaxSpeed, Vector2(71, 108));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("M");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("M");
        MissionText->SetEssential(m_MaxSpeed, Vector2(96, 108));
        m_vecText.push_back(MissionText);
    }


    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("P");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("P");
        MissionText->SetEssential(m_MaxSpeed, Vector2(121, 108));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("L");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("L");
        MissionText->SetEssential(m_MaxSpeed, Vector2(145, 108));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("E");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("E");
        MissionText->SetEssential(m_MaxSpeed, Vector2(168, 108));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("T");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("T");
        MissionText->SetEssential(m_MaxSpeed, Vector2(192, 108));
        m_vecText.push_back(MissionText);
    }

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("E");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("E");
        MissionText->SetEssential(m_MaxSpeed, Vector2(215, 108));
        m_vecText.push_back(MissionText);
    }

    //MissionText = nullptr;
    //MissionText = CreateWidget<CMovingTextImage>("E");
    //if (MissionText)
    //{
    //    MissionText->SetTextImage("MissionFont");
    //    MissionText->SetKerning(0);
    //    MissionText->SetPos(-150.f, -150.f);
    //    MissionText->SetText("E");
    //    MissionText->SetEssential(m_MaxSpeed, Vector2(216, 108));
    //    m_vecText.push_back(MissionText);
    //}

    MissionText = nullptr;
    MissionText = CreateWidget<CMovingTextImage>("!");
    if (MissionText)
    {
        MissionText->SetTextImage("MissionFont");
        MissionText->SetKerning(0);
        MissionText->SetPos(-150.f, -150.f);
        MissionText->SetText("!");
        MissionText->SetEssential(m_MaxSpeed, Vector2(245, 108));
        m_vecText.push_back(MissionText);
    }
}