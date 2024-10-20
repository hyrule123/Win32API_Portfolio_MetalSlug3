
#include "GameObject.h"

//리소스 접근
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

//텍스처
#include "../Resource/Texture/Texture.h"

//DELTA_TIME
#include "../GameManager.h"

//Camera
#include "../Scene/Camera.h"

//충돌체
#include "../Collision/Collider.h"
#include "../Scene/SceneCollision.h"


//위젯 컴포넌트
#include "../UI/WidgetComponent.h"

//애니메이션 프리셋 로딩용 csv 리더
#include "../CSVEditor.h"

//임시
#include "../Resource/Animation/Animation.h"




CGameObject::CGameObject() :
	m_Start(false),
	m_Dir(),
	m_Speed(),
	m_MaxSpeed(0.f),
	m_DeAccel(1.f),
	m_Pos(),
	m_Size(10.f, 10.f),
	m_Pivot(0.5f, 1.f),
	m_Scale(1.f),
	m_Scene(),
	m_PrevPos(),
	m_MoveDist(),
	m_PhysicsSimulate(false),
	m_Ground(true),
	m_GravityAccel(9.8f),
	m_FallTime(),
	m_FallStartY(),
	m_Jump(false),
	m_JumpVelocity(30.f),
	m_SideWallCheck(),
	m_WidgetComponent(),
	m_MasterObject(),
	m_RenderLayer(ERenderLayer::Default),
	m_TimeScaleObj(1.f),
	m_Texture(),
	m_Animation{},
	m_NoMove(),
	m_MaxReactionChannel(),
	m_CurrentReaction(),
	m_ReactionTime(),
	m_FlickerFrequency(0.1f),
	m_Flicker(),
	m_FlickerOn(),
	m_Invincible(),
	m_CullingDeleteFlag(),
	m_isAccel(),
	m_vecCooltime{},
	m_MoveToMethod(),
	m_MoveToDest(),
	m_MoveToOn(),
	m_SideCollOn(),
	m_CalcDir()
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& Obj) :
	CRef(Obj),
	m_ForceSizeRender(Obj.m_ForceSizeRender),
	m_Start(false),
	m_Dir(Obj.m_Dir),
	m_Speed(Obj.m_Speed),
	m_DeAccel(Obj.m_DeAccel),
	m_MaxSpeed(Obj.m_MaxSpeed),
	m_Pos(Obj.m_Pos),
	m_Size(Obj.m_Size),
	m_Pivot(Obj.m_Pivot),
	m_Scale(Obj.m_Scale),
	m_Scene(Obj.m_Scene),
	m_PrevPos(),
	m_MoveDist(),
	m_PhysicsSimulate(Obj.m_PhysicsSimulate),
	m_Ground(Obj.m_Ground),
	m_GravityAccel(Obj.m_GravityAccel),
	m_FallTime(Obj.m_FallTime),
	m_FallStartY(Obj.m_FallStartY),
	m_Jump(Obj.m_Jump),
	m_JumpVelocity(Obj.m_JumpVelocity),
	m_SideWallCheck(Obj.m_SideWallCheck),
	m_WidgetComponent(nullptr),
	m_MasterObject(),
	m_RenderLayer(Obj.m_RenderLayer),
	m_TimeScaleObj(Obj.m_TimeScaleObj),
	m_Texture(Obj.m_Texture),
	m_NoMove(Obj.m_NoMove),
	m_MaxReactionChannel(Obj.m_MaxReactionChannel),
	m_CurrentReaction(),
	m_ReactionTime(),
	m_FlickerFrequency(0.1f),
	m_Flicker(),
	m_FlickerOn(),
	m_Invincible(Obj.m_Invincible),
	m_CullingDeleteFlag(),
	m_isAccel(Obj.m_isAccel),
	m_vecCooltime(Obj.m_vecCooltime),
	m_MoveToMethod(Obj.m_MoveToMethod),
	m_MoveToDest(Obj.m_MoveToDest),
	m_MoveToOn(Obj.m_MoveToOn),
	m_SideCollOn(Obj.m_SideCollOn),
	m_CalcDir(Obj.m_CalcDir)

{
	if (Obj.m_Animation)
		m_Animation = new CAnimation(*Obj.m_Animation);

	//기본적으로 전부 컬링 진행
	m_CullingDeleteFlag |= (UINT8)ECullingDelete::CullingOn;
	m_CullingDeleteFlag |= (UINT8)ECullingDelete::All;
	
	//Init() 이후에 처리되는 변수들
	//m_MasterObject
	//m_SlaveObject 
	//m_listCollider
}



CGameObject::~CGameObject()
{
	SAFE_DELETE(m_Animation);


	//만약 충돌체 리스트에 가지고 있는 충돌체가 있을경우 
	//해당 충돌체의 ClearCollisionList() 메소드를 호출하여
	//해당 출돌체와 충돌판정이 나고 있는 상대방까지 모두 깔끔하게 제거해준다.
	if (!m_listCollider.empty())
	{
		auto iter = m_listCollider.begin();
		auto iterEnd = m_listCollider.end();

		while (iter != iterEnd)
		{
			(*iter)->ClearCollisionList();

			

			++iter;
		}
	}

	m_listCollider.clear();
}

bool CGameObject::LoadResource()
{
	return true;
}

bool CGameObject::Init(CGameObject* Obj)
{
	if (Obj)
		m_MasterObject = Obj;

	if (m_Animation)
		m_Animation->m_OwnerObj = this;

	

	return true;
}

