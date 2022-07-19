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

//���� ���� ��� �޼ҵ�
bool CTexture::LoadTexture(const std::tstring& FileName, const std::string& PathName, EReactionChannel MaxChannel)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (!Path)
		return false;

	TCHAR FullPath[MAX_PATH] = {};

	//��θ� �ϴ� �־����, 
	lstrcpy(FullPath, Path->Path);
	//���ϸ��� �ٿ��ش�.
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

	//�޸� DC�� ���´�. =
	Image->hMemDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	//�̹����� �ε��Ѵ�. 
	Image->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(), FullPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//Ȥ�ó� �̹����� �ε���� �ʾҴٸ� false�� �����Ѵ�.
	if (!Image->hBmp)
		return false;

	//�׸��� ������ ���Ƴ����, ���� ������ ���� ������ ����� ��� �ִ´�.
	Image->hPrevBmp = (HBITMAP)SelectObject(Image->hMemDC, Image->hBmp);

	//��Ʈ�� �̹����� ������ �����Ѵ�.
	GetObject(Image->hBmp, sizeof(BITMAP), &Image->BmpInfo);

	lstrcpy(Image->FullPath, FullPath.c_str());

	m_vecImageInfo.push_back(Image);


	//������� �⺻ �ؽ�ó �ε� ���� ����
	//�߰��� ��Ʈ����Ʈ��, �ʿ�� ���� ����Ʈ�� �ִ� ������ ����

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

			//HSB���� �����س���
			HSB.SetParameters(&Param);

			//�������κ��� �̹��� ������ GDI+�� ����
			Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromHBITMAP(Image->hBmp, NULL);

			//��ü ���� ��ȯ�� ���� ����� �޾Ƴ���
			RECT rc;
			rc.left = 0;
			rc.top = 0;
			rc.right = bmp->GetWidth();
			rc.bottom = bmp->GetHeight();

			//ȿ�� ����
			Gdiplus::Status status = bmp->ApplyEffect(&HSB, &rc);

			//�÷�Ű�� Ȯ���� 0, 0 �ȼ��� RGB�� �޾ƿ�
			Gdiplus::Color color;
			bmp->GetPixel(0, 0, &color);
			Gdiplus::ARGB rgb = color.GetValue();
			int r = ((rgb >> RED_SHIFT) & 0x000000FF);
			int g = ((rgb >> GREEN_SHIFT) & 0x000000FF);
			int b = ((rgb >> BLUE_SHIFT) & 0x000000FF);

			//�����Ҵ�
			ImageInfo* EffectImage = new ImageInfo;

			//�޸� DC�� ���´�. =
			EffectImage->hMemDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

			// GDI ���� HBITMAP�� �޾ƿ´�
			bmp->GetHBITMAP(Gdiplus::Color(0, 0, 0), &EffectImage->hBmp);

			//Ȥ�ó� �̹����� �ε���� �ʾҴٸ� false�� �����Ѵ�.
			if (!EffectImage->hBmp)
				return false;

			//��� �޾Ƴ��� �÷�Ű ����
			EffectImage->ColorKey = RGB(r, g, b);
			EffectImage->ColorKeyEnable = true;

			//�׸��� ������ ���Ƴ����, ���� ������ ���� ������ ����� ��� �ִ´�.
			EffectImage->hPrevBmp = (HBITMAP)SelectObject(EffectImage->hMemDC, EffectImage->hBmp);

			//��Ʈ�� �̹����� ������ �����Ѵ�.
			GetObject(EffectImage->hBmp, sizeof(BITMAP), &EffectImage->BmpInfo);
			lstrcpy(EffectImage->FullPath, FullPath.c_str());

			m_vecImageInfo.push_back(EffectImage);
		}
	}

	return true;
}



