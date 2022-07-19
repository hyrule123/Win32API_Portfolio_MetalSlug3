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
			//주소를 미리 받아놓고
			std::string Name = iter->first;

			//삭제(삭제하면 다음 iterator 주소가 반환되므로 ++iter 해줄 필요 없다)
			//먼저 삭제해서 RefCount를 줄여놓는다.
			iter = m_mapAnimationSequence.erase(iter);
			iterEnd = m_mapAnimationSequence.end();

			//그리고 ReleaseTexture을 통해 남아있는 RefCount를 확인하고, 
			//1만 남아있으면(-> 새로 로드된 씬 또는 기존 씬 모두 사용하지 않는 텍스처)
			//제거를 하도록 해준다.
			CResourceManager::GetInst()->ReleaseTexture(Name);

		}

	}

	{
		auto iter = m_mapTexture.begin();
		auto iterEnd = m_mapTexture.end();

		while (iter != iterEnd)
		{
			//주소를 미리 받아놓고
			std::string Name = iter->first;

			//삭제(삭제하면 다음 iterator 주소가 반환되므로 ++iter 해줄 필요 없다)
			//먼저 삭제해서 RefCount를 줄여놓는다.
			iter = m_mapTexture.erase(iter);
			iterEnd = m_mapTexture.end();

			//그리고 ReleaseTexture을 통해 남아있는 RefCount를 확인하고, 
			//1만 남아있으면(-> 새로 로드된 씬 또는 기존 씬 모두 사용하지 않는 텍스처)
			//제거를 하도록 해준다.
			CResourceManager::GetInst()->ReleaseTexture(Name);

		}

	}

	{
		auto iter = m_mapSound.begin();
		auto iterEnd = m_mapSound.end();

		while (iter != iterEnd)
		{
			//주소를 미리 받아놓고
			std::string Name = iter->first;

			//삭제(삭제하면 다음 iterator 주소가 반환되므로 ++iter 해줄 필요 없다)
			//먼저 삭제해서 RefCount를 줄여놓는다.
			iter = m_mapSound.erase(iter);
			iterEnd = m_mapSound.end();

			//그리고 ReleaseTexture을 통해 남아있는 RefCount를 확인하고, 
			//1만 남아있으면(-> 새로 로드된 씬 또는 기존 씬 모두 사용하지 않는 텍스처)
			//제거를 하도록 해준다.
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

	//혹시 여기에 없으면
	if (iter == m_mapSound.end())
	{
		//ResourceManager에서도 한번 찾아본다.
		CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

		//여기서도 없으면 nullptr을 반환하고
		if (!Sound)
			return nullptr;

		//혹시나 있으면 이것도 SceneResource에 등록하고 주소를 반환한다.
		m_mapSound.insert(std::make_pair(Name, Sound));

		return Sound;
	}

	return iter->second;
}

bool CSceneResource::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	//사운드를 찾아서 있으면 true 반환
	CSound* Sound = FindSound(Name);
	if (Sound)
		return true;

	//리소스매니저에서도 찾아봄
	Sound = CResourceManager::GetInst()->FindSound(Name);

	if (Sound)
		return true;

	//못찾았으면 만들기. 만들기 실패했으면 삭제하고 return false
	if (!CResourceManager::GetInst()->LoadSound(GroupName, Name, Loop, FileName, PathName))
		return false;
	
	//여기까지 왔으면 안 만들어졌을리가 없음. map에 등록 후 true 반환
	//FindSound에서 자동으로 등록됨.
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

	//혹시 여기에 없으면
	if (iter == m_mapAnimationSequence.end())
	{
		//ResourceManager에서도 한번 찾아본다.
		CAnimationSequence* AnimationSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

		//여기서도 없으면 nullptr을 반환하고
		if (!AnimationSequence)
			return nullptr;

		//혹시나 있으면 이것도 SceneResource에 등록하고 주소를 반환한다.
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
	//CResourceManager에 접근해서 바로 생성
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
	//이미 만들어져 있다면 return
	if (FindAnimationSequence(Name))
		return true;

	//생성에 실패했다면 return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, Texture))
		return false;

	//성공했다면 주소를 받아온다.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//해당 주소를 이 클래스의 map에 추가한다.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& Name, const std::string& TexName)
{
	//이미 만들어져 있다면 return
	if (FindAnimationSequence(Name))
		return true;

	//텍스처 주소 받아오고 없으면 return
	CTexture* Tex = CResourceManager::GetInst()->FindTexture(TexName);
	if (!Tex)
		return false;

	//생성에 실패했다면 return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, Tex))
		return false;

	//성공했다면 주소를 받아온다.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//해당 주소를 이 클래스의 map에 추가한다.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& Name, const std::string& TexName, const std::tstring& FileName, const std::string& PathName)
{
	//이미 만들어져 있다면 return
	if (FindAnimationSequence(Name))
		return true;

	//생성에 실패했다면 return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, TexName, FileName, PathName))
		return false;

	//성공했다면 주소를 받아온다.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//해당 주소를 이 클래스의 map에 추가한다.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName, const std::tstring& FullPath)
{
	//이미 만들어져 있다면 return
	if (FindAnimationSequence(Name))
		return true;

	//생성에 실패했다면 return
	if (!CResourceManager::GetInst()->CreateAnimationSequenceFullPath(Name, TexName, FullPath))
		return false;

	//성공했다면 주소를 받아온다.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//해당 주소를 이 클래스의 map에 추가한다.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}



bool CSceneResource::CreateAnimationSequence(const std::string& Name, const std::string& TexName, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	//이미 만들어져 있다면 return
	if (FindAnimationSequence(Name))
		return true;

	//생성에 실패했다면 return
	if (!CResourceManager::GetInst()->CreateAnimationSequence(Name, TexName, vecFileName, PathName))
		return false;

	//성공했다면 주소를 받아온다.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//해당 주소를 이 클래스의 map에 추가한다.
	m_mapAnimationSequence.insert(std::make_pair(Name, Seq));


	return true;
}

bool CSceneResource::CreateAnimationSequenceFullPath(const std::string& Name, const std::string& TexName, const std::vector<std::tstring>& vecFullPath)
{
	//이미 만들어져 있다면 return
	if (FindAnimationSequence(Name))
		return true;

	//생성에 실패했다면 return
	if (!CResourceManager::GetInst()->CreateAnimationSequenceFullPath(Name, TexName, vecFullPath))
		return false;

	//성공했다면 주소를 받아온다.
	CAnimationSequence* Seq = CResourceManager::GetInst()->FindAnimationSequence(Name);

	//해당 주소를 이 클래스의 map에 추가한다.
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
	EReactionChannel MaxReactionChannel = (EReactionChannel)EReactionChannelFlag;


	if (LoadTexture(TexName, FileName, TEXTURE_PATH, MaxReactionChannel))
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
		SetColorKey(TexName, R, G, B);
	}
	else
	{
		SAFE_DELETE(CSVEditor);
		return false;
	}

	//네 번째 행 첫번쨰 열부터 만들어야 할 애니메이션 시퀀스 이름이 적혀있다.
	int rowcount = (int)CSVEditor->GetRowCount();
	for (int i = (int)ERowInfo::AnimSeqInfoStart; i < rowcount; ++i)
	{
		if (CSVEditor->GetRow(i).empty())
			continue;

		//가장 첫번째 열은 만들 시퀀스의 이름이 적혀 있다.
		std::string AnimName = CSVEditor->GetCell(i, (int)EColInfo::AnimSeqName);

		//애니메이션 시퀀스 생성에 도로 실패하면 삭제하고 return
		if (!CreateAnimationSequence(AnimName, TexName))
		{
			SAFE_DELETE(CSVEditor);
			return false;
		}


		int rowsize = 0;

		//이미 로드되었음이 확인되면 시퀀스 부분은 로드하지 않음.
		if (CheckAnimLoaded(AnimName))
			continue;
		else
			rowsize = (int)CSVEditor->GetRow(i).size();

		//먼저 애니메이션 Sequence에 대한 정보를 등록한다.
		for (
			int j = (int)EColInfo::FramePosStart;
			j < rowsize;
			j++
			)
		{
			std::string src = CSVEditor->GetCell(i, j);

			//만약 아무 내용도 입력되어있지 않다면 continue
			//AnimInfo 인자값에도 공란이 들어올 수 있으므로 해당 사항도 체크
			if (src == "")
				continue;


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

			//아무런 정보가 입력되어있지 않을 경우 그림파일의 첫 10프레임 정보를 불러온다.
			if (Start == 0.f && End == 0.f)
			{
				End = 10.f;
			}

			//프레임 데이터 추가
			if (!AddAnimationSpriteFrame(AnimName,
				Start, End, Offset))
			{
				SAFE_DELETE(CSVEditor);
				return false;
			}

			//애니메이션 시퀀스에 로드되었음을 전달
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

	//혹시 여기에 없으면
	if (iter == m_mapTexture.end())
	{
		//ResourceManager에서도 한번 찾아본다.
		CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

		//여기서도 없으면 nullptr을 반환하고
		if (!Texture)
			return nullptr;

		//혹시나 있으면 이것도 SceneResource에 등록하고 주소를 반환한다.
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
	
	//이미 만들어져 있으면 true 반환.
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