void CGameObject::Start()
{
	m_Start = true;

	m_PrevPos = m_Pos;
}



int CGameObject::InflictDamage(int Damage)
{
	if (m_Invincible > 0.f)
		return 0;
	
	return Damage;
}

void CGameObject::SetInvincible(float Duration, bool Flicker)
{
	if (Flicker)
		StartReaction(Duration, true, EReactionChannel::InvincibleReaction);

	m_Invincible = Duration;
}


void CGameObject::StartReaction(
	float ReactionDuration, bool FlickerOn, EReactionChannel Reaction)
{
	m_ReactionTime = ReactionDuration;

	m_FlickerOn = FlickerOn;

	//최대 리액션 채널이 지정한 리액션 채널 이상일 경우에만 처리
	if (m_MaxReactionChannel >= Reaction)
		m_CurrentReaction = Reaction;
}

bool CGameObject::Culling(
	const Vector2& PosLT, const Vector2& Size, const Vector2& Res)
{
	if (PosLT.x + Size.x < 0.f)	//왼쪽
	{
		if (m_CullingDeleteFlag & (UINT8)ECullingDelete::Left)
			SetActive(false);

		return false;
	}


	if (PosLT.y + Size.y < 0.f)	//위쪽
	{
		if (m_CullingDeleteFlag & (UINT8)ECullingDelete::Top)
			SetActive(false);

		return false;
	}
		

	if (PosLT.x > Res.x)
	{
		if (m_CullingDeleteFlag & (UINT8)ECullingDelete::Right)
			SetActive(false);

		return false;
	}
		
	if (PosLT.y > Res.y)
	{
		if (m_CullingDeleteFlag & (UINT8)ECullingDelete::Bottom)
			SetActive(false);

		return false;
	}
		


	return true;
}

void CGameObject::SetCulling(bool CullingOn)
{
	if (CullingOn)
		m_CullingDeleteFlag |= (UINT8)ECullingDelete::CullingOn;
	else
		m_CullingDeleteFlag &= ~(UINT8)ECullingDelete::CullingOn;
}

void CGameObject::SetCullingDelete(ECullingDelete Delete, bool On)
{
	if (On)
	{
		m_CullingDeleteFlag |= (UINT8)Delete;
		m_CullingDeleteFlag |= (UINT8)ECullingDelete::CullingOn;
	}
		
	else
		m_CullingDeleteFlag &= ~(UINT8)Delete;
}	

UINT8 CGameObject::GetCullingDelete() const
{
	return m_CullingDeleteFlag;
}

void CGameObject::ClearCollider()
{
	auto iter = m_listCollider.begin();
	auto iterEnd = m_listCollider.end();

	if (iter != iterEnd)
	{
		(*iter)->SetActive(false);

		++iter;
	}
}

bool CGameObject::CheckColliding(CGameObject* Obj)
{
	auto iter = m_listCollider.begin();
	auto iterEnd = m_listCollider.end();

	while (iter != iterEnd) 
	{
		if ((*iter)->CheckColliding(Obj))
			return true;
		++iter;
	}

	return false;
}

void CGameObject::MoveDir(const Vector2& Dir)
{
	m_Pos += Dir * m_MaxSpeed * m_TimeScaleObj * DELTA_TIME;
}

void CGameObject::MovePos(const Vector2& Pos)
{
	m_Pos = Pos;
}

void CGameObject::MoveAngle(float Angle)
{
	float rad = DegreeToRadian(Angle);

	Vector2 Dir = Vector2(cosf(rad), sinf(rad));

	m_Pos += Dir * m_MaxSpeed * m_TimeScaleObj * DELTA_TIME;

}

Vector2 CGameObject::GetCalcDir() const
{
	return m_CalcDir;
}

void CGameObject::SetSideColl(bool On)
{
	m_SideCollOn = On;
}

void CGameObject::MoveValue(const Vector2& MoveValue)
{
	m_Pos += MoveValue * m_TimeScaleObj;
}

float CGameObject::GetScale() const
{
	return m_Scale;
}

void CGameObject::SetScale(float Scale)
{
	m_Scale = Scale;
}

ERenderLayer CGameObject::GetRenderLayer() const
{
	return m_RenderLayer;
}

void CGameObject::SetRenderLayer(ERenderLayer Layer)
{
	m_Scene->ChangeRenderLayer(this, m_RenderLayer, Layer);

		
	m_RenderLayer = Layer;
}

float CGameObject::GetTimeScaleObj() const
{
	return m_TimeScaleObj;
}

void CGameObject::SetTimeScaleObj(float TimeScaleObj)
{
	m_TimeScaleObj = TimeScaleObj;
}

void CGameObject::AddObj(CGameObject* Obj)
{
	CSharedPtr<CGameObject> obj = Obj;
	m_SlaveObject.push_back(obj);
}

void CGameObject::DeleteSlave(CGameObject* SlavePtr)
{
	auto iter = m_SlaveObject.begin();
	auto iterEnd = m_SlaveObject.end();

	while (iter != iterEnd)
	{
		if (SlavePtr == (*iter))
		{
			m_SlaveObject.erase(iter);
			break;
		}

		++iter;
	}
}

void CGameObject::SetOwnerScene(CScene* Scene)
{
	m_Scene = Scene;
}

CScene* CGameObject::GetOwnerScene() const
{
	return m_Scene;
}

