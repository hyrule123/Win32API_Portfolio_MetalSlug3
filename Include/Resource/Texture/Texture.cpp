#include "Texture.h"
#include "../../GameManager.h"

CTexture::CTexture():
	m_TextureType(ETextureType::Sprite)
{
	SetTypeID<CTexture>();
}

CTexture::~CTexture()
{
	size_t size = m_vecImageInfo.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecImageInfo[i]);
	}

}


void CTexture::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int index)
{
	if (index != -1)
	{
		m_vecImageInfo[index]->ColorKey = RGB(r, g, b);
		m_vecImageInfo[index]->ColorKeyEnable = true;
	}
		
	else
	{
		size_t size = 0; 

		if (m_TextureType == ETextureType::Sprite)
			size = 1;
		else
			size = m_vecImageInfo.size();
		
		for (size_t i = 0; i < size; ++i)
		{
			m_vecImageInfo[i]->ColorKey = RGB(r, g, b);
			m_vecImageInfo[i]->ColorKeyEnable = true;
		}
	}

	
}

COLORREF CTexture::GetColorKey(int index) const
{
	return m_vecImageInfo[index]->ColorKey;
}

bool CTexture::GetColorKeyEnable(int index) const
{
	return m_vecImageInfo[index]->ColorKeyEnable;
}

int CTexture::GetMaxReactionChannel() const
{
	if (m_TextureType == ETextureType::Frame)
		return 0;
	else
		return (int)m_vecImageInfo.size();
}

//개별 파일 등록 메소드
bool CTexture::LoadTexture(const std::tstring& FileName, const std::string& PathName, EReactionChannel MaxChannel)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (!Path)
		return false;

	TCHAR FullPath[MAX_PATH] = {};

	//경로를 일단 넣어놓고, 
	lstrcpy(FullPath, Path->Path);
	//파일명을 붙여준다.
	lstrcat(FullPath, FileName.c_str());

	bool result = LoadTextureFullPath(FullPath, MaxChannel);

	if (result)
	{
		lstrcpy(m_vecImageInfo.back()->FileName, FileName.c_str());
		m_vecImageInfo.back()->PathName = PathName;
	}

	return result;
}

bool CTexture::LoadTextureFullPath(const std::tstring& FullPath, EReactionChannel MaxChannel)
{
	ImageInfo* Image = new ImageInfo;

	//메모리 DC를 얻어온다. =
	Image->hMemDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	//이미지를 로드한다. 
	Image->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(), FullPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//혹시나 이미지가 로드되지 않았다면 false를 리턴한다.
	if (!Image->hBmp)
		return false;

	//그리기 도구를 갈아끼우고, 이전 도구를 따로 변수를 만들어 들고 있는다.
	Image->hPrevBmp = (HBITMAP)SelectObject(Image->hMemDC, Image->hBmp);

	//비트맵 이미지의 정보를 저장한다.
	GetObject(Image->hBmp, sizeof(BITMAP), &Image->BmpInfo);

	lstrcpy(Image->FullPath, FullPath.c_str());

	m_vecImageInfo.push_back(Image);


	//여기까진 기본 텍스처 로드 과정 진행
	//추가로 히트이펙트와, 필요시 무적 이펙트를 넣는 과정을 진행

	if (MaxChannel != EReactionChannel::Normal)
	{
		Gdiplus::HueSaturationLightness HSB;
		Gdiplus::HueSaturationLightnessParams Param;

		int channel = (int)EReactionChannel::MAX;
		for (int i = (int)EReactionChannel::Normal + 1; i < channel; ++i)
		{
			switch ((EReactionChannel)i)
			{
			case EReactionChannel::HitReaction:
				Param.hueLevel = -50;
				Param.lightnessLevel = 0;
				Param.saturationLevel = 0;
				break;
			case EReactionChannel::InvincibleReaction:
				Param.hueLevel = 20;
				Param.lightnessLevel = 20;
				Param.saturationLevel = 0;
				break;
			case EReactionChannel::Laser1:
				Param.hueLevel = 100;
				Param.lightnessLevel = 0;
				Param.saturationLevel = 0;
				break;
			case EReactionChannel::Laser2:
				Param.hueLevel = -130;
				Param.lightnessLevel = 0;
				Param.saturationLevel = 0;
				break;
			case EReactionChannel::Laser3:
				Param.hueLevel = 60;
				Param.lightnessLevel = 0;
				Param.saturationLevel = 0;
				break;


			default:
				break;
			}

			//HSB값을 저장해놓고
			HSB.SetParameters(&Param);

			//원본으로부터 이미지 파일을 GDI+로 복사
			Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromHBITMAP(Image->hBmp, NULL);

			//전체 범위 변환을 위해 사이즈를 받아놓음
			RECT rc;
			rc.left = 0;
			rc.top = 0;
			rc.right = bmp->GetWidth();
			rc.bottom = bmp->GetHeight();

			//효과 적용
			Gdiplus::Status status = bmp->ApplyEffect(&HSB, &rc);

			//컬러키를 확인할 0, 0 픽셀의 RGB값 받아옴
			Gdiplus::Color color;
			bmp->GetPixel(0, 0, &color);
			Gdiplus::ARGB rgb = color.GetValue();
			int r = ((rgb >> RED_SHIFT) & 0x000000FF);
			int g = ((rgb >> GREEN_SHIFT) & 0x000000FF);
			int b = ((rgb >> BLUE_SHIFT) & 0x000000FF);

			//동적할당
			ImageInfo* EffectImage = new ImageInfo;

			//메모리 DC를 얻어온다. =
			EffectImage->hMemDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

			// GDI 형식 HBITMAP을 받아온다
			bmp->GetHBITMAP(Gdiplus::Color(0, 0, 0), &EffectImage->hBmp);

			//혹시나 이미지가 로드되지 않았다면 false를 리턴한다.
			if (!EffectImage->hBmp)
				return false;

			//방금 받아놓은 컬러키 저장
			EffectImage->ColorKey = RGB(r, g, b);
			EffectImage->ColorKeyEnable = true;

			//그리기 도구를 갈아끼우고, 이전 도구를 따로 변수를 만들어 들고 있는다.
			EffectImage->hPrevBmp = (HBITMAP)SelectObject(EffectImage->hMemDC, EffectImage->hBmp);

			//비트맵 이미지의 정보를 저장한다.
			GetObject(EffectImage->hBmp, sizeof(BITMAP), &EffectImage->BmpInfo);
			lstrcpy(EffectImage->FullPath, FullPath.c_str());

			m_vecImageInfo.push_back(EffectImage);
		}
	}

	return true;
}



