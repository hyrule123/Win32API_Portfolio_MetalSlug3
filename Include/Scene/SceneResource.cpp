#include "SceneResource.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Resource/Sound/Sound.h"

#include "../Resource/Font/Font.h"

#include "../CSVEditor.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto iter = m_mapAnimationSequence.begin();
		auto iterEnd = m_mapAnimationSequence.end();

		while (iter != iterEnd)
		{
			//�ּҸ� �̸� �޾Ƴ���
			std::string Name = iter->first;

			//����(�����ϸ� ���� iterator �ּҰ� ��ȯ�ǹǷ� ++iter ���� �ʿ� ����)
			//���� �����ؼ� RefCount�� �ٿ����´�.
			iter = m_mapAnimationSequence.erase(iter);
			iterEnd = m_mapAnimationSequence.end();

			//�׸��� ReleaseTexture�� ���� �����ִ� RefCount�� Ȯ���ϰ�, 
			//1�� ����������(-> ���� �ε�� �� �Ǵ� ���� �� ��� ������� �ʴ� �ؽ�ó)
			//���Ÿ� �ϵ��� ���ش�.
			CResourceManager::GetInst()->ReleaseTexture(Name);

		}

	}

	{
		auto iter = m_mapTexture.begin();
		auto iterEnd = m_mapTexture.end();

		while (iter != iterEnd)
		{
			//�ּҸ� �̸� �޾Ƴ���
			std::string Name = iter->first;

			//����(�����ϸ� ���� iterator �ּҰ� ��ȯ�ǹǷ� ++iter ���� �ʿ� ����)
			//���� �����ؼ� RefCount�� �ٿ����´�.
			iter = m_mapTexture.erase(iter);
			iterEnd = m_mapTexture.end();

			//�׸��� ReleaseTexture�� ���� �����ִ� RefCount�� Ȯ���ϰ�, 
			//1�� ����������(-> ���� �ε�� �� �Ǵ� ���� �� ��� ������� �ʴ� �ؽ�ó)
			//���Ÿ� �ϵ��� ���ش�.
			CResourceManager::GetInst()->ReleaseTexture(Name);

		}

	}

	{
		auto iter = m_mapSound.begin();
		auto iterEnd = m_mapSound.end();

		while (iter != iterEnd)
		{
			//�ּҸ� �̸� �޾Ƴ���
			std::string Name = iter->first;

			//����(�����ϸ� ���� iterator �ּҰ� ��ȯ�ǹǷ� ++iter ���� �ʿ� ����)
			//���� �����ؼ� RefCount�� �ٿ����´�.
			iter = m_mapSound.erase(iter);
			iterEnd = m_mapSound.end();

			//�׸��� ReleaseTexture�� ���� �����ִ� RefCount�� Ȯ���ϰ�, 
			//1�� ����������(-> ���� �ε�� �� �Ǵ� ���� �� ��� ������� �ʴ� �ؽ�ó)
			//���Ÿ� �ϵ��� ���ش�.
			CResourceManager::GetInst()->ReleaseSound(Name);

		}

	}

}


bool CSceneResource::LoadFontInfo(const std::string& Name, const std::tstring& FontName,
	int Width, int Height)
{
	if (FindFont(Name))
		return false;

	if (!CResourceManager::GetInst()->LoadFontInfo(Name, FontName, Width, Height))
		return false;

	CFont* Font = CResourceManager::GetInst()->FindFont(Name);

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

bool CSceneResource::LoadFontResource(const std::tstring& FontFileName, const std::string& PathName)
{
	return CResourceManager::GetInst()->LoadFontResource(FontFileName, PathName);
}

void CSceneResource::SetFont(const std::string& Name, HDC hDC)
{
	return CResourceManager::GetInst()->SetFont(Name, hDC);
}

void CSceneResource::ResetFont(const std::string& Name, HDC hDC)
{
	return CResourceManager::GetInst()->ResetFont(Name, hDC);
}

CFont* CSceneResource::FindFont(const std::string& Name)
{
	auto	iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
	{
		CFont* Font = CResourceManager::GetInst()->FindFont(Name);

		if (!Font)
			return nullptr;

		m_mapFont.insert(std::make_pair(Name, Font));

		return Font;
	}

	return iter->second;
}

CSound* CSceneResource::FindSound(const std::string& Name)
{

	auto iter = m_mapSound.find(Name);

	//Ȥ�� ���⿡ ������
	if (iter == m_mapSound.end())
	{
		//ResourceManager������ �ѹ� ã�ƺ���.
		CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

		//���⼭�� ������ nullptr�� ��ȯ�ϰ�
		if (!Sound)
			return nullptr;

		//Ȥ�ó� ������ �̰͵� SceneResource�� ����ϰ� �ּҸ� ��ȯ�Ѵ�.
		m_mapSound.insert(std::make_pair(Name, Sound));

		return Sound;
	}

	return iter->second;
}

bool CSceneResource::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	//���带 ã�Ƽ� ������ true ��ȯ
	CSound* Sound = FindSound(Name);
	if (Sound)
		return true;

	//���ҽ��Ŵ��������� ã�ƺ�
	Sound = CResourceManager::GetInst()->FindSound(Name);

	if (Sound)
		return true;

	//��ã������ �����. ����� ���������� �����ϰ� return false
	if (!CResourceManager::GetInst()->LoadSound(GroupName, Name, Loop, FileName, PathName))
		return false;
	
	//������� ������ �� ������������� ����. map�� ��� �� true ��ȯ
	//FindSound���� �ڵ����� ��ϵ�.
	Sound = FindSound(Name);


	return true;
}