void CGameObject::DeleteMasterSlave()
{
	if (m_MasterObject)
		m_MasterObject->DeleteSlave(this);

	m_MasterObject = nullptr;
}

void CGameObject::SetDir(const Vector2& Dir)
{
	m_Dir = Dir;
}
void CGameObject::SetForceSizeRender(bool On)
{
	m_ForceSizeRender = On;
}
void CGameObject::SetDir(float _x, float _y)
{
	m_Dir.x = _x;
	m_Dir.y = _y;

}
void CGameObject::SetDirX(float _x)
{
	m_Dir.x = _x;
}
void CGameObject::SetDirY(float _y)
{
	m_Dir.y = _y;
}
Vector2 CGameObject::GetDir()	const
{
	return m_Dir;
}



void CGameObject::SetPos(float _x, float _y)
{
	m_Pos.x = _x;
	m_Pos.y = _y;
}

void CGameObject::SetPos(const Vector2& Vec)
{
	m_Pos = Vec;
}

void CGameObject::SetPosX(float x)
{
	m_Pos.x = x;
}

void CGameObject::SetPosY(float y)
{
	m_Pos.y = y;
}

Vector2 CGameObject::GetPos() const
{
	return m_Pos;
}






void CGameObject::SetMaxSpeed(float Velocity)
{
	m_MaxSpeed = Velocity;
}

Vector2 CGameObject::GetPrevPos() const
{
	return m_PrevPos;
}

void CGameObject::SetSize(float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;
}
void CGameObject::SetSize(const Vector2& Size)
{
	m_Size = Size;
}

Vector2 CGameObject::GetSize() const
{
	return m_Size;
}

void CGameObject::SetPivot(float _x, float _y)
{
	m_Pivot.x = _x;
	m_Pivot.y = _y;
}

void CGameObject::MoveValue(float _x, float _y)
{
	m_Pos.x += _x;
	m_Pos.y += _y;
}

Vector2 CGameObject::GetPivot() const
{
	return m_Pivot;
}



void CGameObject::AddImpact(float Impact)
{
	m_isAccel = true;
	m_Speed += Impact;
}

void CGameObject::AddForce(float Force)
{
	m_isAccel = true;
	m_Speed += Force * DELTA_TIME;
}


void CGameObject::SetDeAccel(float DeAccel)
{
	if (DeAccel > 1.f)
		m_DeAccel = 1.f;
	else if (DeAccel < 0.f)
		m_DeAccel = 0.f;
	else
		m_DeAccel = DeAccel;
}


void CGameObject::MoveToDest(bool On, EMoveToMethod Method, const Vector2& Dest)
{
	m_MoveToOn = On;
	m_MoveToMethod = Method;
	m_MoveToDest = Dest;
}

bool CGameObject::GetMoveToDone() const
{
	return !m_MoveToOn;
}

void CGameObject::SetNoMove(bool NoMove)
{
	m_NoMove = NoMove;
}

void CGameObject::SetPhysicsSimulate(bool Physics)
{
	m_PhysicsSimulate = Physics;
}

void CGameObject::SetGravityAccel(float Accel)
{
	m_GravityAccel = Accel;
}
void CGameObject::SetJumpVelocity(float JumpVelocity)
{
	m_JumpVelocity = JumpVelocity;
}

void CGameObject::Jump()
{
	if (!m_Jump)
	{
		m_Jump = true;
		m_Ground = false;

		//낙하 시간을 0으로 초기화하고 측정 시작
		m_FallTime = 0.f;
		m_FallStartY = m_Pos.y;
	}
}

void CGameObject::SoundPlay(const std::string& SoundName)
{
	m_Scene->GetSceneResource()->SoundPlay(SoundName);
}

void CGameObject::SoundStop(const std::string& SoundName)
{
	m_Scene->GetSceneResource()->SoundStop(SoundName);
}

bool CGameObject::GetPlaying(const std::string& SoundName)
{
	return m_Scene->GetSceneResource()->GetPlaying(SoundName);
}

void CGameObject::SoundPlayOneChan(const std::string& SoundName)
{
	m_Scene->GetSceneResource()->SoundPlayOneChan(SoundName);
}

void CGameObject::SoundFadeIn(const std::string& SoundName, float DeltaTime)
{
	m_Scene->GetSceneResource()->SoundFadeIn(SoundName, DeltaTime);
}

void CGameObject::SoundFadeOut(const std::string& SoundName, float DeltaTime)
{
	m_Scene->GetSceneResource()->SoundFadeOut(SoundName, DeltaTime);
}





void CGameObject::CreateWidgetComponent()
{
	if (!m_WidgetComponent)
	{
		m_WidgetComponent = m_Scene->CreateWidgetWindow<CWidgetComponent>("WidgetComponent");
	}

	m_WidgetComponent->m_OwnerGameObj = this;

}

CWidgetComponent* CGameObject::GetWidgetComponent() const
{
	return m_WidgetComponent;
}





void CGameObject::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int index)
{
	if (!m_Texture)
		return;

	m_Texture->SetColorKey(r, g, b, index);
}

/////////////////////////// Texture //////////////////////////////////
void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CGameObject::SetTexture(CTexture* Tex)
{
	m_Texture = Tex;
}






void CGameObject::SetTexture(const std::string& Name, const std::tstring& FileName, const std::string& PathName)
{
	//일단 로드를 먼저 시도
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	//로드가 됐든 안됐든 간에 Text를 찾아 등록. 기존의 텍스처가 있다면
	//위에서 로드가 안 됐어도 기존 텍스처를 등록할 것이다.
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

}