bool CTexture::LoadTextureByTextureNumbers(const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName)
{
	//경로를 찾아놓고
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	//없으면 리턴
	if (!Path)
		return false;

	//만들어진 풀 경로 수만큼 미리 공간을 확보해놓은 뒤
	std::vector<std::tstring> vecFullPath;
	vecFullPath.resize(NumberOfTexture);

	std::vector<std::tstring> vecFileName;
	vecFileName.resize(NumberOfTexture);

	//풀경로 입력 과정
	for (size_t i = 0; i < NumberOfTexture; ++i)
	{
		vecFullPath[i] = Path->Path;

		//폴더 경로 추가
		vecFullPath[i] += FolderPath;

		//개별 파일명 생성
		TCHAR TempFileName[MAX_PATH] = {};
		tsPrintf_s(TempFileName, _countof(TempFileName), TEXT("/%d.bmp"), (int)i);

		//파일명 추가
		vecFullPath[i] += TempFileName;

		//파일명 받아놓음(뒤에서 사용)
		vecFileName[i] = TempFileName;
		

	}

	//로드하고 성공하면 파일 이름, 주소, 이름을 등록.
	bool result = LoadTextureFullPath(vecFullPath);
	if (result)
	{
		for (size_t i = 0; i < NumberOfTexture; ++i)
		{
			lstrcpy(m_vecImageInfo[i]->FileName, vecFileName[i].c_str());
			m_vecImageInfo[i]->PathName = PathName;
		}
	}

	return result;
}



bool CTexture::LoadTexture(const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	//경로를 찾아놓고
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	//없으면 리턴
	if (!Path)
		return false;

	//파일 이름 리스트의 사이즈가 몇개인지 받아놓고
	size_t size = vecFileName.size();

	//만들어진 풀 경로 수만큼 미리 공간을 확보해놓은 뒤
	std::vector<std::tstring> vecFullPath;
	vecFullPath.resize(size);

	//풀경로 입력
	for (size_t i = 0; i < size; ++i)
	{
		vecFullPath[i] = Path->Path;
		vecFullPath[i] += vecFileName[i];
	}

	bool result = LoadTextureFullPath(vecFullPath);

	//파일명과 경로 이름을 등록한다.(*풀경로는 LoadTextureFullPath에서 처리)
	if (result)
	{
		for (size_t i = 0; i < size; ++i)
		{
			lstrcpy(m_vecImageInfo[i]->FileName, vecFileName[i].c_str());
			m_vecImageInfo[i]->PathName = PathName;
		}
	}

	return result;

}

