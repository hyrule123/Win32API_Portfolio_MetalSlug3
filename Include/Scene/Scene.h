#pragma once

#include "../GameInfo.h"
#include "../GameObject/GameObject.h"
#include "../UI/WidgetWindow.h"




class CScene
{
	friend class CSceneManager;
	  
protected:
	CScene();
	virtual ~CScene();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);


protected://리소스 관리자와 연결
	class CSceneResource* m_SceneResource;
public:
	class CSceneResource* GetSceneResource() const;
//=====================================================================

protected:
	class CPlayer* m_Player;
public:
	class CPlayer* GetPlayer() const;
	void SetPlayer(class CPlayer* Player);
	void DeletePlayer(class CPlayer* Player);
	virtual void SetEssential();


protected: // 카메라 관련
	//씬이 들고있을 카메라 클래스
	class CCamera* m_Camera;
	//카메라의 위치 저장용 - 카메라의 위치를 앞으로 CScene에서 얻어온다.
	Vector2 m_CamPos;
	Vector2 m_CamRes;
public:
	void SetCamTarget(CGameObject* Obj);
	const Vector2& GetCamPos()	const;
	const Vector2& GetCamRes() const;
	class CCamera* GetCamera() const;
	//=====================================================================

protected:	//충돌체 관리 클래스
	class CSceneCollision* m_Collision;
public:
	class CSceneCollision* GetSceneCollision()	const;
//=====================================================================


protected://위젯 관련
	std::vector<CSharedPtr<class CWidgetWindow>> m_vecWidgetWindow;

	//Zorder '내림차순' 순서대로 정렬 -> 충돌 처리는 내림차순부터 하기때문
	//한번 정렬해놓고, 충돌 체크때는 순차, 렌더링 때는 역순으로 출력하면 될듯.
	static bool SortWidgetWindow(const CSharedPtr<class CWidgetWindow>& Src,
		const CSharedPtr<class CWidgetWindow>& Dest);
public:
	CWidgetWindow* FindWidgetWindow(const std::string& Name);

	template <typename T>
	T* CreateWidgetWindow(const std::string& Name = "Window")
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->m_Scene = this;

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_vecWidgetWindow.push_back(Window);

		return (T*)Window;
	}
//=====================================================================



protected://게임오브젝트 관련
	std::unordered_map<size_t, CSharedPtr<class CGameObject>> 
		m_mapOriginalObj;	//원본을 저장할 리스트

	//복사본(실제 로직에 들어가는) 오브젝트들
	std::list<CSharedPtr<class CGameObject>> m_ObjList[(int)ERenderLayer::Max];
public:
	bool ChangeRenderLayer(class CGameObject* Obj, ERenderLayer SrcLayer, ERenderLayer DestLayer);
	CGameObject* FindObject(const std::string& Name);

	//복사 생성자로 복제해서 사용할 기본 오브젝트 생성. 원본은 이름을 반드시 입력해야한다.
	template <typename T>
	T* CreateOriginalObj(const std::string& Name)
	{
		//들어온 T타입의 게임오브젝트를 생성한다.
		T* Obj = new T;

		//자신을 생성한 CScene을 등록한다.
		Obj->SetOwnerScene((CScene*)this);
		//이름을 설정해준다.
		Obj->SetName(Name);

		//리소스를 로드하고 실패하면 삭제하고 원래로 돌아간다.
		if (!Obj->LoadResource())
		{
			//방금 만들어진 변수여서 굳이 딴데 참조받는 곳이 없으므로 SAFE_DELETE
			SAFE_DELETE(Obj);
			return nullptr;
		}

		//생성된 원본 게임오브젝트를 관리하는 m_mapOriginalObj에 업캐스팅하여 삽입한다.
		m_mapOriginalObj.insert(std::make_pair(Obj->GetTypeID(), (CGameObject*)Obj));

		return Obj;
	}

	template <typename T>
	T* CreateObject(const std::string& Name = "GameObject",
		void* Master = nullptr)
	{
		size_t Key = typeid(T).hash_code();

		auto iter = m_mapOriginalObj.find(Key);

		T* FoundObj = nullptr;
		//만약 원본 리스트에 없으면 생성한다. 실패 시 return.
		if (iter == m_mapOriginalObj.end())
		{
			FoundObj = CreateOriginalObj<T>(Name);

			if (!FoundObj)
				return nullptr;
		}
		else
		{
			FoundObj = (T*)iter->second.Get();
		}

		T* Obj = new T(*FoundObj);

		//오브젝트에 이름을 설정한다.
		Obj->SetName(Name);

		//초기화 함수를 작동시키고 실패하면 다시 제거하고 nullptr을 반환한다.
		if (!Obj->Init((CGameObject*)Master))
		{
			//방금 만들어진 변수여서 굳이 딴데 참조받는 곳이 없으므로 SAFE_DELETE
			SAFE_DELETE(Obj);
			return nullptr;
		}


		//생성된 게임오브젝트를 관리하는 m_ObjList에 업캐스팅하여 삽입한다.
		m_ObjList[(int)Obj->m_RenderLayer].push_back((CGameObject*)Obj);

		//주인 오브젝트가 있을 경우 주인 오브젝트의 주소도 업캐스팅하여 삽입한다.
		if (Master)
		{
			AddSlave<CGameObject>((CGameObject*)Master, (CGameObject*)Obj);
		}

		return Obj;
	}

	template <typename T>
	void AddSlave(T* Master, T* Obj)
	{
		Master->AddObj((CGameObject*)Obj);
	}
//=====================================================================



protected:
	//y소팅을 위한 정렬 기준 메소드
	static bool SortY(const CSharedPtr<class CGameObject>& Src, const CSharedPtr<class CGameObject>& Dest);




};

