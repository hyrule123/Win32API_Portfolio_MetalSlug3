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

	//���� �ش� �̸����� ������ �ؽ�ó�� �ִٸ� �״�� return �Ѵ�.
	if (FindTexture(Name))
		return false;


	//��������� �� �ؽ�ó�� ���� �Ҵ��Ѵ�.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);


	//�ؽ�ó �ε带 �õ��ϰ�, �����ϸ� ���� �����ϰ� false ����
	if (!NewTex->LoadTexture(FileName, PathName, MaxChannel))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//���������� map�� ����
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, const std::tstring& FullPath, EReactionChannel MaxChannel)
{
	//���� �ش� �̸����� ������ �ؽ�ó�� �ִٸ� �ִ� ���̹Ƿ� ��������ٰ� �˷��ְ� return
	if (FindTexture(Name))
		return true;


	//��������� �� �ؽ�ó�� ���� �Ҵ��Ѵ�.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//�ؽ�ó �ε带 �õ��ϰ�, �����ϸ� ���� �����ϰ� false ����
	if (!NewTex->LoadTextureFullPath(FullPath, MaxChannel))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//���������� map�� ����
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}


bool CTextureManager::LoadTextureByTextureNumbers(const std::string& Name,
	const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName)
{
	//���� �ش� �̸����� ������ �ؽ�ó�� �ִٸ� �״�� return �Ѵ�.
	if (FindTexture(Name))
		return false;


	//��������� �� �ؽ�ó�� ���� �Ҵ��Ѵ�.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//�ؽ�ó �ε带 �õ��ϰ�, �����ϸ� ���� �����ϰ� false ����
	if (!NewTex->LoadTextureByTextureNumbers(FolderPath, NumberOfTexture, PathName))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//���� �̸��� ���Ϳ� �־ �ε��� ��� ������ Ÿ���ϼ��ۿ� ����.
	NewTex->m_TextureType = ETextureType::Frame;

	//���������� map�� ����
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;

}



bool CTextureManager::LoadTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	//���� �ش� �̸����� ������ �ؽ�ó�� �ִٸ� �״�� return �Ѵ�.
	if (FindTexture(Name))
		return false;


	//��������� �� �ؽ�ó�� ���� �Ҵ��Ѵ�.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//�ؽ�ó �ε带 �õ��ϰ�, �����ϸ� ���� �����ϰ� false ����
	if (!NewTex->LoadTexture(vecFileName, PathName))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//���� �̸��� ���Ϳ� �־ �ε��� ��� ������ Ÿ���ϼ��ۿ� ����.
	NewTex->m_TextureType = ETextureType::Frame;

	//���������� map�� ����
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath)
{
	//���� �ش� �̸����� ������ �ؽ�ó�� �ִٸ� �״�� return �Ѵ�.
	if (FindTexture(Name))
		return false;


	//��������� �� �ؽ�ó�� ���� �Ҵ��Ѵ�.
	CTexture* NewTex = new CTexture;

	NewTex->SetName(Name);

	//�ؽ�ó �ε带 �õ��ϰ�, �����ϸ� ���� �����ϰ� false ����
	if (!NewTex->LoadTextureFullPath(vecFullPath))
	{
		SAFE_DELETE(NewTex);
		return false;
	}

	//���� �̸��� ���Ϳ� �־ �ε��� ��� ������ Ÿ���ϼ��ۿ� ����.
	NewTex->m_TextureType = ETextureType::Frame;

	//���������� map�� ����
	m_mapTexture.insert(std::make_pair(Name, NewTex));

	return true;
}

CTexture* CTextureManager::LoadTexture(FILE* File)
{
	// ���� �̸����� ����Ȱ� �ִٸ� �߸��Ȱ��̴�.
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






