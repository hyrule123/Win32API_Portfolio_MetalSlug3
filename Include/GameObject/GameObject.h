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

enum class EMoveToMethod : UINT8	//�̵� ���. � ������� �̵����� ���Ѵ�.
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

	//���⼭ �⺻���� ��ġ ���� ���� ���Ŀ� ������ϴ� �������� �������ش�.
	virtual void Start();
	bool m_Start;


protected://��ġ, ũ��
	Vector2 m_Dir;	//�÷��̾ ������ �� �ִ� ����
	Vector2 m_CalcDir;	//������ ������ �Ź� ���Ǵ� ����. ���� ���� �Ұ�
	Vector2 m_Pos;
	Vector2 m_PrevPos;
	Vector2 m_Size;
	bool	m_ForceSizeRender;	//������ ����� �����ؼ� Render��Ų��.
	Vector2 m_Pivot;
	float	m_MaxSpeed;	//�ִ� �ӵ�
	float	m_Speed;		//���� �ӵ�
	bool	m_isAccel;		//���� ���� ������
	float	m_DeAccel;		//���ӵ� ������. 0(�ٷθ���), 1(������ �ʰ� �̵�)
	Vector2 m_MoveDist;

	bool	m_MoveToOn;
	bool	m_SideCollOn;
	
	EMoveToMethod m_MoveToMethod;	//�̵� ��� ON
	Vector2 m_MoveToDest;	//�ش� ��ġ���� �˾Ƽ� �̵�

public:	//��ġ
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

	//�̵� ����
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
	void AddForce(float Force);	//DeltaTime�� ���ؼ� ����. �ε巯�� �������� �ٶ�
	void AddImpact(float Impact);	//DeltaTime�� ������ �ʰ� ����. ���� ��� �Ǵ� ��� �̵�
	void SetDeAccel(float DeAccel);
	

	void MoveToDest(bool On, EMoveToMethod Method = EMoveToMethod::MoveDir, const Vector2& Dest = Vector2(0.f,0.f));
	bool GetMoveToDone() const;	//�̵� ������� �ƴ��� Ȯ���ϴ� �Լ�. �̵�������� �ƴϸ� true�� ��ȯ.


protected://���� ó��
	bool	m_PhysicsSimulate;
	bool	m_Ground;
	float	m_GravityAccel;
	float	m_FallTime;
	float	m_FallStartY;
	bool	m_Jump;
	float	m_JumpVelocity;
	bool	m_SideWallCheck;
	bool	m_NoMove; //�̵� �Ұ� ����

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




protected://�ް� ���� �������� ��Ƶ� ���� ������Ʈ
	CSharedPtr<class CWidgetComponent> m_WidgetComponent;
public:
	void CreateWidgetComponent();
	class CWidgetComponent* GetWidgetComponent() const;


protected://��� ������ ����
	float m_Scale;
public:
	float GetScale() const;
	void SetScale(float Scale);


protected://�ҼӰ���
	class CScene* m_Scene;
	class CGameObject* m_MasterObject;
	std::list<CSharedPtr<CGameObject>> m_SlaveObject;
	ERenderLayer m_RenderLayer;
public:
	//������ ���̾�
	ERenderLayer GetRenderLayer()	const;
	void SetRenderLayer(ERenderLayer Layer);

	//���� ���ӿ�����Ʈ ���
	virtual void AddObj(CGameObject* Obj);


	//�ڽ��� �Ҽӵ� ��
	void SetOwnerScene(class CScene* Scene);
	CScene* GetOwnerScene() const;


	//��� �������� ����.
	void DeleteMasterSlave();

	//�� �Լ��� �޴� �ʿ��� ó�����ִ� �Լ��̴�.
	void DeleteSlave(CGameObject* SlavePtr);



protected://�� ���ӿ�����Ʈ�� Ÿ�ӽ�����
	float m_TimeScaleObj;
public:
	float GetTimeScaleObj()	const;
	void SetTimeScaleObj(float TimeScaleObj);
	



protected://��Ÿ�� Ȯ�ο�
		std::vector<CooltimeChecker> m_vecCooltime;
		//�� Ŭ������ ��ӹ޴� Ŭ�������� Update() �޼ҵ忡�� ���.
		//Update()�޼ҵ忡�� ���ϰ� ��ų��Ÿ���� üũ�ϱ� ���� ���� �޼ҵ�
		void CheckSkillCooltime(int SkillEnumEnd, float DeltaTime);

		//�� Ŭ������ ��ӹ޴� Ŭ�������� ��ų ��Ÿ���� ������ �� ��,
		//�ش� ��ų �޼ҵ� �ȿ��� ���.
		//��ų ��Ÿ���� ���۽�Ų��.
		void EnterSkillCoolTIme(int SkillEnum);


protected:
	float m_Invincible;
public:
	virtual int InflictDamage(int Damage);
	void SetInvincible(float Duration = 2.f, bool Flicker = true);

protected:
	//���ҽ�
	CSharedPtr<class CTexture> m_Texture;
	CAnimation* m_Animation;
	EReactionChannel m_MaxReactionChannel;

	//���� ���׼��� Normal�� ���¿��� Flicker�� �����ϸ� ����� ������� ��Ÿ���� �ϴ� ����
	EReactionChannel m_CurrentReaction;
	float m_ReactionTime;	//���׼��� ������ �ð� ����
	float m_FlickerFrequency;//�°ų� ������ ��(���׼� ���ϋ�) ��¦�̴� �ֱ� ����. 0.1��?
	bool m_Flicker;		//�ø�Ŀ ����Ʈ�� ������ ���� ���θ� ����
	bool m_FlickerOn;	//�ø�Ŀ ����Ʈ�� �ٰ�����?

public:
	//���׼ǿ� ���� ������ ���ۻ��(�븻���¿��� ���� = ����� ������� ��Ÿ���� ��)
	void StartReaction(float ReactionDuration, bool FlickerOn, EReactionChannel Reaction = EReactionChannel::Normal);



protected:
	//�浹ü ����Ʈ
	std::list<CSharedPtr<class CCollider>> m_listCollider;
	UINT8 m_CullingDeleteFlag;

public:
	bool Culling(const Vector2& PosLT,const Vector2& Size, const Vector2& Res);

	void SetCulling(bool CullingOn);
	void SetCullingDelete(ECullingDelete Delete, bool On);
	UINT8 GetCullingDelete() const;
	void ClearCollider();	//�浹ü ���� ����
	bool CheckColliding(CGameObject* Obj);	//�ڽŰ� �浹���� ���ӿ�����Ʈ���� Ȯ��

	//�ڽſ��� �浹ü�� �߰��Ѵ�.(T -> �浹ü Ÿ��(���)�� ���� Ŭ������
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


	void CreateAnimation(bool UseHitEffectChannel = false);	//�ִϸ��̼��� �����Ҵ��ϰ� ���� ������Ʈ�� ���� ���
	void ResetAnimation(); //����Ʈ������ ����.

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