void CGameObject::SetTextureFullPath(const std::string& Name, const std::tstring& FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	
}


void CGameObject::SetTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CGameObject::SetTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	m_Size = m_Texture->GetTextureSize();
}





bool CGameObject::LoadCSVPreset(
	const std::tstring& CSVFileName,
	const std::string& PathName
)
{
	if (!m_Animation)
		CreateAnimation();

	std::string FullPath = CPathManager::GetInst()->FindPath(PathName)->PathMultiByte;

	std::string CSVFileNameA;

#ifdef _UNICODE
	char FullPathATemp[MAX_PATH] = {};
	int FullPathALen = WideCharToMultiByte(CP_ACP, 0, CSVFileName.c_str(), -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, CSVFileName.c_str(), -1, FullPathATemp, FullPathALen, 0, 0);
	CSVFileNameA = FullPathATemp;
#else
	CSVFileNameA = FullPath;
#endif	//_UNICODE

	FullPath += CSVFileNameA;

	CCSVEditor* CSVEditor = new CCSVEditor;
	if (!CSVEditor->ReadFile(FullPath))
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}


	int TexInfoRow = (int)ERowInfo::TexInfo;
	//csv의 두번째행 첫번째 열에는 텍스처 이름이 적혀있다.
	std::string TexName = CSVEditor->GetCell(TexInfoRow, 0);

	//첫행 두번째행 두번째 열에는 애니메이션 파일명이 적혀있다.
	std::tstring FileName;
#ifdef _UNICODE
	FileName.assign(
		CSVEditor->GetCell(TexInfoRow, 1).begin(),
		CSVEditor->GetCell(TexInfoRow, 1).end()
	);
#else
	FileName = m_CSVEditor->GetCell(TexInfoRow, 1);
#endif


	//첫행 두번째행 5번째 열(1, 4)에는 히트이펙트 채널을 사용할것인지 여부가 적혀있다.
	std::string EReactionChannelstr = CSVEditor->GetCell(TexInfoRow, 4);
	int EReactionChannelFlag = 0;
	sscanf_s(EReactionChannelstr.c_str(), "%d", &EReactionChannelFlag);

	//텍스처를 최대로 지정한 EReactionChannel까지 로드한다.
	m_MaxReactionChannel = (EReactionChannel)EReactionChannelFlag;


	if (m_Scene->GetSceneResource()->LoadTexture(TexName, FileName, TEXTURE_PATH, m_MaxReactionChannel))
	{
		//컬러키가 들어있는 셀
		std::string src = CSVEditor->GetCell(TexInfoRow, 2);

		int R = 255;
		int G = 0;
		int B = 255;

		int SlashCount = 0;
		for (size_t k = 0; k < src.size(); ++k)
		{
			if (src[k] == '/')
				++SlashCount;
		}
		//컬러키 RGB값이 정상적으로 입력되었을 경우
		if (SlashCount == 2)
		{
			sscanf_s(src.c_str(), "%d/%d/%d", &R, &G, &B);
		}
		m_Scene->GetSceneResource()->SetColorKey(TexName, R, G, B);
	}
	else
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}

	//네 번째 행 첫번쨰 열부터 만들어야 할 애니메이션 시퀀스 이름이 적혀있다.
	int rowcount =(int)CSVEditor->GetRowCount();
	for (int i = (int)ERowInfo::AnimSeqInfoStart; i < rowcount; ++i)
	{
		if (CSVEditor->GetRow(i).empty())
			continue;

		//가장 첫번째 열은 만들 시퀀스의 이름이 적혀 있다.
		std::string AnimName = CSVEditor->GetCell(i, (int)EColInfo::AnimSeqName);

		//애니메이션 시퀀스 생성에 도로 실패하면 삭제하고 return
		if (!m_Scene->GetSceneResource()->
			CreateAnimationSequence(AnimName, TexName))
		{
			SAFE_DELETE(CSVEditor);
			return false;
		}

		//AnimationInfo를 생성하기 위한 변수들 선언
		float PlayTime = 1.f;
		int LoopFlag = 0;
		bool Reverse = false;
		float PlayScale = 1.f;
		int Layer = 0;
		float OffsetX = 0.f;
		float OffsetY = 0.f;
		float PivotX = 0.5f;
		float PivotY = 1.f;

		int rowsize = 0;

		//이미 로드되었음이 확인되면 시퀀스 부분은 로드하지 않음.
		if (m_Scene->GetSceneResource()->CheckAnimLoaded(AnimName))
			rowsize = (int)EColInfo::FramePosStart;
		else
			rowsize = (int)CSVEditor->GetRow(i).size();

		//먼저 애니메이션 Sequence에 대한 정보를 등록한다.
		for (
			int j = (int)EColInfo::AnimSeqName + 1;
			j < rowsize;
			j++
			)
		{
			std::string src = CSVEditor->GetCell(i, j);

			//만약 아무 내용도 입력되어있지 않다면 continue
			//AnimInfo 인자값에도 공란이 들어올 수 있으므로 해당 사항도 체크
			if (src == "")
				continue;

			else if (//애니메이션 Info 정보란 처리
				j > (int)EColInfo::AnimSeqName
				&& j < (int)EColInfo::FramePosStart
				)
			{
				int temp = 0;

				switch (j)
				{
				case (int)EColInfo::PlayTime://PlayTime
					sscanf_s(src.c_str(), "%f", &PlayTime);
					break;
				case (int)EColInfo::LoopFlag:
					sscanf_s(src.c_str(), "%d", &LoopFlag);
					break;
				case (int)EColInfo::Reverse://Reverse:버퍼오버런 방지를 위해 byte 타입으로
					sscanf_s(src.c_str(), "%d", &temp);
					Reverse = temp;
					break;
				case (int)EColInfo::PlayScale://PlayScale
					sscanf_s(src.c_str(), "%f", &PlayScale);
					break;
				case (int)EColInfo::Layer://Layer
					sscanf_s(src.c_str(), "%d", &Layer);
					break;
				case (int)EColInfo::OffsetX://OffsetX
					sscanf_s(src.c_str(), "%f", &OffsetX);
					break;
				case (int)EColInfo::OffsetY://OffsetY
					sscanf_s(src.c_str(), "%f", &OffsetY);
					break;
				case (int)EColInfo::PivotX://PivotX
					sscanf_s(src.c_str(), "%f", &PivotX);
					break;
				case (int)EColInfo::PivotY://PivotY
					sscanf_s(src.c_str(), "%f", &PivotY);
					break;

				default:
					break;
				}
			}
			
			//해당 행의 좌표 부분 로드
			else if (j >= (int)EColInfo::FramePosStart)
			{

				Vector2 Start;
				Vector2 End;
				Vector2 Offset;

				//슬래쉬가 4개 이상이면 Offset도 등록되어있다는 뜻이므로 Offset값도 전달하여 생성한다.
				int SlashCount = 0;
				for (size_t k = 0; k < src.size(); ++k)
				{
					if (src[k] == '/')
						++SlashCount;
				}

				//입력 안된 부분이 나오면 return;
				if (SlashCount < 3)
					continue;
				//스프라이트 좌표값만 입력되었을경우
				else if (SlashCount == 3)
				{
					sscanf_s(src.c_str(), "%f/%f/%f/%f", &Start.x, &Start.y, &End.x, &End.y);
				}
				//오프셋 값까지 입력됐을경우
				else if (SlashCount > 3)
				{
					sscanf_s(src.c_str(), "%f/%f/%f/%f/%f/%f",
						&Start.x, &Start.y, &End.x, &End.y, &Offset.x, &Offset.y);
				}


				//프레임 데이터 추가
				if (!m_Scene->GetSceneResource()->AddAnimationSpriteFrame(AnimName,
					Start, End, Offset))
				{
					SAFE_DELETE(CSVEditor);
					return false;
				}
			}

			//애니메이션 시퀀스에 로드되었음을 전달
			m_Scene->GetSceneResource()->SetAnimLoaded(AnimName);
		}

		//그 다음 Animation Info 정보를 등록한다.
		//한 행에 대한 순회가 완료되면 해당 애니메이션에 대한 데이터 추가
		AddAnimationInfo(
			AnimName,
			PlayTime,
			(ELoopFlag)LoopFlag,
			Reverse,
			PlayScale,
			Layer,
			OffsetX,
			OffsetY,
			PivotX,
			PivotY);

		
	}

	SAFE_DELETE(CSVEditor);
	return true;
}

