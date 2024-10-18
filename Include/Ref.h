#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

	void AddRef();
	void Release();

protected:
	int m_RefCount;
	size_t m_TypeID;
	std::string m_TypeName;
	std::string m_Name;


	//이게 false가 되면 삭제 대기에 들어감
	bool m_Active;
	//이게 false가 되면 일시적으로만 비활성화
	bool m_Enable;

public:
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	int GetRefCount()	const;
	void SetName(const std::string& Name);
	const std::string& GetName() const;
	size_t GetTypeID() const;
	const std::string& GetTypeName() const;
	void SetActive(bool Active);
	void SetEnable(bool Enable);
	bool GetActive() const;
	bool GetEnable() const;

	//T 템를릿으로 들어온 타입과 같은 타입인지 체크
	template <typename T>
	bool CheckTypeID() const
	{
		return m_TypeID == typeid(T).hash_code();
	}

	template <typename T>
	void SetTypeID()
	{
		m_TypeName = typeid(T).name();
		m_TypeID = typeid(T).hash_code();
	}

};

