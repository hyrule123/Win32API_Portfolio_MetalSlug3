#pragma once

#include "../Ref.h"
#include "../Resource/Animation/Animation.h"

enum class EStatus
{
	Normal,
	Hit,
	Invincible
};

enum class ECullingDelete : UINT8
{
	Left =	0b00000001,
	Top =	0b00000010,
	Right =	0b00000100,
	Bottom =0b00001000,
	All =	0b00001111,
	CullingOn = 0b10000000
};

enum class EMoveToMethod : UINT8	//이동 방법. 어떤 방식으로 이동할지 정한다.
{
	MoveDir,
	AddForce,
	AddImpact
};





class CGameObject : public CRef
{
	friend class CScene;
	friend class CInput;

protected:
	CGameObject();
	CGameObject(const CGameObject& Obj);
	virtual ~CGameObject();
public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

	//여기서 기본적인 위치 등의 세팅 이후에 해줘야하는 변수들을 설정해준다.
	virtual void Start();
	bool m_Start;


protected://위치, 크기
	Vector2 m_Dir;	//플레이어가 지정할 수 있는 방향
	Vector2 m_CalcDir;	//움직일 때마다 매번 계산되는 방향. 따로 설정 불가
	Vector2 m_Pos;
	Vector2 m_PrevPos;
	Vector2 m_Size;
	bool	m_ForceSizeRender;	//강제로 사이즈를 지정해서 Render시킨다.
	Vector2 m_Pivot;
	float	m_MaxSpeed;	//최대 속도
	float	m_Speed;		//현재 속도
	bool	m_isAccel;		//현재 가속 중인지
	float	m_DeAccel;		//감속도 보정값. 0(바로멈춤), 1(멈추지 않고 이동)
	Vector2 m_MoveDist;

	bool	m_MoveToOn;
	bool	m_SideCollOn;
	
	EMoveToMethod m_MoveToMethod;	//이동 모드 ON
	Vector2 m_MoveToDest;	//해당 위치까지 알아서 이동

public:	//위치
	void SetForceSizeRender(bool On);
	void SetDir(float _x, float _y);
	void SetDirX(float _x);
	void SetDirY(float _y);
	void SetDir(const Vector2& Dir);
	Vector2 GetDir()	const;
	void SetPos(float _x, float _y);
	void SetPos(const Vector2& Vec);
	void SetPosX(float x);
	void SetPosY(float y);
	Vector2 GetPos() const;
	Vector2 GetPrevPos() const;
	void SetMaxSpeed(float);
	Vector2 GetCalcDir() const;
	void SetSideColl(bool On);

	//이동 관련
	void MoveDir(const Vector2& Dir);
	void MovePos(const Vector2& Pos);
	void MoveAngle(float Angle);
	void MoveValue(const Vector2& MoveValue);
	void MoveValue(float _x, float _y);
	void SetSize(float _x, float _y);
	void SetSize(const Vector2& Size);
	Vector2 GetSize()	const;
	void SetPivot(float _x, float _y);
	Vector2 GetPivot()	const;
	void AddForce(float Force);	//DeltaTime을 곱해서 적용. 부드러운 움직임을 줄때
	void AddImpact(float Impact);	//DeltaTime을 곱하지 않고 적용. 강한 충격 또는 즉시 이동
	void SetDeAccel(float DeAccel);
	

	void MoveToDest(bool On, EMoveToMethod Method = EMoveToMethod::MoveDir, const Vector2& Dest = Vector2(0.f,0.f));
	bool GetMoveToDone() const;	//이동 명령중이 아닌지 확인하는 함수. 이동명령중이 아니면 true를 반환.


protected://물리 처리
	bool	m_PhysicsSimulate;
	bool	m_Ground;
	float	m_GravityAccel;
	float	m_FallTime;
	float	m_FallStartY;
	bool	m_Jump;
	float	m_JumpVelocity;
	bool	m_SideWallCheck;
	bool	m_NoMove; //이동 불가 여부

public:
	void SetNoMove(bool NoMove);
	void SetPhysicsSimulate(bool Physics);
	void SetGravityAccel(float Accel);
	void SetJumpVelocity(float JumpVelocity);
	void Jump();