//////////////////////////// ANIMATION ///////////////////////////////
void CGameObject::CreateAnimation(bool UseHitEffectChannel)
{
	//히트이펙트 채널까지 사용할 지 여부 지정.
	m_Animation = new CAnimation;
	m_Animation->m_OwnerObj = this;
	m_Animation->m_Scene = m_Scene;

}

void CGameObject::ResetAnimation()
{
	SAFE_DELETE(m_Animation);
	CreateAnimation();
}

void CGameObject::AddAnimationInfo(const std::string& AnimName, float PlayTime, ELoopFlag LoopFlag, bool Reverse, float PlayScale, int Layer, float OffsetX, float OffsetY, float PivotX, float PivotY)
{
	if (m_Animation)
		m_Animation->AddAnimationInfo(AnimName, PlayTime, LoopFlag, Reverse, PlayScale, Layer, OffsetX, OffsetY, PivotX, PivotY);
}

void CGameObject::SetFrame(const std::string& AnimName, int Frame)
{
	if (m_Animation)
		m_Animation->SetFrame(AnimName, Frame);
}

void CGameObject::SetPlayTime(const std::string& AnimName, float PlayTime)
{
	if (m_Animation)
		m_Animation->SetPlayTime(AnimName, PlayTime);
}

void CGameObject::SetPlayScale(const std::string& AnimName, float PlayScale)
{
	if (m_Animation)
		m_Animation->SetPlayScale(AnimName, PlayScale);
}

void CGameObject::SetPlayLoopFlag(const std::string& AnimName, ELoopFlag LoopFlag)
{
	if (m_Animation)
		m_Animation->SetPlayLoopFlag(AnimName, LoopFlag);
}

void CGameObject::SetPlayReverse(const std::string& AnimName, bool Reverse, bool Once)
{
	if (m_Animation)
		m_Animation->SetPlayReverse(AnimName, Reverse, Once);
}

void CGameObject::SetLayer(const std::string& AnimName, int Layer)
{
	if (m_Animation)
	{
		m_Animation->SetLayer(AnimName, Layer);
		m_Animation->ChangeAnimation(AnimName);
	}
}

void CGameObject::SetAnimOffset(const std::string& AnimName, float OffsetX, float OffsetY)
{
	if (m_Animation)
		m_Animation->SetOffset(AnimName, OffsetX, OffsetY);
}

