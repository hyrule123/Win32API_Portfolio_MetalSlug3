#pragma once


#include "../../GameInfo.h"


class CTextureManager
{
	friend class CResourceManager;
private:
	CTextureManager();
	~CTextureManager();

	bool Init();

	std::unordered_map<std::string, CSharedPtr<class CTexture>>	m_mapTexture;

	bool SetColorKey(const std::string& Name, 
		unsigned char r = 255,
		unsigned char g = 0,
		unsigned char b = 255,
		int index = -1);


	class CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);


	bool LoadTexture(const std::string& Name, const std::tstring& FileName, const std::string& PathName = TEXTURE_PATH, EReactionChannel MaxChannel = EReactionChannel::Normal);
	bool LoadTextureFullPath(const std::string& Name, const std::tstring& FullPath, EReactionChannel MaxChannel = EReactionChannel::Normal);


	//Frame ���� ��� �޼ҵ�
	bool LoadTextureByTextureNumbers(const std::string& Name,
		const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName = TEXTURE_PATH);

	bool LoadTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath);


	//���̺�, �ε� ����
	CTexture* LoadTexture(FILE* File);

};