	void SoundPlay(const std::string& SoundName);
	void SoundStop(const std::string& SoundName);
	bool GetPlaying(const std::string& SoundName);
	void SoundPlayOneChan(const std::string& SoundName);
	void SoundFadeIn(const std::string& SoundName, float DeltaTime);
	void SoundFadeOut(const std::string& SoundName, float DeltaTime);




protected://달고 있을 위젯들을 모아둘 위젯 컴포넌트
	CSharedPtr<class CWidgetComponent> m_WidgetComponent;
public:
	void CreateWidgetComponent();
	class CWidgetComponent* GetWidgetComponent() const;


protected://출력 사이즈 조절
	float m_Scale;
public:
	float GetScale() const;
	void SetScale(float Scale);


protected://소속관계
	class CScene* m_Scene;
	class CGameObject* m_MasterObject;
	std::list<CSharedPtr<CGameObject>> m_SlaveObject;
	ERenderLayer m_RenderLayer;
public:
	//렌더링 레이어
	ERenderLayer GetRenderLayer()	const;
	void SetRenderLayer(ERenderLayer Layer);

	//종속 게임오브젝트 등록
	virtual void AddObj(CGameObject* Obj);


	//자신이 소속된 씬
	void SetOwnerScene(class CScene* Scene);
	CScene* GetOwnerScene() const;


	//양방 주종관계 해제.
	void DeleteMasterSlave();

	//이 함수는 받는 쪽에서 처리해주는 함수이다.
	void DeleteSlave(CGameObject* SlavePtr);



protected://각 게임오브젝트별 타임스케일
	float m_TimeScaleObj;
public:
	float GetTimeScaleObj()	const;
	void SetTimeScaleObj(float TimeScaleObj);
	



protected://쿨타임 확인용
		std::vector<CooltimeChecker> m_vecCooltime;
		//이 클래스를 상속받는 클래스들의 Update() 메소드에서 사용.
		//Update()메소드에서 편하게 스킬쿨타임을 체크하기 위해 만든 메소드
		void CheckSkillCooltime(int SkillEnumEnd, float DeltaTime);

		//이 클래스를 상속받는 클래스들이 스킬 쿨타임을 가져야 할 때,
		//해당 스킬 메소드 안에서 사용.
		//스킬 쿨타임을 동작시킨다.
		void EnterSkillCoolTIme(int SkillEnum);


protected:
	float m_Invincible;
public:
	virtual int InflictDamage(int Damage);
	void SetInvincible(float Duration = 2.f, bool Flicker = true);

protected:
	//리소스
	CSharedPtr<class CTexture> m_Texture;
	CAnimation* m_Animation;
	EReactionChannel m_MaxReactionChannel;

	//현재 리액션이 Normal인 상태에서 Flicker를 시작하면 모습이 사라졌다 나타났다 하는 점멸
	EReactionChannel m_CurrentReaction;
	float m_ReactionTime;	//리액션을 지속할 시간 지정
	float m_FlickerFrequency;//맞거나 무적일 떄(리액션 중일떄) 반짝이는 주기 계산용. 0.1초?
	bool m_Flicker;		//플리커 이펙트를 줬을때 점멸 여부를 저장
	bool m_FlickerOn;	//플리커 이펙트를 줄것인지?

public:
	//리액션에 따른 점멸을 시작산다(노말상태에서 점멸 = 모습이 사라졌다 나타났다 함)
	void StartReaction(float ReactionDuration, bool FlickerOn, EReactionChannel Reaction = EReactionChannel::Normal);



protected:
	//충돌체 리스트
	std::list<CSharedPtr<class CCollider>> m_listCollider;
	UINT8 m_CullingDeleteFlag;

public:
	bool Culling(const Vector2& PosLT,const Vector2& Size, const Vector2& Res);