void CGameObject::SetPivot(const std::string& AnimName,
	float PivotX, float PivotY)
{
	if (m_Animation)
		m_Animation->SetPivot(AnimName, PivotX, PivotY);
}

void CGameObject::StopAnimation(const std::string& AnimName)
{
	if (m_Animation)
		m_Animation->StopAnimation(AnimName);
}

void CGameObject::StopAnimation(int Layer)
{
	if (m_Animation)
		m_Animation->StopAnimation(Layer);
}

void CGameObject::SetAnimation(const std::string& AnimName, bool PlayOnce)
{
	if (m_Animation)
		m_Animation->SetAnimation(AnimName, PlayOnce);
}

void CGameObject::ChangeAnimation(const std::string& AnimName, bool PlayOnce)
{
	if (m_Animation)
		m_Animation->ChangeAnimation(AnimName, PlayOnce);
}

void CGameObject::SetAnimationDuration(const std::string& AnimName, float Duration)
{
	if (m_Animation)
		m_Animation->SetAnimationDuration(AnimName, Duration);
}

void CGameObject::ChangeAnimationDuration(const std::string& AnimName, float Duration)
{
	if (m_Animation)
		m_Animation->ChangeAnimationDuration(AnimName, Duration);
}

void CGameObject::ChangeAnimContinue(const std::string& AnimName, bool PlayOnce)
{
	if (m_Animation)
		m_Animation->ChangeAnimContinue(AnimName, PlayOnce);
}

bool CGameObject::CheckAnimationPlaying(const std::string& AnimName)
{
	if (m_Animation)
		return m_Animation->CheckAnimationPlaying(AnimName);

	return false;
}

void CGameObject::CheckSkillCooltime(int SkillEnumEnd, float DeltaTime)
{
	//등록된 스킬의 수만큼 순회를 돈다.
	for (size_t i = 0; i < SkillEnumEnd; ++i)
	{
		//만약 스킬의 쿨타임이면,
		if (m_vecCooltime[i].isCooltime)
		{
			//스킬의 쿨타임의 남은 시간을 DeltaTime만큼 감소시킨다.
			m_vecCooltime[i].CooltimeLeft -= DeltaTime;

			//만약 스킬 쿨타임이 다 돌았으면
			if (m_vecCooltime[i].CooltimeLeft <= 0.f)
			{
				//스킬을 다시 사용 가능하도록 활성화시킨다.
				m_vecCooltime[i].isCooltime = false;
			}
		}
	}
}

//스킬 사용시 이 함수에 스킬의 Enum을 넣으면 편하게 처리 가능
void CGameObject::EnterSkillCoolTIme(int SkillEnum)
{
	//스킬이 쿨타임에 진입했다고 바꾸고
	m_vecCooltime[SkillEnum].isCooltime = true;

	//스킬의 쿨타임 남은 시간을 초기화한다.
	m_vecCooltime[SkillEnum].CooltimeLeft = m_vecCooltime[SkillEnum].Cooltime;
}














