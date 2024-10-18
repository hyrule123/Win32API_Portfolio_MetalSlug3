#include "WindowEdit.h"

//텍스트 출력
#include "WidgetText.h"

//카메라 위치 추적
#include "../Scene/Scene.h"

#include "../Input.h"

CWindowEdit::CWindowEdit()
{
}

CWindowEdit::~CWindowEdit()
{
}

bool CWindowEdit::Init()
{
	m_Text = CreateWidget<CWidgetText>("Notice");
	m_Text->SetPos(0, ORIGINAL_GAME_RES_HEIGHT - 10);
	m_Text->SetText(TEXT("< CTRL + T > To Open Tile Editor"));
	m_Text->SetFont("PixelFont");

	//m_Text = CreateWidget<CWidgetText>("Notice1");
	//m_Text->SetPos(0, ORIGINAL_GAME_RES_HEIGHT - 20);
	//m_Text->SetFont("PixelFont");

	return true;
}

void CWindowEdit::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	//TCHAR txt[128] = {};

	//tsPrintf_s(txt, TEXT("CamPos: %d, %d"), (int)m_Scene->GetCamPos().x, (int)m_Scene->GetCamPos().y);
	//m_Text->SetText(txt);

}