	void SetCulling(bool CullingOn);
	void SetCullingDelete(ECullingDelete Delete, bool On);
	UINT8 GetCullingDelete() const;
	void ClearCollider();	//충돌체 전부 제거
	bool CheckColliding(CGameObject* Obj);	//자신과 충돌중인 게임오브젝트인지 확인

	//자신에게 충돌체를 추가한다.(T -> 충돌체 타입(모양)에 따른 클래스명
	template <typename T>
	T* AddCollider(const std::string& Name)
	{
		T* Collider = new T;

		if (!Collider->Init())
		{
			SAFE_DELETE(Collider);
			return nullptr;
		}

		Collider->SetName(Name);
		Collider->m_OwnerObj = this;
		Collider->m_Scene = m_Scene;

		m_listCollider.push_back((class CCollider*)Collider);

		return Collider;
	}



///////////////////////// ANIMATION ////////////////////////
public:
	bool LoadCSVPreset(
		const std::tstring& CSVFileName,
		const std::string& PathName = DATA_PATH
	);


	void CreateAnimation(bool UseHitEffectChannel = false);	//애니메이션을 동적할당하고 주인 오브젝트와 씬을 등록
	void ResetAnimation(); //에디트씬에서 사용됨.

	void AddAnimationInfo(const std::string& AnimName,
		float PlayTime = 1.f,
		ELoopFlag LoopFlag = ELoopFlag::NoLoop,
		bool Reverse = false,
		float PlayScale = 1.f, 
		int Layer = 0, 
		float OffsetX = 0.f, float OffsetY = 0.f,
		float PivotX = 0.f, float PivotY = 0.f);

	void SetFrame(const std::string& AnimName, int Frame);
	void SetPlayTime(const std::string& AnimName, float PlayTime);
	void SetPlayScale(const std::string& AnimName, float PlayScale);
	void SetPlayLoopFlag(const std::string& AnimName, ELoopFlag LoopFlag);
	void SetPlayReverse(const std::string& AnimName, bool Reverse, bool Once = false);
	void SetLayer(const std::string& AnimName, int Layer);
	void SetAnimOffset(const std::string& AnimName, float OffsetX, float OffsetY);
	void SetPivot(const std::string& AnimName, float OffsetX, float OffsetY);


	void StopAnimation(const std::string& AnimName);
	void StopAnimation(int Layer = 0);

	void SetAnimation(const std::string& AnimName, bool PlayOnce = false);
	void ChangeAnimation(const std::string& AnimName, bool PlayOnce = false);

	void SetAnimationDuration(const std::string& AnimName, float Duration);
	void ChangeAnimationDuration(const std::string& AnimName, float Duration);

	void ChangeAnimContinue(const std::string& AnimName, bool PlayOnce = false);
	bool CheckAnimationPlaying(const std::string& AnimName);

	template <typename T>
	void SetEndFunction(const std::string& AnimName, T* Obj, void (T::* Func)(), bool Once = false)
	{
		if (m_Animation)
			m_Animation->SetEndFunction<T>(AnimName, Obj, Func, Once);

	}

	template <typename T>
	void AddNotify(const std::string& AnimName, int Frame, T* Obj, void(T::* Func)())
	{
		if (m_Animation)
			m_Animation->AddNotify<T>(AnimName, Frame, Obj, Func);
	}
/////////////////////////////////////////////////////////////////////












///////////////////////// Texture ///////////////////////////////
public:
	void SetColorKey(unsigned char r = 255,
		unsigned char g = 0,
		unsigned char b = 255,
		int index = -1);

	void SetTexture(const std::string& Name);
	void SetTexture(class CTexture* Tex);
	void SetTexture(const std::string& Name, const std::tstring& FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const std::tstring& FullPath);


	void SetTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath);

/////////////////////////////////////////////////////////////////////

};