bool CSceneResource::CreateSoundChannelGroup(const std::string& Name)
{
	return CResourceManager::GetInst()->CreateSoundChannelGroup(Name);
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& Name)
{
	return CResourceManager::GetInst()->FindChannelGroup(Name);
}





bool CSceneResource::SetMasterVolume(int Volume)
{
	return CResourceManager::GetInst()->SetMasterVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
	return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundResume(Name);
}

bool CSceneResource::GetPlaying(const std::string& Name)
{
	return CResourceManager::GetInst()->GetPlaying(Name);
}

bool CSceneResource::SoundFadeIn(const std::string& Name, float DeltaTime)
{
	return CResourceManager::GetInst()->SoundFadeIn(Name, DeltaTime);
}

bool CSceneResource::SoundFadeOut(const std::string& Name, float DeltaTime)
{
	return CResourceManager::GetInst()->SoundFadeOut(Name, DeltaTime);
}

bool CSceneResource::SoundPlayOneChan(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlayOneChan(Name);
}

bool CSceneResource::SoundSetNoDelay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundSetNoDelay(Name);
}



//////////////////////////////////////////////////////////////////////






/////////////////////////// ANIMATION /////////////////////////////////////


CAnimationSequence* CSceneResource::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	//Ȥ�� ���⿡ ������
	if (iter == m_mapAnimationSequence.end())
	{
		//ResourceManager������ �ѹ� ã�ƺ���.
		CAnimationSequence* AnimationSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

		//���⼭�� ������ nullptr�� ��ȯ�ϰ�
		if (!AnimationSequence)
			return nullptr;

		//Ȥ�ó� ������ �̰͵� SceneResource�� ����ϰ� �ּҸ� ��ȯ�Ѵ�.
		m_mapAnimationSequence.insert(std::make_pair(Name, AnimationSequence));

		return AnimationSequence;
	}

	return iter->second;

}

void CSceneResource::SetAnimLoaded(const std::string& Name, bool isLoaded)
{
	CResourceManager::GetInst()->SetAnimLoaded(Name, isLoaded);
}

bool CSceneResource::CheckAnimLoaded(const std::string& Name)
{
	return CResourceManager::GetInst()->CheckAnimLoaded(Name);
}

bool CSceneResource::AddAnimationSpriteFrame(
	const std::string& Name,
	const Vector2& Start, const Vector2& End, 
	const Vector2& Offset
	)
{
	//CResourceManager�� �����ؼ� �ٷ� ����
	return CResourceManager::GetInst()->AddAnimationSpriteFrame(Name, Start, End, Offset);
}

bool CSceneResource::AddAnimationSpriteFrame(
	const std::string& Name, 
	float PosX, float PosY, 
	float SizeX, float SizeY, 
	float OffsetX, float OffsetY)
{
	return CResourceManager::GetInst()->AddAnimationSpriteFrame(Name, PosX, PosY, SizeX, SizeY, OffsetX, OffsetY);
}

bool CSceneResource::AddAnimationFrameFrame(const std::string& Name)
{
	return  CResourceManager::GetInst()->AddAnimationFrameFrame(Name);
}