void CGameObject::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	if (!m_isAccel)
	{
		m_Speed *= m_DeAccel;
	}
	m_isAccel = false;

	if (m_Speed > 0.f)
	{
		m_Speed = min(m_MaxSpeed, m_Speed);
	}
	else if (m_Speed < 0.f)
	{
		m_Speed = max(-m_MaxSpeed, m_Speed);
	}

	//이동했음이 확인되면 방향을 지속적으로 구한다
	if ((m_Pos - m_PrevPos).fabs() >= 0.01f)
		m_CalcDir = (m_Pos - m_PrevPos).Normalize();

	m_Dir = m_Dir.Normalize();
	m_Pos += m_Dir * m_Speed * DeltaTime;

	if (m_MoveToOn)
	{
	
		//서로의 방향 벡터가 역벡터가 되는 순간 정지(이동 완료)
		Vector2 PrevDir = (m_MoveToDest - m_PrevPos).Normalize();
		Vector2 CurrDir = (m_MoveToDest - m_Pos).Normalize();

		PrevDir += CurrDir;
		
		if (fabs(PrevDir.x + PrevDir.y) <= 0.1f)
		{
			m_MoveToOn = false;
			m_Pos = m_MoveToDest;
		}
		else
		{
			m_Dir = CurrDir;

			switch (m_MoveToMethod)
			{
			case EMoveToMethod::MoveDir:
			{
				MoveDir(m_Dir);

				//MoveDir같은 경우는 여기서 처리해주어야함.
				Vector2 Sub = m_MoveToDest - m_Pos;
				if (hypotf(Sub.x, Sub.y) < m_MaxSpeed * DeltaTime)
				{
					m_MoveToOn = false;
					m_Pos = m_MoveToDest;
				}

				break;
			}

			case EMoveToMethod::AddForce:
				AddForce(m_MaxSpeed);
				break;
			case EMoveToMethod::AddImpact:
				AddImpact(m_MaxSpeed);
				break;
			}
		}
	}
	

	// 중력 적용
	if (!m_Ground && m_PhysicsSimulate)
	{
		//시간 * 가속도
		m_FallTime += DeltaTime * m_GravityAccel;

		// 9.8 m/s^2
		// t초 후 y값
		// V : 속도 A : 가속도 G : 중력
		// y = V * A - 0.5f * G * t * t
		// 0 = -0.5fGA^2 + VA - y
		float Velocity = 0.f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);
	}

	//변경된 위치를 토대로 위치 차이 계산
	m_MoveDist = m_Pos - m_PrevPos;

	if (m_SideCollOn)
	{
		do 
		{
			UINT8 Count = 0;

			float LeftRightSide = m_Size.x * m_Pivot.x;
			if (m_Pos.x - LeftRightSide < 0.f)
			{
				m_Pos.x = LeftRightSide;
				++Count;
			}

			float TopBottomSide = m_Size.y * m_Pivot.y;
			if (m_Pos.y - TopBottomSide < 0.f)
			{
				m_Pos.y = TopBottomSide;
				++Count;
			}
			if (Count >= 2)
				break;

			LeftRightSide = m_Size.x - LeftRightSide;
			if (m_Pos.x + LeftRightSide > (float)ORIGINAL_GAME_RES_WIDTH)
			{
				m_Pos.x = (float)ORIGINAL_GAME_RES_WIDTH - LeftRightSide;
				++Count;
			}
			if (Count >= 2)
				break;

			TopBottomSide = m_Size.y - TopBottomSide;
			if (m_Pos.y + TopBottomSide > (float)ORIGINAL_GAME_RES_HEIGHT)
			{
				m_Pos.y = (float)ORIGINAL_GAME_RES_HEIGHT - TopBottomSide;
				++Count;
			}

		} while (false);
	}


	if (m_Animation)
		m_Animation->Update(DeltaTime * m_TimeScaleObj);


	//충돌체 리스트가 비어있다면 안해도 됨
	if (!m_listCollider.empty())
	{
		auto iter = m_listCollider.begin();
		auto iterEnd = m_listCollider.end();

		while (iter != iterEnd)
		{

			//만약 충돌체가 제거되기로 예약 되어있었다면
			if (!(*iter)->GetActive())
			{
				//해당 충돌체와 충돌하고 있던 충돌체 리스트를 호출해
				//자신을 지워달라고 요청하고
				(*iter)->ClearCollisionList();

				//그 뒤에 지워준다.
				iter = m_listCollider.erase(iter);
				continue;
			}

			//만약 임시로 비활성화만 되어있다면 처리하지 않고 넘어간다.
			else if (!(*iter)->GetEnable())
			{
				iter++;
				continue;
			}

			(*iter)->Update(DeltaTime);

			++iter;
		}
	}

	//무적시간 계산
	if (m_Invincible > 0.f)
		m_Invincible -= DeltaTime;

	//이동 불가 설정시 이동 X
	if (m_NoMove)
		m_Pos = m_PrevPos;

}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();




	//충돌체 리스트가 비어있다면 안해도 됨
	if (!m_listCollider.empty())
	{
		auto iter = m_listCollider.begin();
		auto iterEnd = m_listCollider.end();

		while (iter != iterEnd)
		{
			//만약 충돌체가 제거되기로 예약 되어있었다면
			if (!(*iter)->GetActive())
			{
				//해당 충돌체와 충돌하고 있던 충돌체 리스트를 비워준 다음
				//(비울 떄 충돌하고 있던 상대방 충돌체에서도 자동적으로 자신을 제거함)
				(*iter)->ClearCollisionList();

				//그 뒤에 지워준다.
				iter = m_listCollider.erase(iter);
				continue;
			}

			//만약 임시로 비활성화만 되어있다면 처리하지 않고 넘어간다.
			else if (!(*iter)->GetEnable())
			{
				iter++;
				continue;
			}
			else
			{
				(*iter)->PostUpdate(DeltaTime);
			}
			

			//PostUpdate가 끝났으면 충돌체의 처리를 담당하는 CSceneCollision에 추가한다.
			//다시 한번 Active 여부를 확인한다.
			if((*iter)->GetActive() && (*iter)->GetEnable())
				m_Scene->GetSceneCollision()->AddCollider(*iter);


			++iter;
		}
	}



	// 바닥에 착지시킨다.
	if (m_PhysicsSimulate && m_MoveDist.y >= 0.f)
	{

	}


	//모든 과정이 끝나면 m_Pos는 이제 PrevPos가 된다.
	m_PrevPos = m_Pos;
}