bool CTexture::LoadTextureFullPath(const std::vector<std::tstring>& vecFullPath)
{
	size_t size = vecFullPath.size();

	for (size_t i = 0; i < size; ++i)
	{
		ImageInfo* Image = new ImageInfo;

		//메모리 DC를 얻어온다. =
		Image->hMemDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

		//이미지를 로드한다. 
		Image->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(), vecFullPath[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		//혹시나 이미지가 로드되지 않았다면 false를 리턴한다.
		if (!Image->hBmp)
			return false;

		//그리기 도구를 갈아끼우고, 이전 도구를 따로 변수를 만들어 들고 있는다.
		Image->hPrevBmp = (HBITMAP)SelectObject(Image->hMemDC, Image->hBmp);

		//비트맵 이미지의 정보를 저장한다.
		GetObject(Image->hBmp, sizeof(BITMAP), &Image->BmpInfo);

		//풀 경로 주소를 등록한다.
		lstrcpy(Image->FullPath, vecFullPath[i].c_str());

		m_vecImageInfo.push_back(Image);

	}


	//텍스처를 여러장 등록했다는 것은 Frame타입 애니메이션을 만들겠다는 소리가 된다.
	m_TextureType = ETextureType::Frame;


	return true;
}

bool CTexture::LoadTexture(ImageInfo* Info, const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Path)
		lstrcpy(FullPath, Path->Path);

	lstrcat(FullPath, FileName);

	bool Result = LoadTextureFullPath(Info, FullPath);

	if (Result)
	{
		lstrcpy(m_vecImageInfo.back()->FileName, FileName);
		m_vecImageInfo.back()->PathName = PathName;
	}

	return Result;
}

bool CTexture::LoadTextureFullPath(ImageInfo* Info, const TCHAR* FullPath)
{
	// 화면DC를 넣고 메모리 DC를 얻는다.
	HDC	hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	// 비트맵을 로딩한다.
	HBITMAP	hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (!hBmp)
		return false;

	// 읽어온 비트맵을 메모리 DC에 지정한다.
	// 기존에 DC가 가지고 있던 도구를 반환한다.
	HBITMAP hPrevBmp = (HBITMAP)SelectObject(hDC, hBmp);

	BITMAP	BmpInfo;

	GetObject(hBmp, sizeof(BITMAP), &BmpInfo);

	Info->hMemDC = hDC;
	Info->hBmp = hBmp;
	Info->hPrevBmp = hPrevBmp;
	Info->BmpInfo = BmpInfo;
	lstrcpy(Info->FullPath, FullPath);

	return true;
}

bool CTexture::LoadTexture(std::vector<ImageInfo*>* vecInfo, const std::vector<std::tstring>& vecFileName, const std::string& PathName)
{
	m_TextureType = ETextureType::Frame;

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	std::vector<std::tstring>	vecFullPath;

	size_t	Size = vecFileName.size();

	// 미리 개수만큼 push 해놓는 효과이다.
	vecFullPath.resize(Size);

	// 미리 배열 공간을 확보하여 꽉 찼을때 공간 재할당이 일어나는것을 방지해주는
	// 역할을 할때 사용한다.
	//vecFullPath.reserve(Size);

	for (size_t i = 0; i < Size; ++i)
	{
		if (Path)
			vecFullPath[i] = Path->Path;

		vecFullPath[i] += vecFileName[i];
	}

	bool Result = LoadTextureFullPath(vecInfo, vecFullPath);

	if (Result)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			lstrcpy(m_vecImageInfo[i]->FileName, vecFileName[i].c_str());
			m_vecImageInfo[i]->PathName = PathName;
		}
	}

	return Result;
}

bool CTexture::LoadTextureFullPath(std::vector<ImageInfo*>* vecInfo, const std::vector<std::tstring>& vecFullPath)
{
	m_TextureType = ETextureType::Frame;

	size_t	Size = vecFullPath.size();

	for (size_t i = 0; i < Size; ++i)
	{
		// 화면DC를 넣고 메모리 DC를 얻는다.
		HDC	hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

		// 비트맵을 로딩한다.
		// string 이나 wstring 클래스의 c_str() 함수는 문자열 포인터를 얻어온다.
		HBITMAP	hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
			vecFullPath[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (!hBmp)
			return false;

		// 읽어온 비트맵을 메모리 DC에 지정한다.
		// 기존에 DC가 가지고 있던 도구를 반환한다.
		HBITMAP hPrevBmp = (HBITMAP)SelectObject(hDC, hBmp);

		BITMAP	BmpInfo;

		GetObject(hBmp, sizeof(BITMAP), &BmpInfo);

		ImageInfo* Info = *(vecInfo->begin() + i);

		Info->hMemDC = hDC;
		Info->hBmp = hBmp;
		Info->hPrevBmp = hPrevBmp;
		Info->BmpInfo = BmpInfo;
		lstrcpy(Info->FullPath, vecFullPath[i].c_str());
	}

	return true;
}


HBITMAP* CTexture::GetHBITMAP(int index)
{
	return &m_vecImageInfo[index]->hBmp;
}

BITMAP CTexture::GetBITMAP(int index)
{
	return m_vecImageInfo[index]->BmpInfo;
}

int CTexture::GetTextureNumbers() const
{
	return (int)m_vecImageInfo.size();
}

Vector2 CTexture::GetTextureSize(int index) const
{
	Vector2 Size;
	Size.x = (float)m_vecImageInfo[index]->BmpInfo.bmWidth;
	Size.y = (float)m_vecImageInfo[index]->BmpInfo.bmHeight;

	return Size;
}

ETextureType CTexture::GetTextureType() const
{
	return m_TextureType;
}

HDC CTexture::GetDC(int index) const
{
	return m_vecImageInfo[index]->hMemDC;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
