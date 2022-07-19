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


protected://���ҽ� �����ڿ� ����
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


protected: // ī�޶� ����
	//���� ������� ī�޶� Ŭ����
	class CCamera* m_Camera;
	//ī�޶��� ��ġ ����� - ī�޶��� ��ġ�� ������ CScene���� ���´�.
	Vector2 m_CamPos;
	Vector2 m_CamRes;
public:
	void SetCamTarget(CGameObject* Obj);
	const Vector2& GetCamPos()	const;
	const Vector2& GetCamRes() const;
	class CCamera* GetCamera() const;
	//=====================================================================

protected:	//�浹ü ���� Ŭ����
	class CSceneCollision* m_Collision;
public:
	class CSceneCollision* GetSceneCollision()	const;
//=====================================================================


protected://���� ����
	std::vector<CSharedPtr<class CWidgetWindow>> m_vecWidgetWindow;

	//Zorder '��������' ������� ���� -> �浹 ó���� ������������ �ϱ⶧��
	//�ѹ� �����س���, �浹 üũ���� ����, ������ ���� �������� ����ϸ� �ɵ�.
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



protected://���ӿ�����Ʈ ����
	std::unordered_map<size_t, CSharedPtr<class CGameObject>> 
		m_mapOriginalObj;	//������ ������ ����Ʈ

	//���纻(���� ������ ����) ������Ʈ��
	std::list<CSharedPtr<class CGameObject>> m_ObjList[(int)ERenderLayer::Max];
public:
	bool ChangeRenderLayer(class CGameObject* Obj, ERenderLayer SrcLayer, ERenderLayer DestLayer);
	CGameObject* FindObject(const std::string& Name);

	//���� �����ڷ� �����ؼ� ����� �⺻ ������Ʈ ����. ������ �̸��� �ݵ�� �Է��ؾ��Ѵ�.
	template <typename T>
	T* CreateOriginalObj(const std::string& Name)
	{
		//���� TŸ���� ���ӿ�����Ʈ�� �����Ѵ�.
		T* Obj = new T;

		//�ڽ��� ������ CScene�� ����Ѵ�.
		Obj->SetOwnerScene((CScene*)this);
		//�̸��� �������ش�.
		Obj->SetName(Name);

		//���ҽ��� �ε��ϰ� �����ϸ� �����ϰ� ������ ���ư���.
		if (!Obj->LoadResource())
		{
			//��� ������� �������� ���� ���� �����޴� ���� �����Ƿ� SAFE_DELETE
			SAFE_DELETE(Obj);
			return nullptr;
		}

		//������ ���� ���ӿ�����Ʈ�� �����ϴ� m_mapOriginalObj�� ��ĳ�����Ͽ� �����Ѵ�.
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
		//���� ���� ����Ʈ�� ������ �����Ѵ�. ���� �� return.
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

		//������Ʈ�� �̸��� �����Ѵ�.
		Obj->SetName(Name);

		//�ʱ�ȭ �Լ��� �۵���Ű�� �����ϸ� �ٽ� �����ϰ� nullptr�� ��ȯ�Ѵ�.
		if (!Obj->Init((CGameObject*)Master))
		{
			//��� ������� �������� ���� ���� �����޴� ���� �����Ƿ� SAFE_DELETE
			SAFE_DELETE(Obj);
			return nullptr;
		}


		//������ ���ӿ�����Ʈ�� �����ϴ� m_ObjList�� ��ĳ�����Ͽ� �����Ѵ�.
		m_ObjList[(int)Obj->m_RenderLayer].push_back((CGameObject*)Obj);

		//���� ������Ʈ�� ���� ��� ���� ������Ʈ�� �ּҵ� ��ĳ�����Ͽ� �����Ѵ�.
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
	//y������ ���� ���� ���� �޼ҵ�
	static bool SortY(const CSharedPtr<class CGameObject>& Src, const CSharedPtr<class CGameObject>& Dest);




};

