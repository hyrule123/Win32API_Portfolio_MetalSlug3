#include "TextureManager.h"

#include "Texture.h"

#include "../../Ref.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

bool CTextureManager::Init()
{
	return true;
}

bool CTextureManager::SetColorKey(const std::string& Name, 
	unsigned char r, unsigned char g, unsigned char b, 
	int index)
{
	CTexture* Tex = FindTexture(Name);

	if (!Tex)
		return false;

	Tex->SetColorKey(r, g, b, index);

	return true;
}


CTexture* CTextureManager::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::ReleaseTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);

}


bool CTextureManager::LoadTexture(const std::string& Name, const std::tstring& FileName, const std::string& PathName, EReactionChannel MaxChannel)
{

	//만약 해당 이름으로 생성된 텍스처가 있다면 그대로 return 한다.
	if (FindTexture(Name))
		return false;


	//통과했으면 새 텍스처를 동적 할당한다.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);


	//텍스처 로드를 시도하고, 실패하면 도로 삭제하고 false 리턴
	if (!NewTex->LoadTexture(FileName, PathName, MaxChannel))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//성공했으면 map에 저장
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, const std::tstring& FullPath, EReactionChannel MaxChannel)
{
	//만약 해당 이름으로 생성된 텍스처가 있다면 있는 것이므로 만들어졌다고 알려주고 return
	if (FindTexture(Name))
		return true;


	//통과했으면 새 텍스처를 동적 할당한다.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//텍스처 로드를 시도하고, 실패하면 도로 삭제하고 false 리턴
	if (!NewTex->LoadTextureFullPath(FullPath, MaxChannel))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//성공했으면 map에 저장
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}


bool CTextureManager::LoadTextureByTextureNumbers(const std::string& Name,
	const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName)
{
	//만약 해당 이름으로 생성된 텍스처가 있다면 그대로 return 한다.
	if (FindTexture(Name))
		return false;


	//통과했으면 새 텍스처를 동적 할당한다.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//텍스처 로드를 시도하고, 실패하면 도로 삭제하고 false 리턴
	if (!NewTex->LoadTextureByTextureNumbers(FolderPath, NumberOfTexture, PathName))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//파일 이름을 벡터에 넣어서 로드할 경우 프레임 타입일수밖에 없다.
	NewTex->m_TextureType = ETextureType::Frame;

	//성공했으면 map에 저장
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;

}



bool CTextureManager::LoadTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	//만약 해당 이름으로 생성된 텍스처가 있다면 그대로 return 한다.
	if (FindTexture(Name))
		return false;


	//통과했으면 새 텍스처를 동적 할당한다.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//텍스처 로드를 시도하고, 실패하면 도로 삭제하고 false 리턴
	if (!NewTex->LoadTexture(vecFileName, PathName))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//파일 이름을 벡터에 넣어서 로드할 경우 프레임 타입일수밖에 없다.
	NewTex->m_TextureType = ETextureType::Frame;

	//성공했으면 map에 저장
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath)
{
	//만약 해당 이름으로 생성된 텍스처가 있다면 그대로 return 한다.
	if (FindTexture(Name))
		return false;


	//통과했으면 새 텍스처를 동적 할당한다.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//텍스처 로드를 시도하고, 실패하면 도로 삭제하고 false 리턴
	if (!NewTex->LoadTextureFullPath(vecFullPath))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//파일 이름을 벡터에 넣어서 로드할 경우 프레임 타입일수밖에 없다.
	NewTex->m_TextureType = ETextureType::Frame;

	//성공했으면 map에 저장
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}

CTexture* CTextureManager::LoadTexture(FILE* File)
{
	// 같은 이름으로 저장된게 있다면 잘못된것이다.
	CTexture* Texture = new CTexture;


	Texture->Load(File);

	CTexture* Find = FindTexture(Texture->GetName());

	if (Find)
	{
		SAFE_DELETE(Texture);
		return Find;
	}

	m_mapTexture.insert(std::make_pair(Texture->GetName(), Texture));

	return Texture;
}