bool CTexture::LoadTextureByTextureNumbers(const std::tstring& FolderPath, int NumberOfTexture, const std::string& PathName)
{
	//��θ� ã�Ƴ���
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	//������ ����
	if (!Path)
		return false;

	//������� Ǯ ��� ����ŭ �̸� ������ Ȯ���س��� ��
	std::vector<std::tstring> vecFullPath;
	vecFullPath.resize(NumberOfTexture);

	std::vector<std::tstring> vecFileName;
	vecFileName.resize(NumberOfTexture);

	//Ǯ��� �Է� ����
	for (size_t i = 0; i < NumberOfTexture; ++i)
	{
		vecFullPath[i] = Path->Path;

		//���� ��� �߰�
		vecFullPath[i] += FolderPath;

		//���� ���ϸ� ����
		TCHAR TempFileName[MAX_PATH] = {};
		tsPrintf_s(TempFileName, _countof(TempFileName), TEXT("/%d.bmp"), (int)i);

		//���ϸ� �߰�
		vecFullPath[i] += TempFileName;

		//���ϸ� �޾Ƴ���(�ڿ��� ���)
		vecFileName[i] = TempFileName;
		

	}

	//�ε��ϰ� �����ϸ� ���� �̸�, �ּ�, �̸��� ���.
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
	//��θ� ã�Ƴ���
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	//������ ����
	if (!Path)
		return false;

	//���� �̸� ����Ʈ�� ����� ����� �޾Ƴ���
	size_t size = vecFileName.size();

	//������� Ǯ ��� ����ŭ �̸� ������ Ȯ���س��� ��
	std::vector<std::tstring> vecFullPath;
	vecFullPath.resize(size);

	//Ǯ��� �Է�
	for (size_t i = 0; i < size; ++i)
	{
		vecFullPath[i] = Path->Path;
		vecFullPath[i] += vecFileName[i];
	}

	bool result = LoadTextureFullPath(vecFullPath);

	//���ϸ�� ��� �̸��� ����Ѵ�.(*Ǯ��δ� LoadTextureFullPath���� ó��)
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

		//�޸� DC�� ���´�. =
		Image->hMemDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

		//�̹����� �ε��Ѵ�. 
		Image->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(), vecFullPath[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		//Ȥ�ó� �̹����� �ε���� �ʾҴٸ� false�� �����Ѵ�.
		if (!Image->hBmp)
			return false;

		//�׸��� ������ ���Ƴ����, ���� ������ ���� ������ ����� ��� �ִ´�.
		Image->hPrevBmp = (HBITMAP)SelectObject(Image->hMemDC, Image->hBmp);

		//��Ʈ�� �̹����� ������ �����Ѵ�.
		GetObject(Image->hBmp, sizeof(BITMAP), &Image->BmpInfo);

		//Ǯ ��� �ּҸ� ����Ѵ�.
		lstrcpy(Image->FullPath, vecFullPath[i].c_str());

		m_vecImageInfo.push_back(Image);

	}


	//�ؽ�ó�� ������ ����ߴٴ� ���� FrameŸ�� �ִϸ��̼��� ����ڴٴ� �Ҹ��� �ȴ�.
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
	// ȭ��DC�� �ְ� �޸� DC�� ��´�.
	HDC	hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

	// ��Ʈ���� �ε��Ѵ�.
	HBITMAP	hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (!hBmp)
		return false;

	// �о�� ��Ʈ���� �޸� DC�� �����Ѵ�.
	// ������ DC�� ������ �ִ� ������ ��ȯ�Ѵ�.
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

	// �̸� ������ŭ push �س��� ȿ���̴�.
	vecFullPath.resize(Size);

	// �̸� �迭 ������ Ȯ���Ͽ� �� á���� ���� ���Ҵ��� �Ͼ�°��� �������ִ�
	// ������ �Ҷ� ����Ѵ�.
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
		// ȭ��DC�� �ְ� �޸� DC�� ��´�.
		HDC	hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());

		// ��Ʈ���� �ε��Ѵ�.
		// string �̳� wstring Ŭ������ c_str() �Լ��� ���ڿ� �����͸� ���´�.
		HBITMAP	hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
			vecFullPath[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (!hBmp)
			return false;

		// �о�� ��Ʈ���� �޸� DC�� �����Ѵ�.
		// ������ DC�� ������ �ִ� ������ ��ȯ�Ѵ�.
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