bool CSceneResource::CreateAnimationSequence(const std::string& Name, CTexture* Texture)
{
	//�̹� ������� �ִٸ� return
	if (FindAnimationSequence(Name))
		return true;

	//������ �����ߴٸ� return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, Texture))
		return false;

	//�����ߴٸ� �ּҸ� �޾ƿ´�.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//�ش� �ּҸ� �� Ŭ������ map�� �߰��Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& Name, const std::string& TexName)
{
	//�̹� ������� �ִٸ� return
	if (FindAnimationSequence(Name))
		return true;

	//�ؽ�ó �ּ� �޾ƿ��� ������ return
	CTexture* Tex = CResourceManager::GetInst()->FindTexture(TexName);
	if (!Tex)
		return false;

	//������ �����ߴٸ� return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, Tex))
		return false;

	//�����ߴٸ� �ּҸ� �޾ƿ´�.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//�ش� �ּҸ� �� Ŭ������ map�� �߰��Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& Name, const std::string& TexName, const std::tstring& FileName, const std::string& PathName)
{
	//�̹� ������� �ִٸ� return
	if (FindAnimationSequence(Name))
		return true;

	//������ �����ߴٸ� return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, TexName, FileName, PathName))
		return false;

	//�����ߴٸ� �ּҸ� �޾ƿ´�.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//�ش� �ּҸ� �� Ŭ������ map�� �߰��Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName, const std::tstring& FullPath)
{
	//�̹� ������� �ִٸ� return
	if (FindAnimationSequence(Name))
		return true;

	//������ �����ߴٸ� return
	if (!CResourceManager::GetInst()->CreateAnimationSequenceFullPath(Name, TexName, FullPath))
		return false;

	//�����ߴٸ� �ּҸ� �޾ƿ´�.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//�ش� �ּҸ� �� Ŭ������ map�� �߰��Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}



bool CSceneResource::CreateAnimationSequence(const std::string& Name, const std::string& TexName, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	//�̹� ������� �ִٸ� return
	if (FindAnimationSequence(Name))
		return true;

	//������ �����ߴٸ� return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, TexName, vecFileName, PathName))
		return false;

	//�����ߴٸ� �ּҸ� �޾ƿ´�.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//�ش� �ּҸ� �� Ŭ������ map�� �߰��Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName, const std::vector<std::tstring>& vecFullPath)
{
	//�̹� ������� �ִٸ� return
	if (FindAnimationSequence(Name))
		return true;

	//������ �����ߴٸ� return
	if (!CResourceManager::GetInst()->CreateAnimationSequenceFullPath(Name, TexName, vecFullPath))
		return false;

	//�����ߴٸ� �ּҸ� �޾ƿ´�.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//�ش� �ּҸ� �� Ŭ������ map�� �߰��Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

void CSceneResource::ClearAnimSeq()
{
	m_mapAnimationSequence.clear();
	CResourceManager::GetInst()->ClearAnimSeq();
}




bool CSceneResource::LoadCSVPreset(
	const std::tstring& CSVFileName,
	const std::string& PathName
)
{
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
	//csv�� �ι�°�� ù��° ������ �ؽ�ó �̸��� �����ִ�.
	std::string TexName = CSVEditor->GetCell(TexInfoRow, 0);

	//ù�� �ι�°�� �ι�° ������ �ִϸ��̼� ���ϸ��� �����ִ�.
	std::tstring FileName;
#ifdef _UNICODE
	FileName.assign(
		CSVEditor->GetCell(TexInfoRow, 1).begin(),
		CSVEditor->GetCell(TexInfoRow, 1).end()
	);
#else
	FileName = m_CSVEditor->GetCell(TexInfoRow, 1);
#endif

	//ù�� �ι�°�� 5��° ��(1, 4)���� ��Ʈ����Ʈ ä���� ����Ұ����� ���ΰ� �����ִ�.
	std::string EReactionChannelstr = CSVEditor->GetCell(TexInfoRow, 4);
	int EReactionChannelFlag = 0;
	sscanf_s(EReactionChannelstr.c_str(), "%d", &EReactionChannelFlag);

	//�ؽ�ó�� �ִ�� ������ EReactionChannel���� �ε��Ѵ�.
	EReactionChannel MaxReactionChannel = (EReactionChannel)EReactionChannelFlag;


	if (LoadTexture(TexName, FileName, TEXTURE_PATH, MaxReactionChannel))
	{
		//�÷�Ű�� ����ִ� ��
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
		//�÷�Ű RGB���� ���������� �ԷµǾ��� ���
		if (SlashCount == 2)
		{
			sscanf_s(src.c_str(), "%d/%d/%d", &R, &G, &B);
		}
		SetColorKey(TexName, R, G, B);
	}
	else
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}

	//�� ��° �� ù���� ������ ������ �� �ִϸ��̼� ������ �̸��� �����ִ�.
	int rowcount = (int)CSVEditor->GetRowCount();
	for (int i = (int)ERowInfo::AnimSeqInfoStart; i < rowcount; ++i)
	{
		if (CSVEditor->GetRow(i).empty())
			continue;

		//���� ù��° ���� ���� �������� �̸��� ���� �ִ�.
		std::string AnimName = CSVEditor->GetCell(i, (int)EColInfo::AnimSeqName);

		//�ִϸ��̼� ������ ������ ���� �����ϸ� �����ϰ� return
		if (!CreateAnimationSequence(AnimName, TexName))
		{
			SAFE_DELETE(CSVEditor);
			return false;
		}


		int rowsize = 0;

		//�̹� �ε�Ǿ����� Ȯ�εǸ� ������ �κ��� �ε����� ����.
		if (CheckAnimLoaded(AnimName))
			continue;
		else
			rowsize = (int)CSVEditor->GetRow(i).size();

		//���� �ִϸ��̼� Sequence�� ���� ������ ����Ѵ�.
		for (
			int j = (int)EColInfo::FramePosStart;
			j < rowsize;
			j++
			)
		{
			std::string src = CSVEditor->GetCell(i, j);

			//���� �ƹ� ���뵵 �ԷµǾ����� �ʴٸ� continue
			//AnimInfo ���ڰ����� ������ ���� �� �����Ƿ� �ش� ���׵� üũ
			if (src == "")
				continue;


			Vector2 Start;
			Vector2 End;
			Vector2 Offset;

			//�������� 4�� �̻��̸� Offset�� ��ϵǾ��ִٴ� ���̹Ƿ� Offset���� �����Ͽ� �����Ѵ�.
			int SlashCount = 0;
			for (size_t k = 0; k < src.size(); ++k)
			{
				if (src[k] == '/')
					++SlashCount;
			}

			//�Է� �ȵ� �κ��� ������ return;
			if (SlashCount < 3)
				continue;
			//��������Ʈ ��ǥ���� �ԷµǾ������
			else if (SlashCount == 3)
			{
				sscanf_s(src.c_str(), "%f/%f/%f/%f", &Start.x, &Start.y, &End.x, &End.y);
			}
			//������ ������ �Էµ������
			else if (SlashCount > 3)
			{
				sscanf_s(src.c_str(), "%f/%f/%f/%f/%f/%f",
					&Start.x, &Start.y, &End.x, &End.y, &Offset.x, &Offset.y);
			}

			//�ƹ��� ������ �ԷµǾ����� ���� ��� �׸������� ù 10������ ������ �ҷ��´�.
			if (Start == 0.f && End == 0.f)
			{
				End = 10.f;
			}

			//������ ������ �߰�
			if (!AddAnimationSpriteFrame(AnimName,
				Start, End, Offset))
			{
				SAFE_DELETE(CSVEditor);
				return false;
			}

			//�ִϸ��̼� �������� �ε�Ǿ����� ����
			SetAnimLoaded(AnimName);
		}
	}

	SAFE_DELETE(CSVEditor);
	return true;
}







///////////////////////////// TEXTURE //////////////////////////////////
CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	//Ȥ�� ���⿡ ������
	if (iter == m_mapTexture.end())
	{
		//ResourceManager������ �ѹ� ã�ƺ���.
		CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

		//���⼭�� ������ nullptr�� ��ȯ�ϰ�
		if (!Texture)
			return nullptr;

		//Ȥ�ó� ������ �̰͵� SceneResource�� ����ϰ� �ּҸ� ��ȯ�Ѵ�.
		m_mapTexture.insert(std::make_pair(Name, Texture));

		return Texture;
	}

	return iter->second;

}

bool CSceneResource::SetColorKey(const std::string& Name,
	unsigned char r, unsigned char g, unsigned char b,
	int index)
{
	return CResourceManager::GetInst()->SetColorKey(Name, r, g, b, index);
}

bool CSceneResource::LoadTexture(const std::string& Name, const std::tstring& FileName,  const std::string& PathName, EReactionChannel MaxChannel)
{
	CTexture* Tex = FindTexture(Name);
	
	//�̹� ������� ������ true ��ȯ.
	if (Tex)
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName, MaxChannel))
		return false;
	
	Tex = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Tex));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const std::tstring& FullPath, EReactionChannel MaxChannel)
{
	CTexture* Tex = FindTexture(Name);

	if (Tex)
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath, MaxChannel))
		return false;

	Tex = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Tex));

	return true;
}


bool CSceneResource::LoadTextureByTextureNumbers(const std::string& Name, const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName)
{
	CTexture* Tex = FindTexture(Name);

	if (Tex)
		return true;


	if (!CResourceManager::GetInst()->LoadTextureByTextureNumbers(Name, FolderPath,  NumberOfTexture, PathName))
		return false;

	Tex = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Tex));


	return true;
}



bool CSceneResource::LoadTexture(const std::string& Name, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	CTexture* Tex = FindTexture(Name);

	if (Tex)
		return true;


	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	Tex = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Tex));


	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const std::vector<std::tstring>& vecFullPath)
{
	CTexture* Tex = FindTexture(Name);

	if (Tex)
		return true;


	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	Tex = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Tex));


	return true;
}

CTexture* CSceneResource::LoadTexture(FILE* File)
{
	CTexture* Texture = CResourceManager::GetInst()->LoadTexture(File);

	if (!Texture)
		return nullptr;

	if (!FindTexture(Texture->GetName()))
		m_mapTexture.insert(std::make_pair(Texture->GetName(), Texture));

	return Texture;
}


////////////////////////////////////////////////////////////////////////////