void CGameObject::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();


	//점멸 여부 계산
	if (m_ReactionTime > 0.f)
	{
		m_ReactionTime -= DeltaTime;
		if (m_FlickerOn)
		{
			m_FlickerFrequency -= DeltaTime;

			if (m_FlickerFrequency < 0.f)
			{
				m_Flicker = !m_Flicker;
				m_FlickerFrequency = 0.05f;
			}
		}
		else
			m_Flicker = false;
	}
	else
	{
		m_Flicker = false;
		m_CurrentReaction = EReactionChannel::Normal;
	}
	

	//만약 점멸 = true이고 EReactionChannel = Normal이면 꺼졌다 켜졌다 반복
	if (!m_Flicker || m_CurrentReaction != EReactionChannel::Normal)
	{
		Vector2 Res;
		Vector2	Pos;
		CCamera* Camera = nullptr;

		if (m_Scene)
		{
			Camera = m_Scene->GetCamera();
			Res = Camera->GetRes();
			Pos = m_Pos - Camera->GetPos();
		}
		else
		{
			Camera = CSceneManager::GetInst()->GetScene()->GetCamera();
			Res = Camera->GetRes();
			Pos = m_Pos - Camera->GetPos();
		}



		//애니메이션이 있다면 애니메이션이 우선
		if (m_Animation)
		{
			for (int i = 0; i <= m_Animation->GetHighestLayer(); ++i)
			{
				if (!m_Animation->m_CurrentAnimation[i])
					continue;


				CAnimationInfo* Current = m_Animation->m_CurrentAnimation[i];
				int CurrentFrame = Current->m_Frame;
				const Sprite_FrameData& FrameData = Current->m_Sequence->GetFrameData(CurrentFrame);

				Vector2 RenderSize = FrameData.End - FrameData.Start;
				//애니메이션 자체의 오프셋 값 + 프레임 자체의 보정 오프셋 값
				Vector2 RenderLT;
				
				if (m_ForceSizeRender)
				{
					RenderLT = Pos - (m_Size * Current->m_Pivot).Round() + Current->m_Offset + FrameData.Offset;
				}
				else
				{
					RenderLT = Pos - (RenderSize * Current->m_Pivot).Round() + Current->m_Offset + FrameData.Offset;
				}
				

				//오브젝트 컬링
				//화면을 안에 있는 CGameObject만 출력해서 최적화 도모
				//화면을 벗어나면 따라다니는 UI도 비활성화
				if (
					m_CullingDeleteFlag 
					& (UINT)ECullingDelete::CullingOn
					)
				{
					if (!Culling(RenderLT, RenderSize, Res))
					{
						if (m_WidgetComponent)
							m_WidgetComponent->SetEnable(false);
						return;
					}
					else if (m_WidgetComponent)
					{
						m_WidgetComponent->SetEnable(true);
					}
				}


				CTexture* Tex = Current->m_Sequence->GetTexture();

				if (Current->m_Sequence->GetTextureType() == ETextureType::Sprite)
				{
					//현재 상태 확인. 및 리액션 결정
					int ReactionNum = 0;
					if (m_MaxReactionChannel > EReactionChannel::Normal && !m_Flicker)
					{
						ReactionNum = (int)m_CurrentReaction;
					}


					if (Current->m_Sequence->GetTexture()->GetColorKeyEnable())
					{

						TransparentBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(ReactionNum),
							(int)FrameData.Start.x, (int)FrameData.Start.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetColorKey(ReactionNum));
					}
					else
					{
						StretchBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(ReactionNum),
							(int)FrameData.Start.x, (int)FrameData.Start.y,
							(int)RenderSize.x, (int)RenderSize.y,
							SRCCOPY);
						//StretchBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)m_Size.x, (int)m_Size.y,
							//Current->m_Sequence->GetTexture()->GetDC(), 0, 0, SRCCOPY, );
					}
				}
				else //ETexturetype::Frame
				{
					if (Current->m_Sequence->GetTexture()->GetColorKeyEnable(CurrentFrame))
					{

						TransparentBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(CurrentFrame), 0, 0,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetColorKey(CurrentFrame));
					}
					else
					{
						StretchBlt(hDC,
							(int)RenderLT.x, (int)RenderLT.y,
							(int)RenderSize.x, (int)RenderSize.y,
							Tex->GetDC(CurrentFrame), 0, 0,
							(int)RenderSize.x, (int)RenderSize.y, SRCCOPY);

						/*BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y, (int)m_Size.x, (int)m_Size.y, Current->m_Sequence->GetTexture()->GetDC(CurrentFrame), 0, 0, SRCCOPY);*/
					}
				}

			}


		}

		else if (m_Texture)
		{
			Vector2 RenderLT = Pos - m_Pivot * m_Size * m_Scale;
			Vector2 RenderSize = m_Texture->GetTextureSize() * m_Scale;

			//오브젝트 컬링
			//화면을 안에 있는 CGameObject만 출력해서 최적화 도모
			//화면을 벗어나면 따라다니는 UI도 비활성화
			if (
				m_CullingDeleteFlag
				& (UINT)ECullingDelete::CullingOn
				)
			{
				if (!Culling(RenderLT, RenderSize, Res))
				{
					if (m_WidgetComponent)
						m_WidgetComponent->SetEnable(false);
					return;
				}
				else if (m_WidgetComponent)
				{
					m_WidgetComponent->SetEnable(true);
				}
			}

			if (m_Texture->GetColorKeyEnable())
			{
				TransparentBlt(hDC,
					(int)RenderLT.x, (int)RenderLT.y,
					(int)RenderSize.x, (int)RenderSize.y,
					m_Texture->GetDC(), 0, 0,
					(int)m_Texture->GetTextureSize().x, (int)m_Texture->GetTextureSize().y,
					m_Texture->GetColorKey());

			}
			else
			{
				StretchBlt(hDC,
					(int)RenderLT.x, (int)RenderLT.y,
					(int)RenderSize.x, (int)RenderSize.y,
					m_Texture->GetDC(), 0, 0,
					(int)m_Texture->GetTextureSize().x, (int)m_Texture->GetTextureSize().y, SRCCOPY);
			}


		}

	}






	if (DEBUG_RENDER)
	{

		//충돌체 리스트가 비어있다면 안해도 됨
		if (!m_listCollider.empty())
		{
			auto iter = m_listCollider.begin();
			auto iterEnd = m_listCollider.end();

			while (iter != iterEnd)
			{
				//만약 충돌체가 제거되기로 예약 되어있었다면
				if (!(*iter)->GetActive())
				{
					iter++;
					continue;
				}

				//만약 임시로 비활성화만 되어있다면 처리하지 않고 넘어간다.
				else if (!(*iter)->GetEnable())
				{
					iter++;
					continue;
				}

				(*iter)->Render(hDC, DeltaTime);

				++iter;
			}
		}
	}
}
